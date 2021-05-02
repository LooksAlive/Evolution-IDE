#include <QApplication>
#include <QFileInfo>
#include <QPainter>
#include <QScrollBar>
#include <QSettings>
#include <QShortcut>
#include <QTextBlock>
#include <QTextStream>

// have to be there, parsing error with some .inc files
#include "Clang/ClangBridge.h"
#include "Widgets/CodeInfoDock/CodeInfoDock.h"
#include "Widgets/NodeView/NodeView.h"
#include "plaintextedit.h"

PlainTextEdit::PlainTextEdit(QWidget *parent)
        : QPlainTextEdit(parent) {
    SetupAdditionalWidgets();

    // queue is critical for geometry
    statusArea = new StatusArea(this);
    breakPointArea = new BreakPointArea(this);
    codeNotifyArea = new CodeNotifyArea(this);
    lineNumberArea = new LineNumberArea(this);
    arrowArea = new ArrowArea(this);
    scrollBar = new ScrollBar(this);

    setVerticalScrollBar(scrollBar);
    // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QSettings settings;
    QFont font;
#ifdef Q_OS_WIN
    font.setFamily(settings.value("editor_font", "Courier New").toString());
#elif defined(Q_OS_MACOS)
    font.setFamily(settings.value("editor_font", "Monaco").toString());
#else
    font.setFamily(settings.value("editor_font", "Ubuntu Mono").toString());
#endif
    font.setFixedPitch(true);
    font.setPointSize(settings.value("editor_font_size", 10).toInt());
    font.setStyleHint(QFont::Monospace);
    const bool whitespaces = settings.value("editor_whitespaces", false).toBool();
    if (whitespaces) {
        QTextOption options;
        options.setFlags(QTextOption::ShowTabsAndSpaces);
        document()->setDefaultTextOption(options);
    }


    setCursorWidth(2);
    setFrameStyle(QFrame::NoFrame);
    setFont(font);
    setTabChangesFocus(false);
    setWordWrapMode(QTextOption::NoWrap);
    setReadOnly(false);
    setMouseTracking(true);
    setEnabled(true);
    setCenterOnScroll(false);// when scrolling cursor is visible in the smallest view
    setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextEditorInteraction);    // links

    /*
    touchTimer = new QTimer(this);
    connect(touchTimer, &QTimer::timeout, this, [=](){ searchByMouseTouch(); });
    touchTimer->setInterval(4000);
    touchTimer->start();
    */
    // LineArea
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &PlainTextEdit::slotHighlightCurrentLine);
    connect(this, &QPlainTextEdit::blockCountChanged, this, &PlainTextEdit::slotBlockCountChanged);
    connect(this, &QPlainTextEdit::textChanged, this, &PlainTextEdit::slotTextChanged);
    connect(this, &QPlainTextEdit::updateRequest, this, &PlainTextEdit::slotUpdateRequest);

    // BreakpointArea

    connect(new QShortcut(Qt::CTRL + Qt::Key_U, this), &QShortcut::activated, [=] {
        transformText(true);
    });
    connect(new QShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_U, this), &QShortcut::activated, [=] {
        transformText(false);
    });
    connect(new QShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Up, this), &QShortcut::activated, [=] {
        moveSelection(true);
    });
    connect(new QShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Down, this), &QShortcut::activated, [=] {
        moveSelection(false);
    });
    connect(new QShortcut(Qt::CTRL + Qt::Key_Plus, this), &QShortcut::activated, [=] {
        zoomIn();
    });
    connect(new QShortcut(Qt::CTRL + Qt::Key_Minus, this), &QShortcut::activated, [=] {
        zoomOut();
    });

    createMenu();
    ensureCursorVisible();
    appendHtml("<a style=text-decoration:none;color:gray; href=content>hrejfForContent</a><br>");
    arrowArea->expanded.push_back(1);   // first line
    arrowArea->expanded.push_back(2);   // first line
    arrowArea->expanded.push_back(3);   // first line
}

QRectF PlainTextEdit::blockBoundingGeometryProxy(const QTextBlock &block) const {
    return blockBoundingGeometry(block);
}

QRectF PlainTextEdit::blockBoundingRectProxy(const QTextBlock &block) const {
    return blockBoundingRect(block);
}

QPointF PlainTextEdit::contentOffsetProxy() const {
    return contentOffset();
}


QTextBlock PlainTextEdit::firstVisibleBlockProxy() const {
    return firstVisibleBlock();
}


void PlainTextEdit::setCursorPosition(const int &x, const int &y) {
    doNotSetSelections = true;
    // no idea why -1 for both coords, but works that way ... in searching
    if (!isReadOnly()) {
        QTextCursor cursor = textCursor();
        cursor.setPosition(document()->findBlockByLineNumber(y - 1).position());
        cursor.movePosition(QTextCursor::NextCharacter,
                            QTextCursor::MoveAnchor,
                            x - 1);
        setTextCursor(cursor);
        ensureCursorVisible();
    }
    /*
    const QTextBlock block = document()->findBlockByLineNumber(row - 1);  // - 1
    if(block.isValid())
    {
        QTextCursor cursor = textCursor();
        cursor.setPosition(block.position() + col);  // + 1, bc. col cannot be 0
        setTextCursor(cursor);
        ensureCursorVisible();
    }
    */
}

// cursor
void PlainTextEdit::setCursorPosition(QTextCursor &cursor, const int &x, const int &y) {
    cursor.setPosition(document()->findBlockByNumber(y - 1).position());
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, x - 1);
}

QPoint PlainTextEdit::getCursorPosition() {
    QTextCursor cursor = textCursor();
    // there is no 0 col or row in view but it is acts like it is
    const int y = cursor.blockNumber() + 1;
    const int x = cursor.columnNumber() + 1;

    return QPoint(x, y);
}

QPoint PlainTextEdit::getCursorPosition(QTextCursor &cursor) {
    // there is no 0 col or row in view but it is acts like it is
    const int y = cursor.blockNumber() + 1;
    const int x = cursor.columnNumber() + 1;

    return QPoint(x, y);
}

void PlainTextEdit::setCursorAtLine(const int &line) {
    setCursorPosition(1, line);// +1 ; yet no idea how ? jumps as it wants
}

// text manipulation
void PlainTextEdit::selectLineUnderCursor() {
    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    setTextCursor(cur);
    ensureCursorVisible();
}

void PlainTextEdit::selectLineUnderCursor(QTextCursor &cursor) {
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
}

QString PlainTextEdit::getLineUnderCursor() {
    //selectLineUnderCursor();
    QTextCursor cur = textCursor();
    cur.select(QTextCursor::LineUnderCursor);
    QString line = cur.selectedText();
    cur.clearSelection();
    //setTextCursor(cur);
    //ensureCursorVisible();
    return line;
}

QString PlainTextEdit::getLineUnderCursor(QTextCursor &cursor) {
    selectLineUnderCursor(cursor);
    QString line = cursor.selectedText();
    cursor.clearSelection();
    return line;
}

QString PlainTextEdit::getLineContent(const int &row, const bool &setCursor) {
    if (!setCursor) {
        QTextCursor cursor = textCursor();
        setCursorPosition(cursor, 1, row);
        return getLineUnderCursor(cursor);
    }
    setCursorPosition(1, row);
    return getLineUnderCursor();
}

void PlainTextEdit::selectWordUnderCursor() {
    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    setTextCursor(cur);
    ensureCursorVisible();
}

QString PlainTextEdit::getWordUnderCursor(){
    //selectWordUnderCursor();
    QTextCursor cur = textCursor();
    cur.select(QTextCursor::WordUnderCursor);
    QString word = cur.selectedText();
    cur.clearSelection();
    // do not set, bc. cursor will end up at the end of the given word
    //setTextCursor(cur);
    return word;
}

void PlainTextEdit::completerInsertText(const QString &text) {
    if (completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    const int extra = text.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(text.right(extra));
    setTextCursor(tc);
}

void PlainTextEdit::slotInsertSuggestion(QListWidgetItem *item) {
    const QString newWord = item->text();
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.removeSelectedText();
    cursor.insertText(newWord);
    setTextCursor(cursor);
}

void PlainTextEdit::slotInsertTagComment(QListWidgetItem *item) {
    const QString newWord = item->text();
    QTextCursor cursor = textCursor();
    cursor.insertText("// " + newWord);
    setTextCursor(cursor);
}

void PlainTextEdit::deleteLine() {
    selectLineUnderCursor();
    QTextCursor cur = textCursor();
    if (cur.selectedText() != "") {
        cur.removeSelectedText();
    }
    setTextCursor(cur);
}

void PlainTextEdit::deleteLine(QTextCursor &cursor) {
    selectLineUnderCursor(cursor);
    if (cursor.selectedText() != "") {
        cursor.removeSelectedText();
    }
    setTextCursor(cursor);
}

void PlainTextEdit::toggleComment() {

    //QString file_extension;  // default is cpp
    QTextCursor cur = textCursor();
    if (!cur.hasSelection()) {
        selectLineUnderCursor();
    }

    int endSelection = cur.selectionEnd();
    cur.setPosition(cur.selectionStart());
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    setTextCursor(cur);

    QString line = getLineUnderCursor();

    QString commentStart;
    QString commentEnd;

    if(file_extension == "html" | file_extension == "css"){

        if(file_extension == "html"){
            commentStart = "<!--";
            commentEnd = "-->";
        }else{
            commentStart = "/*";
            commentEnd = "*/";
        }

        if(line.startsWith(commentStart)){
            for(int i=0; i < commentStart.length(); i++){
                cur.deleteChar();
            }
            endSelection -= commentStart.length();

            cur.setPosition(endSelection);
            cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
            setTextCursor(cur);
            QString line2 = getLineUnderCursor();

            if (line2.endsWith(commentEnd)) {
                for (int i = 0; i < commentEnd.length(); i++) {
                    cur.deletePreviousChar();
                }
            }
        }else{
            cur.insertText(commentStart);
            endSelection += commentStart.length();

            cur.setPosition(endSelection);
            cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
            setTextCursor(cur);
            QString line2 = getLineUnderCursor();

            if (!line2.endsWith(commentEnd)) {
                cur.insertText(commentEnd);
            }
        }

    }else{
        if(file_extension == "py"){
            commentStart = "#";
        }else{
            commentStart = "//";
        }

        if(line.startsWith(commentStart)){
            for(int i=0; i < commentStart.length(); i++){
                cur.deleteChar();
            }
            endSelection -= commentStart.length();
            cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);

            while(cur.position() < endSelection){
                cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
                setTextCursor(cur);
                line = getLineUnderCursor();

                if(line.startsWith(commentStart)){
                    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
                    for(int i=0; i < commentStart.length(); i++){
                        cur.deleteChar();
                    }
                    endSelection -= commentStart.length();

                }
                cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);

            }

        }else{
            cur.insertText(commentStart);
            endSelection += commentStart.length();
            cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);

            while(cur.position() < endSelection){
                cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
                setTextCursor(cur);
                line = getLineUnderCursor();

                if(!line.startsWith(commentStart)){
                    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
                    cur.insertText(commentStart);
                    endSelection += commentStart.length();
                }

                cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
            }
        }

    }

    setTextCursor(cur);
}

int PlainTextEdit::indentSize(const QString &text)
{
    int count = 0;
    int i = 0;
    int length = text.length();
    if (length == 0) {
        return 0;
    }
    QChar current = text.at(i);
    while ((i < length) && ((current == ' ') || (current == QChar('\t')))) {
        if (current == QChar('\t')) {
            count++;
            i++;
        } else if (current == ' ') {
            int j = 0;
            while ((i + j < length) && (text.at(i + j) == ' ')) {
                j++;
            }
            i += j;
            count += j / TAB_STOP_WIDTH;
        }
        if (i < length) {
            current = text.at(i);
        }
    }
    return count;
}

bool PlainTextEdit::indentText(const bool &forward)
{
    QTextCursor cursor = textCursor();
    QTextCursor clone = cursor;
    if (!cursor.hasSelection()) {
        return false;
    }
    int start = cursor.selectionStart();
    cursor.setPosition(cursor.selectionStart());
    clone.setPosition(clone.selectionEnd());
    int stop = clone.blockNumber();
    cursor.beginEditBlock();
    do {
        int position = cursor.position();
        QString text = cursor.block().text();
        int count = indentSize(text);
        if (forward) {
            count++;
        } else if (count > 0) {
            count--;
        }
        cursor.select(QTextCursor::LineUnderCursor);
        if (forward) {
            if (TABS_TO_SPACES) {
                cursor.insertText(QString(TAB_STOP_WIDTH, ' ') + text);
            } else {
                cursor.insertText(QChar('\t') + text);
            }
        } else {
            cursor.insertText(indentText(text, count));
        }
        cursor.setPosition(position);
        if (!cursor.movePosition(QTextCursor::NextBlock)) {
            break;
        }
    }
    while(cursor.blockNumber() <= stop);
    cursor.setPosition(start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    while (cursor.block().blockNumber() < stop) {
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
    }
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.endEditBlock();
    setTextCursor(cursor);
    return true;
}

QString PlainTextEdit::indentText(QString text, int count) const
{
    if (text.isEmpty()) {
        return text;
    }
    while ((text.at(0) == ' ') || (text.at(0) == '\t')) {
        text.remove(0, 1);
        if (text.isEmpty()) {
            break;
        }
    }
    while (count != 0) {
        text = text.insert(0, TABS_TO_SPACES ? QString(TAB_STOP_WIDTH, ' ') : QChar('\t'));
        count--;
    }
    return text;
}

bool PlainTextEdit::autoEnterTextIndentation() {
    QTextCursor cursor = textCursor();
    // we are at line we were before enter, return, just with pointer
    // get line content
    const QString lineContent = getLineUnderCursor(cursor);
    cursor.clearSelection();
    // count spaces, tabs
    int spaces = 0;
    int tabs = 0;
    for (const auto &i : lineContent) {
        if (i == ' ') {
            spaces++;
        }
        if (i == '\t') {
            tabs++;
        }
    }

    // insert 1 line with spaces, tabs
    if (spaces != 0) {
        QString space;
        // cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        cursor.beginEditBlock();
        cursor.insertText(space.fill(QLatin1Char(' '), spaces).prepend("\n"));
        cursor.endEditBlock();
        setTextCursor(cursor);
        return true;
    }
    if (tabs != 0) {
        QString space;
        // cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        cursor.beginEditBlock();
        cursor.insertText(space.fill(QLatin1Char('\t'), tabs).prepend("\n"));
        cursor.endEditBlock();
        setTextCursor(cursor);
        return true;
    } else {
        // no change, needed, do not set cursor so nothing really happened
        return false;
    }
}

void PlainTextEdit::autoBlankLineDeletion() {
    QTextCursor cursor = textCursor();
    // get line content
    const QString lineContent = getLineUnderCursor(cursor);
    for (const auto &i : lineContent) {
        if (i == ' ' || i == '\t') {

        } else {
            return;
        }
    }
    // remove whole line
    cursor.beginEditBlock();
    selectLineUnderCursor(cursor);
    cursor.removeSelectedText();
    cursor.endEditBlock();
    // cursor.movePosition(QTextCursor::PreviousBlock);
    setTextCursor(cursor);
}

bool PlainTextEdit::autoTextSurrounding(QTextCursor &cursor, const QString &pair) {
    const QString selectedText = cursor.selectedText();
    if (selectedText.isEmpty()) {
        return false;
    }
    // it is easier to remove it and insert formated text
    if (pair == "{") {
        const QString newText = "{" + selectedText + "}";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    if (pair == "[") {
        const QString newText = "[" + selectedText + "]";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    if (pair == "(") {
        const QString newText = "(" + selectedText + ")";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    if (pair == "\'") {
        const QString newText = "'" + selectedText + "'";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    if (pair == "\"") {
        const QString newText = "\"" + selectedText + "\"";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    if (pair == "<") {
        const QString newText = "<" + selectedText + ">";
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.movePosition(QTextCursor::EndOfWord);
        setTextCursor(cursor);
        return true;
    }
    // never reach
    return false;
}

void PlainTextEdit::moveCursor(const bool &end) {
    QTextCursor cursor = textCursor();
    int length = cursor.block().text().length();
    if (length != 0) {
        int original = cursor.position();
        QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
        if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
            mode = QTextCursor::KeepAnchor;
        }
        cursor.movePosition(QTextCursor::StartOfLine, mode);
        int start = cursor.position();
        int i;
        if (end) {
            cursor.movePosition(QTextCursor::EndOfLine, mode);
            i = length;
            while (cursor.block().text()[i - 1].isSpace()) {
                i--;
                if (i == 1) {
                    i = length;
                    break;
                }
            }
        } else {
            i = 0;
            while (cursor.block().text()[i].isSpace()) {
                i++;
                if (i == length) {
                    i = 0;
                    break;
                }
            }
        }
        if ((original == start) || ((start + i) != original)) {
            cursor.setPosition(start + i, mode);
        }
        setTextCursor(cursor);
    }
}

/* moving selected text arround */
void PlainTextEdit::moveSelection(const bool &up)
{
    QTextCursor original = textCursor();
    QTextCursor moved = original;
    moved.setVisualNavigation(false);
    moved.beginEditBlock();
    bool selected = original.hasSelection();
    if (selected) {
        moved.setPosition(original.selectionStart());
        moved.movePosition(QTextCursor::StartOfBlock);
        moved.setPosition(original.selectionEnd(), QTextCursor::KeepAnchor);
        moved.movePosition(moved.atBlockStart() ? QTextCursor::Left : QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    } else {
        moved.movePosition(QTextCursor::StartOfBlock);
        moved.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    }
    QString text = moved.selectedText();
    moved.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    moved.removeSelectedText();
    if (up) {
        moved.movePosition(QTextCursor::PreviousBlock);
        moved.insertBlock();
        moved.movePosition(QTextCursor::Left);
    } else {
        moved.movePosition(QTextCursor::EndOfBlock);
        if (moved.atBlockStart()) {
            moved.movePosition(QTextCursor::NextBlock);
            moved.insertBlock();
            moved.movePosition(QTextCursor::Left);
        } else {
            moved.insertBlock();
        }
    }
    int start = moved.position();
    moved.clearSelection();
    moved.insertText(text);
    int end = moved.position();
    if (selected) {
        moved.setPosition(start);
        moved.setPosition(end, QTextCursor::KeepAnchor);
    }
    moved.endEditBlock();
    setTextCursor(moved);
}

void PlainTextEdit::transformText(const bool &upper)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    QString before = cursor.selectedText();
    QString after = upper ? before.toUpper() : before.toLower();
    if (before != after) {
        cursor.beginEditBlock();
        cursor.deleteChar();
        cursor.insertText(after);
        cursor.endEditBlock();
        setTextCursor(cursor);
    }
}

void PlainTextEdit::highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background, const int &line,
                              const QColor &color)
{
    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        if(Background){
            selection.format.setBackground(color);
        }else{
            selection.format.setForeground(color);
        }
        selection.format.setProperty(QTextFormat::FontWeight, true);
        selection.cursor=textCursor();
        selection.cursor.clearSelection();
        if(line){
            selection.cursor.setPosition(textCursor().block().position());
        }
        selections.append(selection);
    }
}

// search
void PlainTextEdit::findStoreAndSelectAll(const QString &search, const QTextDocument::FindFlags &find_options)
{
    // ensure no other selections are here
    extra_selections_search_results.clear();
    search_results.clear();
    QTextCursor cursor = textCursor();
    //extra_selections_search_results.setSharable(true);
    // set to start, 1 file search
    setCursorPosition(cursor, 1, 1);

    QColor highlight(Qt::darkRed);
    highlight.setAlpha(25);
    //highlight.setRgb(0, 255, 0);
    // results do not contain search text, it simply got where it is used and parsed as argument
    while (find(cursor, search, find_options)) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(highlight);
        selection.cursor = cursor;
        //selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
        //qDebug() << selection.cursor.selectedText();
        extra_selections_search_results.append(selection);
        // !!!!!!!!!! clear it after append !!!!!!!!!!!!!
        selection.cursor.clearSelection();

        // get data
        search_results.push_back(getCursorPosition(cursor));
        // qDebug() << getCursorPosition(cursor);
    }
    // set position we came from
    updateExtraSelections();
}

bool PlainTextEdit::find(const QString &search, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    QTextCursor cursor = textCursor();
    cursor = document()->find(search, cursor, find_options);

    if (!cursor.isNull()) {
        // cursor.movePosition(QTextCursor::NextCharacter);
        setTextCursor(cursor);
        return true;
    } else {
        return false;// no occurrences found
    }
}

bool PlainTextEdit::find(QTextCursor &cursor, const QString &search, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    cursor = document()->find(search, cursor, find_options);
    if (!cursor.isNull()) {
        return true;
    } else {
        return false;// no occurrences found
    }
}

void PlainTextEdit::findNext(const QString &search, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    // findStoreAndSelectAll(search, find_options); // find all results and select them first

    QTextCursor cursor = textCursor();
    // find_options |= QTextDocument::FindBackward;   // is const now -> auditing in slot in widget
    cursor = document()->find(search, cursor, find_options);

    //qDebug() << cursor.isNull();
    if(!cursor.isNull()){
        setTextCursor(cursor);
    }else{
        // try to start at start of document in case that are there some occurrences left
        cursor.movePosition(QTextCursor::Start);
        cursor = document()->find(search, cursor, find_options);
        if(!cursor.isNull()){
            setTextCursor(cursor);
        }else{
            return;   // no occurrences found
        }
    }
}

void PlainTextEdit::replace(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    QTextCursor cursor = textCursor();
    findNext(oldText, find_options);

    if (cursor.hasSelection() && !isReadOnly() /* && cursor.selectedText() == oldText */) {
        //cursor.beginEditBlock();
        cursor.removeSelectedText();
        cursor.insertText(newText);
        //cursor.endEditBlock();
        setTextCursor(cursor);
    }
}

void PlainTextEdit::replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    replace(oldText, newText, find_options);
    findNext(oldText, find_options);
}

int PlainTextEdit::replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options) {
    doNotSetSelections = true;
    int count = 0;
    //textCursor().movePosition(QTextCursor::Start); // on start, works yet only with one file
    setCursorPosition(1, 1);

    while (find(oldText, find_options)) {
        replace(oldText, newText, find_options);
        count++;
    }
    return count;
}

// other
void PlainTextEdit::createMenu() {
    viewMenu = new QMenu(this);

    viewMenu->addAction(QIcon(IconFactory::Copy), "Copy", this, SLOT(copy()), Qt::CTRL + Qt::Key_C);
    viewMenu->addAction(QIcon(IconFactory::Paste), "Paste", this, SLOT(paste()), Qt::CTRL + Qt::Key_V);
    viewMenu->addAction(QIcon(IconFactory::Cut), "Cut", this, SLOT(cut()), Qt::CTRL + Qt::Key_X);
    viewMenu->addAction(QIcon(IconFactory::Undo), "Undo", this, SLOT(undo()), Qt::CTRL + Qt::Key_Z);
    viewMenu->addAction(QIcon(IconFactory::Redo), "Redo", this, SLOT(redo()), Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    viewMenu->addAction(QIcon(IconFactory::SelectAll), "Select All", this, SLOT(selectAll()), Qt::CTRL + Qt::Key_A);
    viewMenu->addAction(QIcon(IconFactory::Collapse), "Collapse", this, SLOT(slotCollapse()));
    viewMenu->addAction(QIcon(IconFactory::Expand), "Expand", this, SLOT(slotExpand()));
    viewMenu->addSeparator();
    viewMenu->addAction("Create Sample", this, SLOT(slotShowNewSampleWindow()));
    viewMenu->addAction(QIcon(IconFactory::Comment), "Comment Region", this, SLOT(toggleComment()),
                        Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    viewMenu->addSeparator();
    viewMenu->addAction("Toggle BreakPoint", this, SLOT(slotToggleBreakPoint()));// , Qt::Key_F9
    viewMenu->addAction("Generate...", this, SLOT(slotGenerate()), Qt::CTRL + Qt::SHIFT + Qt::Key_G);
    viewMenu->addAction("Format File", this, SLOT(formatFile()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);
    viewMenu->addAction("Go to Definition", this, SLOT(slotGoToDefinition()), Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    viewMenu->addAction("Find References", this, SLOT(slotFindReferences()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);

    viewMenu->addAction("Add comment tag", this, SLOT(slotAddCommentTags()),
                        Qt::CTRL + Qt::SHIFT + Qt::Key_T);
    viewMenu->addAction("Comment tags", this, SLOT(slotShowCommentTags()), Qt::CTRL + Qt::SHIFT + Qt::Key_T);

    viewMenu->addAction("Show in nodes", this, SLOT(slotShowInNodeView()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotShowMenu(const QPoint&)));
}

void PlainTextEdit::SetupAdditionalWidgets() {
    hoverInfo = new HoverInfoWidget(this);

    QStringList words;
    words << "void"
          << "bool"
          << "int"
          << "uint"
          << "unsigned"
          << "short"
          << "char"
          << "wchar"
          << "const"
          << "constexpr"
          << "noexcept"
          << "if"
          << "else"
          << "switch"
          << "case"
          << "for"
          << "while"
          << "explicit";
    //completer->setModel(new QStringListModel(words, completer));

    completer = new Completer(this);
    completer->setWidget(this);
    // completer->setModel(new QStringListModel(words, completer));
    //completer->popup()->setMouseTracking(true);
    //completer->popup()->setTabletTracking(true);
    //auto *model = new QFileSystemModel(this);
    //model->setRootPath(QDir::homePath());
    //completer->popup()->setMinimumSize(200, 100);

    connect(completer, SIGNAL(activated(const QString &)), this, SLOT(completerInsertText(const QString &)));

    //completer->addItems("function", "void");
    //completer->addItems("variable", "int");

    completer->addItems(words);

    spellCheckPopup = new SpellCheckList(this);
    connect(spellCheckPopup, SIGNAL(itemClicked(QListWidgetItem * )), this,
            SLOT(slotInsertSuggestion(QListWidgetItem * )));

    tagListPopup = new CommentTagList(this);
    connect(tagListPopup, SIGNAL(itemClicked(QListWidgetItem * )), this,
            SLOT(slotInsertTagComment(QListWidgetItem * )));

    infoMessage = new InformativeMessage(this);

    smallEdit = new SmallRoundedEdit(this);
    smallEdit->verticalScrollBar()->setVisible(false);

    documentationHelper = new DocumentationHelper(this);
    documentationHelper->setEdit(this);
}

void PlainTextEdit::showMessage(const QString &message) {
    infoMessage->setGeometry(cursorRect());
    infoMessage->setMessage(message);
    infoMessage->show();
    QTimer::singleShot(3000, this, [=] { infoMessage->hide(); });
}

QString PlainTextEdit::indentText(const QString &text) {
    QString newText;

    // indentText(QString text, int count)

    if(indentSize(text) > indentationSize) {
        indentText(false);
    }
    else {
        indentText(true);
    }

    return newText;
}

QString PlainTextEdit::pasteFormatText(const QString &text) {
    QString newText;

    newText = findStoreUrlsInText(text);
    newText = indentText(text);

    return newText;
}

QString PlainTextEdit::findStoreUrlsInText(const QString &text) {
    QRegularExpression re(
      "^"
      "(?:(?:https?|ftp)://)"
      "(?:\\S+(?::\\S*)?@)?"
      "(?:"
      "(?!(?:10|127)(?:\\.\\d{1,3}){3})"
      "(?!(?:169\\.254|192\\.168)(?:\\.\\d{1,3}){2})"
      "(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})"
      "(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])"
      "(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}"
      "(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))"
      "|"
      "(?:(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)"
      "(?:\\.(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)*"
      "(?:\\.(?:[a-z\\u00a1-\\uffff]{2,}))"
      "\\.?"
      ")"
      "(?::\\d{2,5})?"
      "(?:[/?#]\\S*)?"
      "$"
    );

    re.setPatternOptions(QRegularExpression::MultilineOption |
                       QRegularExpression::DotMatchesEverythingOption |
                       QRegularExpression::CaseInsensitiveOption);

    auto match = re.globalMatch(text);
    /*
    if ( match.hasMatch()) {
      qDebug() << match.captured(0);
    } else {
      qDebug() << "Nothing found";
    }
    */

    // set them into link
    QString newText;

    while(match.hasNext()) {
        const auto m = match.next();
        m.captured();
    }

    return newText;
}

int PlainTextEdit::countSpaceTabs(const QString &lineContent, const int &indentation) {
    int spaces = 0;
    int tabs = 0;
    for(const auto& ch : lineContent) {
        if(ch == ' ') {
            spaces++;
        }
        if(ch == '\t') {
            tabs++;
        }
        if(ch != ' ' || ch == '\t') {
            qDebug() << "spaces: ";
            qDebug() << spaces;

            if(spaces != 0) {
                return spaces / indentation;
            }
            if(tabs != 0) {
                return tabs;
            }
        }
    }

    return 0;
}

void PlainTextEdit::setFileExtension(const QString &extension) {
    file_extension = extension;
    // TODO: move this where this function is called, extension set...
    if (file_extension == "cpp" || file_extension == "h") {
        completer->defaultCompletionData = C_CPP_Statements + C_CPP_Types;
        // update data
        completer->addItems(QStringList());
    }
    if (file_extension == "py") {
        completer->defaultCompletionData = pythonTypes;
        completer->addItems(QStringList());
    }
}

void PlainTextEdit::setFilePath(const QString &file_path) {
    filepath = file_path;
    if (QFileInfo(filepath).fileName() == "CmakeLists.txt") {
        completer->defaultCompletionData = cmakeCommands;
    }
}

QString PlainTextEdit::getFilePath() const {
    return filepath;
}

bool PlainTextEdit::toggleBreakPoint(const int& line) const {
    if (breakPointArea->containBlock(line - 1)) {
        return false;
    }
    return true;
}

void PlainTextEdit::searchPairsSelections(QTextCursor &cursor, const QString &first) {
    QString second;
    if (first == "(")
        second = ")";
    if (first == "[")
        second = "]";
    if (first == "{")
        second = "}";
    if (first == "<")
        second = ">";
    if (first == "\"")
        second = "\"";
    if (first == "\'")
        second = "\'";

    if (second.isEmpty()) {
        qDebug() << "empty second pair with " + first;
        return;
    }


    //const int pos = cursor.position();

    QColor highlight = Qt::darkGreen;
    highlight.setAlpha(25);
    // first pair insert,   ASC, (DESC) ; solved with nested occurrences
    QTextEdit::ExtraSelection selection;

    find(cursor, first, QTextDocument::FindWholeWords);
    selection.cursor = cursor;
    selection.format.setBackground(highlight);
    //selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
    extra_selections_search_touched_results.append(selection);
    selection.cursor.clearSelection();


    QTextEdit::ExtraSelection selection2;

    find(cursor, second, QTextDocument::FindWholeWords);
    selection2.cursor = cursor;
    selection2.format.setBackground(highlight);
    //selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
    extra_selections_search_touched_results.append(selection2);
    selection.cursor.clearSelection();

    //cursor.setPosition(pos);

    // pos before
    /*
    int temp_pos = textCursor().position();
    find(first);
    int pos_f = textCursor().position();
    find(second);
    int pos_s = textCursor().position();
    while(pos_f > pos_s){
        temp_pos = textCursor().position();
        find(first);
        pos_f = textCursor().position();
        find(second);
        pos_s = textCursor().position();
        // if there is something in between try to jump over by 1 char
        if(pos_f > pos_s){
            textCursor().setPosition(temp_pos);
            //textCursor().movePosition(QTextCursor::NextCharacter);
        }
        else{
            QTextEdit::ExtraSelection selection;

            selection.cursor = textCursor();
            selection.format.setBackground(highlight);
            //selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
            extra_selections_search_touched_results.append(selection);
            selection.cursor.clearSelection();
        }
    }
    */
}

void PlainTextEdit::setLineSelection(const int &line, const PlainTextEdit::selectionType &type, const bool &removeAll) {
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = textCursor();
    setCursorPosition(cursor, 1, line);
    selection.cursor = cursor;
    QColor color;
    selection.format.setBackground(color);
    selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);

    switch (type) {
        case Warning:
            if (removeAll) {
                extra_selections_line.clear();
                return;
            }
            color.setRgb(0, 0, 255);
            extra_selections_line.append(selection);
            selection.cursor.clearSelection();
            break;
        case Error:
            if (removeAll) {
                extra_selections_line.clear();
                return;
            }
            color.setRgb(255, 0, 0);
            extra_selections_line.append(selection);
            selection.cursor.clearSelection();
            break;
        case BreakPoint:
            if (removeAll) {
                extra_selections_line.clear();
                return;
            }
            color.setRgb(255, 0, 150);
            extra_selections_line.append(selection);
            selection.cursor.clearSelection();
            break;
        case SpellCheck:
            color.setRgb(0, 255, 0);
            extra_selections_line.append(selection);
            selection.cursor.clearSelection();
            break;
    }
    updateExtraSelections();
}

void PlainTextEdit::setUnderLineSelection(const QPoint &start, const QPoint &end, const selectionType &type) {
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = textCursor();
    setCursorPosition(cursor, start.x(), start.y());
    // compute how many character we should move from start.x()
    const int pos = document()->findBlockByNumber(end.y() - 1).position() + end.x();
    //cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, end.x());
    cursor.setPosition(pos, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    switch (type) {
        case Warning:
            selection.format.setUnderlineColor(QColor(0, 0, 255));
            selection.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            extra_selections_underline.append(selection);
            selection.cursor.clearSelection();
            break;
        case Error:
            selection.format.setUnderlineColor(QColor(255, 0, 0));
            selection.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
            extra_selections_underline.append(selection);
            selection.cursor.clearSelection();
            break;
        case BreakPoint:
            selection.format.setUnderlineColor(QColor(255, 0, 150));
            selection.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            extra_selections_underline.append(selection);
            selection.cursor.clearSelection();
            break;
        case SpellCheck:
            selection.format.setUnderlineColor(QColor(0, 255, 0));
            selection.format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
            extra_selections_underline.append(selection);
            selection.cursor.clearSelection();
            break;
    }
    updateExtraSelections();
}

void PlainTextEdit::setCollapsableText(const QPoint &start, const QPoint &end) {
    QTextCursor cursor = textCursor();
    setCursorPosition(cursor, start.x(), start.y());
    const int pos = document()->findBlockByNumber(end.y() - 1).position() + end.x();
    cursor.setPosition(pos, QTextCursor::KeepAnchor);
    const QString originalText = cursor.selectedText();
    cursor.removeSelectedText();
    cursor.insertHtml(
            "<a style=text-decoration:none;color:gray; href= " + originalText + ">..." + "</a>");  // or insertHtml()
    setTextCursor(cursor);
}

void PlainTextEdit::updateExtraSelections() {
    setExtraSelections(extra_selections_current_line + extra_selections_search_results +
                       extra_selections_search_touched_results +
                       extra_selections_line + extra_selections_underline);
}


/* slots
 ------------------------------------------------------------------------- */
void PlainTextEdit::slotShowMenu(const QPoint &pos) {
    viewMenu->exec(viewport()->mapToGlobal(pos));
}

void PlainTextEdit::slotShowNewSampleWindow() {
    newSampleWindow = new NewSampleWindow(this);
    newSampleWindow->newUserSampleWindow();
    connect(newSampleWindow, SIGNAL(sampleNameRegisterDestroyed()), this, SLOT(slotCreateSample()));
}

void PlainTextEdit::slotCreateSample() {
    const QString content = textCursor().selectedText();
    const QString sampleName = newSampleWindow->newSampleName;
    if (content.isEmpty() || sampleName.isEmpty()) {
        showMessage("Empty sammple");
        return;
    }
    education->addUserSample(content, sampleName);
}

void PlainTextEdit::slotToggleBreakPoint() {
    breakPointArea->containBlock(getCursorPosition().y() - 1);
}

void PlainTextEdit::formatFile() const {
    code_info->runAction(CodeInfoDock::FormatFile);
}

void PlainTextEdit::slotGenerate() const {
    code_info->runAction(CodeInfoDock::Generate);
}

void PlainTextEdit::slotGoToDefinition() const {
    code_info->runAction(CodeInfoDock::GoToDefinition);
}

void PlainTextEdit::slotFindReferences() const {
    code_info->runAction(CodeInfoDock::FindReferences);
}

void PlainTextEdit::slotShowCommentTags() const {
    tagReminder->fillView(filepath);
    tagReminder->show();
}

void PlainTextEdit::slotShowInNodeView() {
    nodeView->showNodeFromBlock(getCursorPosition());
    // CHECK: this should set also to right node
}

void PlainTextEdit::slotAddCommentTags() {
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    const QString text = cursor.selectedText();
    /*
    for(const auto& c : text) {
        if(c != " " || c != "\t") {
            showMessage("Line under cursor contains text");
            return;
        }
    }
    */
    if (!text.simplified().isEmpty()) {
        showMessage("Line under cursor contains text");
        return;
    }
    // const QPoint pos = this->cursor().pos();
    tagListPopup->setGeometry(cursorRect());
    tagListPopup->showPopup();
    tagListPopup->setFocus();
}

void PlainTextEdit::slotExpand() {
    indentText(true);
}

void PlainTextEdit::slotCollapse() {
    indentText(false);
}

// increase size when number are 10, 100, 1000, ...
void PlainTextEdit::slotBlockCountChanged(const int &count) {
    Q_UNUSED(count)
    // BreakpointArea->sizeHint().width() + codeNotifiArea->sizeHint().width() +
    // lineNumberArea->sizeHint().width() + arrowArea->sizeHint().width()

    // expanding to right
    setViewportMargins(
                /* left */
                breakPointArea->sizeHint().width() + codeNotifyArea->sizeHint().width() +
                lineNumberArea->sizeHint().width() + arrowArea->sizeHint().width(),
                /* top */
                statusArea->sizeHint().height(), 0, 0);
}

void PlainTextEdit::slotHighlightCurrentLine()
{
    extra_selections_current_line.clear();
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        static QColor highlight = palette().color(QPalette::Text);
        highlight.setAlpha(25);
        selection.format.setBackground(highlight);
        selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extra_selections_current_line.append(selection);
    }
    // there bc. is it connected also to cursorPositionChanged signal + called in slotTextChanged
    updateExtraSelections();

    // line column widget update
    const QPoint pos = getCursorPosition();
    if(lineButton)
        lineButton->setText(QString::number(pos.y()) + ":" + QString::number(pos.x()));


    //  ++++ cursor is changing...

    if (doNotSetSelections) {
        doNotSetSelections = false;
        return;
    }
    /*
    if(touchTimer->isActive()){
        touchTimer->stop();
        touchTimer->start();
    }
    */
    //QTimer::singleShot(3000, this, &PlainTextEdit::searchByMouseTouch);
    // code_info->updateDocks();
}

// update automatically whole view -> all widgets contained
void PlainTextEdit::slotUpdateRequest(const QRect &rect, const int column) {
    if (column) {
        lineNumberArea->scroll(0, column);
        breakPointArea->scroll(0, column);
        arrowArea->scroll(0, column);
        codeNotifyArea->scroll(0, column);
    }
    //lineNumberArea->update(0, rect.y() + statusArea->sizeHint().height(), lineNumberArea->width(), rect.height());
    //BreakpointArea->update(0, rect.y(), BreakpointArea->width(), rect.height());
    lineNumberArea->update();
    breakPointArea->update();
    arrowArea->update();
    statusArea->update();
    codeNotifyArea->update();

    if (rect.contains(viewport()->rect())) {
        slotBlockCountChanged(0);
    }
}

void PlainTextEdit::slotTextChanged()
{
    slotHighlightCurrentLine();
    slotBlockCountChanged(0);

    // cursor is moving, but no touch
    if(!extra_selections_search_touched_results.empty()){
        extra_selections_search_touched_results.clear();
        updateExtraSelections();
    }
}

void PlainTextEdit::searchByMouseTouch() {
    qDebug() << "activated with " + wordUnderCursor;
    QTextCursor cursor = textCursor();
    wordUnderCursor = getWordUnderCursor();
    if (wordUnderCursor != tempWordUnderCursor && wordUnderCursor != "") {
        extra_selections_search_touched_results.clear();
        // update them ??? this will not remove them and they are repainting again !!!

        if (wordUnderCursor != "" && wordUnderCursor != ";" && wordUnderCursor != "/" && wordUnderCursor != "\\" &&
            wordUnderCursor != ":" && wordUnderCursor != "::" && wordUnderCursor != "\'" && wordUnderCursor != "\"") {
            //extra_selections_search_touched_results.setSharable(true);
            setCursorPosition(cursor, 1, 1);

            QColor highlight = Qt::darkGreen;
            highlight.setAlpha(25);
            // results do not contain search text, it is simply got where it is used and parsed as argument
            while (find(cursor, wordUnderCursor, QTextDocument::FindWholeWords)) {
                QTextEdit::ExtraSelection selection;

                selection.cursor = cursor;
                selection.format.setBackground(highlight);
                //selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
                extra_selections_search_touched_results.append(selection);
                selection.cursor.clearSelection();
            }
            tempWordUnderCursor = wordUnderCursor;
            updateExtraSelections();
        }
            // pairs
        else if(wordUnderCursor != ""){
            searchPairsSelections(cursor, wordUnderCursor);
            wordUnderCursor = tempWordUnderCursor;
            updateExtraSelections();
        }
    }
    updateExtraSelections();
}


/* PlainTextEdit protected functions
------------------------------------------------------------------------- */

void PlainTextEdit::dropEvent(QDropEvent *e){
    e->acceptProposedAction();
    QPlainTextEdit::dropEvent(e);
}

void PlainTextEdit::mousePressEvent(QMouseEvent *event) {
    anchor = anchorAt(event->pos());
    if (!anchor.isEmpty())
        QApplication::setOverrideCursor(Qt::PointingHandCursor);

    if (/*event->type() == QEvent::MouseButtonPress && event->button() == Qt::LeftButton &&*/
            event->modifiers() == Qt::ControlModifier) {
        // QApplication::setOverrideCursor(Qt::PointingHandCursor);
        // go to definition declaration request
        //qDebug() << "activated with " + wordUnderCursor;
        //searchByMouseTouch();
        // code_info->runAction(CodeInfoDock::GoToDefinition);
    }
    QPlainTextEdit::mousePressEvent(event);
}

void PlainTextEdit::mouseReleaseEvent(QMouseEvent *event) {
    if (!anchor.isEmpty()) {
        // this is for web
        //QDesktopServices::openUrl(QUrl(anchor));
        // collapsed text
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::WordUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText(anchor);

        QApplication::setOverrideCursor(Qt::ArrowCursor);
        anchor.clear();
        // handle arrowArea to paint propertly
        arrowArea->expanded.erase(
                std::find(arrowArea->expanded.begin(), arrowArea->expanded.end(), cursor.blockNumber()));
        setTextCursor(cursor);
    }
    QPlainTextEdit::mouseReleaseEvent(event);
}

void PlainTextEdit::mouseMoveEvent(QMouseEvent *event) {

    // TODO: check rect position if we are not in hoverInfoWidget if later
    // TODO: we will want to add some more functionalities or for now add
    // TODO: some ms before hiding.
    if (hoverInfo->isVisible()) {
        hoverInfo->hide();
        spellCheckPopup->hide();
        smallEdit->hide();
        tagListPopup->hide();
    }
    QPlainTextEdit::mouseMoveEvent(event);
}

// increasing, decreasing text point size
void PlainTextEdit::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const int delta = event->delta();
        if (delta > 0) {
            zoomIn();
        } else if (delta < 0) {
            zoomOut();
        }
    } else {
        QPlainTextEdit::wheelEvent(event);
    }
}

void PlainTextEdit::paintEvent(QPaintEvent *event) {
    // write content first as it was
    QPlainTextEdit::paintEvent(event);

    QPainter painter(viewport());
    // symbolic line that represents width of 1 row --> 80 characters
    const int offset = static_cast<int>((fontMetrics().width('8') * 80)
                                        + contentOffset().x()
                                        + document()->documentMargin());
    QPen pen = painter.pen();
    static QColor endOfLineColor = palette().color(QPalette::Text);
    endOfLineColor.setAlpha(50);
    pen.setColor(endOfLineColor);
    // line made from dots
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawLine(offset, 0, offset, viewport()->height());

    // DashLine
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    painter.setPen(pen);

    // 4 spaces indent in code       + contentOffset().x() + document()->documentMargin()
    // TODO: append closer to text somehow
    const int indent = (4 * fontMetrics().width('8'));  // + 1 to fit

    QTextCursor cursor = textCursor();
    QTextBlock block = firstVisibleBlockProxy();
    int i = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometryProxy(block).translated(contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(blockBoundingRectProxy(block).height());
    const QRect full = event->rect();
    while (block.isValid() && (top <= full.bottom())) {
        if (block.isVisible() && (bottom >= full.top())) {
            // each indent for 1 line
            setCursorPosition(cursor, 1, i + 1);
            const QString lineContent = getLineUnderCursor(cursor);
            const int times = countSpaceTabs(lineContent, 4);
            // skip if there is only one indent (it is done similarly everywhere)
            if(times == 1)
                goto skip;
            // TODO: skip also last one, when is draw new the text (times - 1)
            for(int z = 1; z <= times; z++) {
                painter.drawLine(indent * z, top, indent * z, bottom);
            }
        }
        skip:
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(blockBoundingRectProxy(block).height()));
        ++i;
    }
}

void PlainTextEdit::resizeEvent(QResizeEvent *event) {
    // whole text edit rect
    const QRect rect = contentsRect();
    // LineArea->setGeometry(QRect(rect.left(), rect.top(), LineArea->sizeHint().width(), rect.height()));
    //BreakpointArea->setGeometry(QRect(rect.left(), rect.top(), BreakpointArea->sizeHint().width(), rect.height()));
    // BreakPoint width is fixed, since it is an image --> but still i can change for another BreakPoint
    breakPointArea->resize(breakPointArea->sizeHint().width(), rect.height());
    codeNotifyArea->resize(codeNotifyArea->sizeHint().width(), rect.height());

    // lines are changing width so we need to update geometry for right widgets
    const int linexWidth = breakPointArea->sizeHint().width() + codeNotifyArea->sizeHint().width();
    lineNumberArea->setGeometry(linexWidth, 0, lineNumberArea->sizeHint().width(), height());
    lineNumberArea->resize(lineNumberArea->sizeHint().width(), rect.height());

    const int arrowxWidth = breakPointArea->sizeHint().width() + codeNotifyArea->sizeHint().width() +
                            lineNumberArea->sizeHint().width();
    arrowArea->setGeometry(arrowxWidth, 0, arrowArea->sizeHint().width(), height());
    arrowArea->resize(arrowArea->sizeHint().width(), rect.height());

    QPlainTextEdit::resizeEvent(event);
}

void PlainTextEdit::keyReleaseEvent(QKeyEvent *event) {
    QTextCursor cursor = textCursor();

    /*
    if (event->modifiers() == Qt::ControlModifier) {
        this->cursor().setShape(Qt::PointingHandCursor);
    }

    const bool ctrlOrShift = event->modifiers() && (Qt::ControlModifier | Qt::ShiftModifier);
    if (!completer || (ctrlOrShift && event->text().isEmpty())) {
        return;
    }
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;

    bool isShortcut = ((event->modifiers() && Qt::ControlModifier) && event->key() == Qt::Key_Space);
    if (!completer || !isShortcut) // do not process the shortcut when we have a completer
    {
        QPlainTextEdit::keyReleaseEvent(event);
    }
    if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 3
                        || eow.contains(event->text().right(1)))) {
        completer->popup()->hide();
        return;
    }
    */

    // select and copy whole line if selection empty
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C && cursor.selectedText().isEmpty()) {
        selectLineUnderCursor();
        copy();
    }
    // remove whole line if selection empty
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_X && cursor.selectedText().isEmpty()) {
        deleteLine(cursor);
    }

    switch (event->key()) {
        case Qt::Key_Backtab:
        case Qt::Key_Tab: {
            const bool forward = !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
            if (indentText(forward)) {
                event->accept();
                return;
            } else if (forward) {
                const QString text = TABS_TO_SPACES ? QString(TAB_STOP_WIDTH, ' ') : QChar('\t');
                //QTextCursor cursor = textCursor();
                cursor.insertText(text);
                setTextCursor(cursor);
                event->accept();
                return;
            }
            break;
            }
        case Qt::Key_Down:
        case Qt::Key_Up:
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                if (event->key() == Qt::Key_Down) {
                    verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
                } else {
                    verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
                }
                event->accept();
            }
            break;
        case Qt::Key_Escape:
            if (cursor.hasSelection()) {
                cursor.clearSelection();
                setTextCursor(cursor);
                event->accept();
            }
            break;
        case Qt::Key_Home:
        case Qt::Key_End:
            if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                moveCursor(event->key() != Qt::Key_Home);
                event->accept();
            }
            break; // return
        case Qt::Key_PageDown:
        case Qt::Key_PageUp:
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                if (event->key() == Qt::Key_Down) {
                    verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
                } else {
                    verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
                }
                event->accept();
            }
            break;
            // automatic type braces
        case Qt::Key_BracketLeft:
            if (autoTextSurrounding(cursor, "[")) {
                return;
            }
            cursor.insertText("]");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;
        case Qt::Key_BraceLeft:
            if (autoTextSurrounding(cursor, "{")) {
                return;
            }
            cursor.insertText("}");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;
        case Qt::Key_ParenLeft:
            if (autoTextSurrounding(cursor, "(")) {
                return;
            }
            cursor.insertText(")");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;
        case Qt::Key_Less:
            if (autoTextSurrounding(cursor, "<")) {
                return;
            }
            cursor.insertText(">");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;
        case Qt::Key_QuoteLeft:
            if (autoTextSurrounding(cursor, "'")) {
                return;
            }
            cursor.insertText("'");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;
        case Qt::Key_QuoteDbl:
            if (autoTextSurrounding(cursor, "\"")) {
                return;
            }
            cursor.insertText("\"");
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
            break;

        default:
            break;
    }
    QPlainTextEdit::keyReleaseEvent(event);
}

void PlainTextEdit::keyPressEvent(QKeyEvent *event) {

    // setToolTip(getWordUnderCursor());
    const QString completionPrefix = getWordUnderCursor();

    if (completer->popup()->isVisible()) {
        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                event->ignore();
                return;
            default:
                break;
        }
        if (!completionPrefix.isEmpty()) {
            // update completer data when typing
            completer->setCompletionPrefix(completionPrefix);
            // also elements count changes --> resize
            //QRect cr = cursorRect();
            //cr.setWidth(
            //        50 + completer->popup()->sizeHintForColumn(0) + completer->popup()->verticalScrollBar()->sizeHint().width());
            // completer->complete(cr);
            //completer->popup()->setGeometry(cr);
        }
    }

    const bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space);
    if (!isShortcut)
        QPlainTextEdit::keyPressEvent(event);

    static const QString endOfWordCharacters("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    const bool hasModifier = (event->modifiers() != Qt::NoModifier);


    if (!isShortcut && (hasModifier || event->text().isEmpty() /*|| completionPrefix.length() < 3*/ ||
                        endOfWordCharacters.contains(event->text().right(1)))) {
        completer->popup()->hide();
        //return;     // FIXME: this cause other things to ignore
    }

    if (completionPrefix != completer->completionPrefix() && isShortcut) {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));

        QRect cr = cursorRect();
        cr.setWidth(
                50 + completer->popup()->sizeHintForColumn(0) +
                completer->popup()->verticalScrollBar()->sizeHint().width());
        // set completion data by running action which will set them after all
        //code_info->runAction(CodeInfoDock::CodeComplete);
        // this is async action so completer will be filled with new data after thread ends, clear it now
        //completer->setItems(std::vector<std::string>());
        // FIXME: when clear cliked, wont show up.
        completer->complete(cr);
    }

    switch (event->key()){
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if(autoEnterTextIndentation()){
                return;
            }

            break;
        case Qt::Key_Backspace:
            autoBlankLineDeletion();
            break;
    }

    // QPlainTextEdit::keyPressEvent(event);
}


void PlainTextEdit::focusInEvent(QFocusEvent *e) {
    /*
    if (completer) {
        completer->setWidget(this);
        completer->popup()->setFocus();
        completer->popup()->installEventFilter(this);
    }
    */

    completer->setWidget(this);

    QPlainTextEdit::focusInEvent(e);
}

// cursor continues to be shown when menu pop up
// or simply when it pop up this->setFocus() to menu
void PlainTextEdit::focusOutEvent(QFocusEvent *e) {
    const Qt::FocusReason r = e->reason();
    if ((r == Qt::PopupFocusReason) ||
        (r == Qt::MenuBarFocusReason)) {
    } else {
        QPlainTextEdit::focusOutEvent(e);
    }
}

// Hover Information label
bool PlainTextEdit::event(QEvent *event) {
    // TODO: for this purpose try to get all data for text positions
    // TODO: and if we will going to show, can avoid action thread run (run it just once for all file)
    if (event->type() == QEvent::ToolTip) {
        setToolTip(QString()); // clear
        const auto *helpEvent = static_cast<QHelpEvent *>(event);
        QTextCursor cursor = cursorForPosition(helpEvent->pos());

        // collapsed text set as default QToolTip
        const QString collapsedText = anchorAt(helpEvent->pos());
        if (!collapsedText.isEmpty()) {
            // smallEdit shows collapsed scope
            smallEdit->setPlainText(collapsedText);
            smallEdit->setGeometry(cursorRect());
            // TODO: height: set sext, get num lines compute height ... numlines*lineheight
            smallEdit->setMinimumSize(fontMetrics().boundingRect(collapsedText).width(), 125);
            smallEdit->show();
            // setToolTip(collapsedText);
            goto end;
        }

        // preformance: match only line
        const int line = cursor.blockNumber();
        for (const auto &suggestion : code_info->spellCheckWorker->suggestions) {
            if (suggestion.startPosition.y() == line) {
                spellCheckPopup->setItemList(suggestion.suggestions);
                spellCheckPopup->setGeometry(cursorRect());
                spellCheckPopup->show();
                spellCheckPopup->setFocus();
                goto end;
                //break;
            }
        }

        // QCursor cur = this->cursor();
        cursor.select(QTextCursor::WordUnderCursor);
        if (!cursor.selectedText().isEmpty()) {
            const QString label =
                    "<a style=text-decoration:none;color:lightblue; href=/home/adam/Desktop/Header_Calls_Profiler/options.cpp:3:3 >" +
                    cursor.selectedText() + "</a>";
            hoverInfo->setLabelText(label);
            //hoverInfo->setMinimumSize(fontMetrics().boundingRect(cursor.selectedText()).width() + 15, fontMetrics().boundingRect(cursor.selectedText()).height() + 15);
            hoverInfo->setGeometry(cursorRect());
            // rect()   -> top left corner
            hoverInfo->show();
            // FIXME: this is done async. set also rectangle to track if we still want to show it
            // code_info->runAction(CodeInfoDock::HoverInfo);
            // QWhatsThis::showText(QPoint(cursorRect().x(), cursorRect().y()), "mytext", hoverInfo);
            // QToolTip::showText(helpEvent->globalPos(), cursor.selectedText(), hoverInfo);
            // TODO: implement this as postition we are in ... function, altough we have codeInfoDock for that
            // QWhatsThis::add(sourceLineEdit, ....
            // ------------- good but very bad ----------------------
            // QWhatsThis::showText(helpEvent->globalPos(), cursor.selectedText(), hoverInfo);
            // QStatusTipEvent(cursor.selectedText());
            setStatusTip(cursor.selectedText());
        }
        //else
        //QToolTip::hideText();
        //return true;
    }
    /*
    if(event->type() == QEvent::Leave) {
        qDebug() << "mouse leave";
        hoverInfo->hide();
    }
    */
    end:
    return QPlainTextEdit::event(event);
}


/* LineNumberArea widget
------------------------------------------------------------------------- */

LineNumberArea::LineNumberArea(PlainTextEdit *edit, QWidget *parent) : QWidget(edit), m_Edit(edit) {
    setStyleSheet("background-color: rgb(47, 47, 47)");
    setUpdatesEnabled(true);

    const int xWidth = m_Edit->breakPointArea->sizeHint().width() + m_Edit->codeNotifyArea->sizeHint().width();
    setGeometry(xWidth, m_Edit->statusArea->sizeHint().height(), sizeHint().width(), m_Edit->height());
}

void LineNumberArea::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    QMouseEvent copy(QEvent::MouseMove, QPoint(-1, -1), Qt::NoButton, nullptr, nullptr);
    mouseEvent(&copy);
}

void LineNumberArea::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QTextBlock block = m_Edit->firstVisibleBlockProxy();
    int i = block.blockNumber();
    // add top are to push down left side widgets
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(m_Edit->contentOffsetProxy()).top() + m_Edit->statusArea->sizeHint().height());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    const QRect full = e->rect();
    painter.fillRect(full, palette().color(QPalette::Base));
    while (block.isValid() && (top <= full.bottom())) {
        if (block.isVisible() && (bottom >= full.top())) {
            // !!! since BreakPoint is on left, i need to move first x pos, or on left -> change break area x pos
            const QRect box(0, top, width(), m_Edit->fontMetrics().height());
            QFont font = painter.font();
            font.setFamily(m_Edit->font().family());
            font.setPointSize(m_Edit->font().pointSize());
            if (m_Edit->textCursor().blockNumber() == i) {
                // painter.fillRect(box, palette().color(QPalette::Highlight));
                painter.setPen(palette().color(QPalette::HighlightedText));
                font.setWeight(QFont::Bold);
            } else {
                font.setWeight(QFont::Normal);
                painter.setPen(palette().color(QPalette::Text));
            }
            painter.setFont(font);
            painter.drawText(box.left(), box.top(), box.width(), box.height(), Qt::AlignCenter,
                             QString::number(i + 1).append(' '));
            // painter.setPen(palette().color(QPalette::Highlight));
            // painter.drawLine(full.topRight(), full.bottomRight());
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
        ++i;
    }
    QWidget::paintEvent(e);
}


/* protected functions
------------------------------------------------------------------------- */

// line number was clicked and whole line was selected
void LineNumberArea::mouseEvent(QMouseEvent *e)
{
    QTextCursor cursor = m_Edit->cursorForPosition(QPoint(0, e->pos().y()));
    if ((e->type() == QEvent::MouseButtonPress) && (e->button() == Qt::LeftButton)) {
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        cursor.setVisualNavigation(true);
        m_Edit->setTextCursor(cursor);
    }
}

void LineNumberArea::mouseMoveEvent(QMouseEvent *event)
{
    mouseEvent(event);
}
void LineNumberArea::mousePressEvent(QMouseEvent *event)
{
    mouseEvent(event);
}
void LineNumberArea::mouseReleaseEvent(QMouseEvent *event)
{
    mouseEvent(event);
}

QSize LineNumberArea::sizeHint() const {
    int digits = 1;
    int blocks = qMax(1, m_Edit->blockCount());
    while (blocks >= 10) {
        blocks /= 10;
        digits++;
    }
    return QSize(((m_Edit->fontMetrics().width('8') * digits) + 4), 0);  // shrink to fit only number
}

void LineNumberArea::wheelEvent(QWheelEvent *e) {
    QApplication::sendEvent(m_Edit->viewport(), e);
}

/* BreakPointArea widget
------------------------------------------------------------------------- */

BreakPointArea::BreakPointArea(PlainTextEdit *edit, QWidget *parent) : QWidget(edit), m_Edit(edit) {
    setUpdatesEnabled(true);
    breakpoint.load(IconFactory::BreakPoint);
    // setFixedWidth(breakpoint.width());
    B_blocks.reserve(5);
    setGeometry(0, m_Edit->statusArea->sizeHint().height(), sizeHint().width(), m_Edit->height());
}

bool BreakPointArea::containBlock(const int& line) {
    //qDebug() << "added";
    //qDebug() << line;
    for (unsigned int i = 0; i < B_blocks.size(); i++) {
        // already contained -> remove   -> will not be painted
        if (B_blocks[i] == line) {
            //qDebug() << "erasing";
            //qDebug() << line;
            B_blocks.erase(B_blocks.begin() + i);  // only block does not work
            breakPointRemoved(line);
            return true;
        }
    }
    // not contained
    B_blocks.push_back(line);
    emit breakPointCreated(line);
    return false;
}

bool BreakPointArea::canCreateBreakPoint(const QTextBlock &block) {
    for (const int &B_block : B_blocks) {
        if (B_block == block.blockNumber()) {
            return true;
        }
    }
    // cannot create
    return false;
}

QSize BreakPointArea::sizeHint() const {
    return QSize((breakpoint.width()), 0);
}

bool BreakPointArea::event(QEvent *event) {
    if (event->type() == QEvent::ToolTip) {
        const auto *helpEvent = static_cast<QHelpEvent *>(event);
        QTextCursor cursor = m_Edit->cursorForPosition(helpEvent->pos());
        const int line = cursor.blockNumber();
        for (const auto &BP : B_blocks) {
            if (line == BP) {
                setToolTip(m_Edit->getFilePath() + "\n" +
                           "line: " + QString::number(BP + 1) + "\n" +
                           "active");
                goto end;
            }
        }
        setToolTip(QString());
    }
    end:;
    return QWidget::event(event);
}

void BreakPointArea::mousePressEvent(QMouseEvent *event) {
    if ((event->type() == QEvent::MouseButtonPress) && (event->button() == Qt::LeftButton)) {
        QTextCursor cursor = m_Edit->cursorForPosition(QPoint(0, event->pos().y()));
        const QTextBlock touched_block = cursor.block();
        // cursor.setVisualNavigation(true);
        // can be called only here, before push
        containBlock(touched_block.blockNumber());
        m_Edit->setTextCursor(cursor);
        //emit breakPointCreated();
        //qDebug() << touched_block.blockNumber();
        //qDebug() << blocks;
    }
    QWidget::mouseReleaseEvent(event);
}

void BreakPointArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QTextBlock block = m_Edit->firstVisibleBlockProxy();
    // y1, y2 coordinates of rectangle we are going to paint in: top, top + height of 1 line(block)
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(m_Edit->contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    // widget area rectangle
    const QRect area_rect = event->rect();
    // while not all lines are painted, at the end increasing, checking if valid
    while (block.isValid() && (top <= area_rect.bottom())) {
        if (block.isVisible() && (bottom >= area_rect.top())) {
            // width is fixed in constructor, height -> i want to fill by font size, since it can increase
            // TODO: first x is problem, since line numbers are also like that painted
            if(canCreateBreakPoint(block)){
                // was 0
                const QRect box(0, top, width(), m_Edit->fontMetrics().height());
                painter.drawPixmap(box, breakpoint);
            }
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
    }
    QWidget::paintEvent(event);
}


ArrowArea::ArrowArea(PlainTextEdit *edit, QWidget *parent) : QWidget(edit), m_Edit(edit) {
    setUpdatesEnabled(true);
    arrowCollapse.load(IconFactory::ScrollUp);
    arrowExpand.load(IconFactory::ScrollDown);
    setFixedWidth(arrowCollapse.width());
    setToolTip("arrow area");

    viewMenu = new QMenu(this);

    viewMenu->addAction("Expand All Scopes", this, SLOT(slotExpandAllScopes()));
    viewMenu->addAction("Collapse All Scopes", this, SLOT(slotCollapseAllScopes()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotShowMenu(const QPoint&)));


    const int xWidth = m_Edit->breakPointArea->sizeHint().width() + m_Edit->codeNotifyArea->sizeHint().width() +
                       m_Edit->lineNumberArea->sizeHint().width();
    setGeometry(xWidth, m_Edit->statusArea->sizeHint().height(), sizeHint().width(), m_Edit->height());
}

void ArrowArea::slotShowMenu(const QPoint &pos) {
    viewMenu->exec(mapToGlobal(pos));
}

void ArrowArea::slotExpandAllScopes() {

}

void ArrowArea::slotCollapseAllScopes() {

}

void ArrowArea::collapseOrExpand(const int &line) {
    if (isExpanded(line)) {
        // collapse it
        expanded.erase(std::find(expanded.begin(), expanded.end(), line));
        // match line and get point
        // m_Edit->code_info->collapsableBoundariesRanges...
        const QPoint start(0, 0);
        const QPoint end(0, 0);
        m_Edit->setCollapsableText(start, end);
    } else {
        // expand it
        expanded.push_back(line);
        QTextCursor cursor = m_Edit->textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.select(QTextCursor::WordUnderCursor);
        // will this be a html ?
        const QString html = cursor.selectedText();
        const QString replacement = html.mid(html.indexOf("href="), html.indexOf(">"));
        cursor.insertText(replacement);
        m_Edit->setTextCursor(cursor);
    }
}

bool ArrowArea::isExpanded(const int &line) {
    for (const int &l : expanded) {
        if (l == line) {
            return true;
        }
    }
    return false;
}

bool ArrowArea::isMarked(const int &line) {
    for (const int &a : arrows) {
        if (a == line) {
            return true;
        }
    }
    return false;
}

QSize ArrowArea::sizeHint() const {
    // both are same size
    return QSize((arrowCollapse.width()), 0);
}

void ArrowArea::mousePressEvent(QMouseEvent *event) {
    if ((event->type() == QEvent::MouseButtonPress) && (event->button() == Qt::LeftButton)) {
        QTextCursor cursor = m_Edit->cursorForPosition(QPoint(0, event->pos().y()));
        const int line = cursor.blockNumber();
        // can be called only here, before push
        if (isMarked(line)) {
            collapseOrExpand(line);
        }
        // m_Edit->setTextCursor(cursor);
    }
    QWidget::mousePressEvent(event);
}

void ArrowArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QTextBlock block = m_Edit->firstVisibleBlockProxy();
    // y1, y2 coordinates of rectangle we are going to paint in: top, top + height of 1 line(block)
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(
            m_Edit->contentOffsetProxy()).top() + m_Edit->statusArea->sizeHint().height());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    // widget area rectangle
    const QRect area_rect = event->rect();
    // while not all lines are painted, at the end increasing, checking if valid
    while (block.isValid() && (top <= area_rect.bottom())) {
        if (block.isVisible() && (bottom >= area_rect.top())) {
            // width is fixed in constructor, height -> i want to fill by font size, since it can increase
            // TODO: first x is problem, since line numbers are also like that painted
            const bool expand = isExpanded(block.blockNumber());
            const bool marked = isMarked(block.blockNumber());
            if (expand) {
                // was 0
                const QRect box(0, top, width(), m_Edit->fontMetrics().height());
                painter.drawPixmap(box, arrowExpand);
            }
            if (!expand && marked) {
                const QRect box(0, top, width(), m_Edit->fontMetrics().height());
                painter.drawPixmap(box, arrowCollapse);
            }
            // else draw nothing
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
    }
    QWidget::paintEvent(event);
}


CodeNotifyArea::CodeNotifyArea(PlainTextEdit *edit, QWidget *parent) : QWidget(edit), m_Edit(edit) {
    WH.setX(m_Edit->fontMetrics().horizontalAdvance("88"));
    WH.setY(m_Edit->fontMetrics().height());

    // next to breakpoints
    setGeometry(m_Edit->breakPointArea->sizeHint().width(), m_Edit->statusArea->sizeHint().height(), WH.x(), m_Edit->height());  // instead of paint event

    error.load(IconFactory::Error);
    warning.load(IconFactory::Warning);
    // TODO: change this, find proper icons
    fixit.load(IconFactory::Warning);
    color.load(IconFactory::Color);

    setUpdatesEnabled(true);

    addNotification("notify widget", 1, Error);
    addNotification("notify widget 2", 2, Warning);
    addNotification("notify widget 3", 3, Fixit);
    addNotification("notify widget 4", 4, Color);
}

void
CodeNotifyArea::addNotification(const QString &toolTip, const int &line, const CodeNotifyArea::Notification &type) {
    notifier notify;
    notify.line = line;
    notify.tooltip = toolTip;
    notify.notify = type;
    switch (type) {
        case Error:
            notify.image = error;
            break;
        case Warning:
            notify.image = warning;
            break;
        case Fixit:
            notify.image = fixit;
            break;
        case Color:
            notify.image = color;
            break;
    }
    notifiers.push_back(notify);
}

void CodeNotifyArea::removeNotification(const int &line) {
    for (unsigned int i = 0; i < notifiers.size(); i++) {
        if (notifiers[i].line == line) {
            notifiers.erase(notifiers.begin() + i);
        }
    }
}

bool CodeNotifyArea::canDrawImage(const int &line) {
    if (notifiers.empty()) {
        return false;
    }
    for (const auto &li : notifiers) {
        if (li.line == line) {
            return true;
        }
    }
    return false;
}

QPixmap CodeNotifyArea::whatToDraw(const int &line) {
    for (const auto &im : notifiers) {
        if (im.line == line) {
            return im.image;
        }
    }
    // never reached
    return QPixmap();
}

QSize CodeNotifyArea::sizeHint() const {
    return QSize(WH.x(), 0);
}

void CodeNotifyArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QTextBlock block = m_Edit->firstVisibleBlockProxy();
    // y1, y2 coordinates of rectangle we are going to paint in: top, top + height of 1 line(block)
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(
            m_Edit->contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    // widget area rectangle
    const QRect area_rect = event->rect();
    // while not all lines are painted, at the end increasing, checking if valid
    while (block.isValid() && (top <= area_rect.bottom())) {
        if (block.isVisible() && (bottom >= area_rect.top())) {
            // width is fixed in constructor, height -> i want to fill by font size, since it can increase
            // TODO: first x is problem, since line numbers are also like that painted
            if (canDrawImage(block.blockNumber())) {
                const QRect box(0, top, width(), m_Edit->fontMetrics().height());
                painter.drawPixmap(box, whatToDraw(block.blockNumber()));
            }
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
    }
    QWidget::paintEvent(event);
}

bool CodeNotifyArea::event(QEvent *event) {
    if (event->type() == QEvent::ToolTip) {
        const auto *helpEvent = static_cast<QHelpEvent *>(event);
        QTextCursor cursor = m_Edit->cursorForPosition(helpEvent->pos());
        const int line = cursor.blockNumber();
        for (const auto &tip : notifiers) {
            if (tip.line == line) {
                setToolTip(tip.tooltip);
                qDebug() << "shown";
                goto end;
            }
        }
        setToolTip(QString());
    }
    end:;
    return QWidget::event(event);
}

void CodeNotifyArea::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}


// https://stackoverflow.com/questions/22606122/draw-text-on-scrollbar
ScrollBar::ScrollBar(PlainTextEdit *edit, QWidget *parent) : QScrollBar(edit), m_Edit(edit) {
    setOrientation(Qt::Vertical);
    setMouseTracking(true);
    smallEdit = new SmallRoundedEdit(m_Edit);
    QFont f;
    f.setPixelSize(15);
    smallEdit->setFont(f);
    // pointer ---> changes possible reflects to real editor
    smallEdit->setDocument(m_Edit->document());
    smallEdit->verticalScrollBar()->setVisible(false);
}

void ScrollBar::paintEvent(QPaintEvent *event) {
    // let paint slider as default and later draw on it what we want.
    QScrollBar::paintEvent(event);

    QPainter painter(this);

    // painting line width calculated for line
    const int widthPerLine = height() / m_Edit->blockCount();

    // this->height();
    QPen pen;
    pen.setWidth(widthPerLine);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    // painting all of them
    for (const auto &spellList : m_Edit->code_info->spellCheckWorker->suggestions) {
        const float H = computeHeightForLine(spellList.startPosition.y());
        pen.setColor(Qt::green);
        const QLineF LINE(QPointF(0, H), QPointF(width(), H));
        painter.drawLine(LINE);
    }
    // search results
    for (const auto &search : m_Edit->search_results) {
        const float H = computeHeightForLine(search.y());
        pen.setColor(Qt::darkBlue);
        const QLineF LINE(QPointF(0, H), QPointF(width(), H));
        painter.drawLine(LINE);
    }
    // comment tags
    /*
    for(int i = 0; i < m_Edit->tagReminder->tagData.size(); i++) {
        if(m_Edit->tagReminder->sources[i] == m_Edit->getFilePath()) {
            for(const auto& elem : m_Edit->tagReminder->tagData[i]) {
                const int H = computeHeightForLine(elem.line);
                pen.setColor(Qt::darkYellow);
                const QLineF LINE(QPointF(0, H), QPointF(width(), H));
                painter.drawLine(LINE);
            }
        }
    }

    // clang diagnostics, warnings, errors
    for(const auto& diag : m_Edit->clang->PData->Diags) {
        // TODO: fixes are there, but does everyone has fixes ? and diag.Source
        const int H = computeHeightForLine(diag.Range.start.line);
        const QLineF LINE(QPointF(0, H), QPointF(width(), H));
        switch (diag.Severity) {
            case clang::DiagnosticsEngine::Level::Warning:
                pen.setColor(Qt::yellow);
                break;
            case clang::DiagnosticsEngine::Level::Error:
                pen.setColor(Qt::red);
                break;

            default:
                break;
        }
        painter.drawLine(LINE);
    }
    */
    // breakPoint
    for (const auto &line : m_Edit->breakPointArea->B_blocks) {
        const float H = computeHeightForLine(line);
        const QLineF LINE(QPointF(0, H), QPointF(width(), H));
        pen.setColor(Qt::darkRed);
        painter.drawLine(LINE);
    }
}

void ScrollBar::mousePressEvent(QMouseEvent *event) {
    const int H = pos().y();
    const int widthPerLine = height() / m_Edit->blockCount();
    const int betweenLinePos = widthPerLine * H;

    m_Edit->scroll(0, H);
    QScrollBar::mousePressEvent(event);
}

void ScrollBar::mouseDoubleClickEvent(QMouseEvent *event) {


    QScrollBar::mouseDoubleClickEvent(event);
}

void ScrollBar::leaveEvent(QEvent *event) {
    // we are in smallEdit
    inWidget = false;
    if (!smallEdit->inWidget) {
        smallEdit->hide();
    }
    QScrollBar::leaveEvent(event);
}

void ScrollBar::enterEvent(QEvent *event) {
    // check if cursor moved
    inWidget = true;
    QTimer::singleShot(2000, this, [=]() {
        if (inWidget) {
            smallEdit->show();
            smallEdit->setFocus();
        }
    });
    QScrollBar::enterEvent(event);
}

void ScrollBar::mouseMoveEvent(QMouseEvent *event) {
    calculateAndSetSmallEditGeometry(event->pos().y());
    QScrollBar::mouseMoveEvent(event);
}

float ScrollBar::computeHeightForLine(const int &line) {
    // NOTE! it has to fit in height() ; blockCount() is never 0

    // FIXME: this works, but the other NOT.
    if (m_Edit->blockCount() <= height()) {
        const int widthPerLine = height() / m_Edit->blockCount();
        return line * widthPerLine;
    } else {
        // we have more lines than our height()
        // we have to get <= height()
        // TODO: this down not work
        const float H = (float(height() / m_Edit->blockCount())) * height();
        return H;
    }
}

void ScrollBar::calculateAndSetSmallEditGeometry(const int &posY) {
    // middle is with cursor 400;
    HEIGHT = 300 * smallEdit->timesAplified;
    // TODO: change 200 to fit width exactly with side widgets
    smallEdit->setGeometry(200, posY - (HEIGHT / 2), m_Edit->width() - width() - (HEIGHT / 2), HEIGHT);
    const int widthPerLine = height() / m_Edit->blockCount();
    const int betweenLinePos = widthPerLine * posY;
    const int lineHeight = m_Edit->fontMetrics().height();
    /*
    if(height() < m_Edit->blockCount())
        smallEdit->scroll(0, posY);
    else
        smallEdit->scroll(0, betweenLinePos);
    */
    smallEdit->scroll(0, widthPerLine * lineHeight);
    // one line has height 15
    // const int numLinesToFit = 400 / 15;
    // by ranges
    /*
    QString content;
    QTextCursor cursor = m_Edit->textCursor();
    for(int i = 0; i <= numLinesToFit; i++) {
        content += m_Edit->getLineUnderCursor(cursor);
    }
    smallEdit->setPlainText(content);
    */
}





StatusArea::StatusArea(PlainTextEdit *edit, QWidget *parent) : QWidget(edit), m_Edit(edit) {
    createWindow();
    setFixedHeight(20);

    setStyleSheet("border: 0px; padding: 0px; margin: 0px; background-color: rgb(68, 68, 68);");

    setGeometry(0, 0, edit->width(), 25);
}

void StatusArea::createWindow() {
    MainLayout = new QHBoxLayout();
    collapseAllScopes = new QToolButton(this);
    expandAllScopes = new QToolButton(this);
    currentSymbolBox = new QComboBox(this);
    encoding = new QToolButton(this);
    lineColumn = new QToolButton(this);
    documentCode = new QToolButton(this);
    commentTagsforThisFile = new QToolButton(this);

    goToLineColumn = new GoToLineColumn(m_Edit);

    collapseAllScopes->setIcon(QIcon(IconFactory::Collapse));
    collapseAllScopes->setToolTip("Collapse All Scopes");
    expandAllScopes->setIcon(QIcon(IconFactory::Expand));
    expandAllScopes->setToolTip("Expand All Scopes");
    currentSymbolBox->setMinimumWidth(100);
    currentSymbolBox->setMaxVisibleItems(15);
    currentSymbolBox->addItems(QStringList() << "myitem" << "youritem" << "ouritem");
    encoding->setText("UTF-8");
    encoding->setFixedWidth(50);
    lineColumn->setFixedWidth(40);
    documentCode->setText("doc");
    documentCode->setFixedWidth(50);
    commentTagsforThisFile->setText("Comment Tags");
    commentTagsforThisFile->setFixedWidth(115);

    connect(lineColumn, &QAbstractButton::clicked, this, [=](){
       goToLineColumn->show();
    });
    connect(commentTagsforThisFile, &QToolButton::clicked, this, [=](){
       m_Edit->tagReminder->fillView(m_Edit->getFilePath());
       m_Edit->tagReminder->show();
    });
    connect(documentCode, &QToolButton::clicked, this, [=](){
        // TODO: with clang get right position offset and params + type(void, non-void, enum)
       m_Edit->documentationHelper->setDocData(m_Edit->getCursorPosition(), QStringList() << "mmmm" << "nnnn");
       m_Edit->documentationHelper->show();
    });

    // TODO: connnect expand, collapse with arrowArea + take care for editor hrefs for this.
    // TODO: make function for this to call.


    MainLayout->addWidget(collapseAllScopes);
    MainLayout->addWidget(expandAllScopes);
    MainLayout->addWidget(currentSymbolBox);

    MainLayout->addWidget(commentTagsforThisFile);
    MainLayout->addWidget(documentCode);
    MainLayout->addWidget(lineColumn);
    MainLayout->addWidget(encoding);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    auto *layout = new QHBoxLayout(this);
    auto *widget = new QWidget(this);

    widget->setLayout(MainLayout);

    layout->addWidget(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setStyleSheet("border-radius: 0px; border: 1px solid black; background-color: rgb(100, 100, 100);");

    setLayout(layout);

}
