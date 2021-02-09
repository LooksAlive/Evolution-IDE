#include <QApplication>
#include <QFileInfo>
#include <QPainter>
#include <QScrollBar>
#include <QSettings>
#include <QShortcut>
#include <QTextBlock>
#include <QTextStream>

#include "icons/IconFactory.h"
#include "plaintextedit.h"

const int TAB_STOP_WIDTH = 4;
const bool TABS_TO_SPACES = true;

PlainTextEdit::PlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    LineArea = new LineNumberArea(this);
    BreakpointArea = new BreakPointArea(this);

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
    setWordWrapMode(QTextOption::WordWrap);
    setReadOnly(false);

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
    SetupCompleter();
    ensureCursorVisible();
}

QRectF PlainTextEdit::blockBoundingGeometryProxy(const QTextBlock &block)
{
    return blockBoundingGeometry(block);
}

QRectF PlainTextEdit::blockBoundingRectProxy(const QTextBlock &block)
{
    return blockBoundingRect(block);
}

QPointF PlainTextEdit::contentOffsetProxy()
{
    return contentOffset();
}


QTextBlock PlainTextEdit::firstVisibleBlockProxy()
{
    return firstVisibleBlock();
}

// cursor
void PlainTextEdit::setCursorPosition(const int &row, const int &col)
{
    const QTextBlock block = document()->findBlockByLineNumber(row - 1);  // - 1
    if(block.isValid())
    {
        QTextCursor cursor = textCursor();
        cursor.setPosition(block.position() + col);  // + 1, bc. col cannot be 0
        setTextCursor(cursor);
        ensureCursorVisible();
    }
}

QPoint PlainTextEdit::getCursorPosition(){
    QTextCursor cursor = textCursor();
    // there is no 0 col or row in view but it is acts like it is
    int row = cursor.blockNumber() + 1;     // +1
    int col = cursor.columnNumber() + 1;    // +1 , starting from 1

    return QPoint(row, col);
}

void PlainTextEdit::setCursorAtLine(const int &line)
{
    setCursorPosition(line, 1);   // +1 ; yet no idea how ? jumps as it wants
}

// text manipulation
void PlainTextEdit::selectLineUnderCursor(){
    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    setTextCursor(cur);
    ensureCursorVisible();
}

QString PlainTextEdit::getLineUnderCursor(){
    selectLineUnderCursor();
    QTextCursor cur = textCursor();
    QString line = cur.selectedText();
    cur.clearSelection();
    setTextCursor(cur);
    ensureCursorVisible();
    return line;
}

QString PlainTextEdit::getLineContent(const int &row) {
    setCursorPosition(row, 1);
    return getLineUnderCursor();
}

void PlainTextEdit::selectWordUnderCursor(){
    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cur.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    setTextCursor(cur);
    ensureCursorVisible();
}

QString PlainTextEdit::getWordUnderCursor(){
    selectWordUnderCursor();
    QTextCursor cur = textCursor();
    QString word = cur.selectedText();
    cur.clearSelection();
    setTextCursor(cur);
    return word;
}

void PlainTextEdit::selectWord(const int &line, const int &column){
    QTextEdit::ExtraSelection selection;
    //selection.cursor.setPosition(document()->findBlockByNumber(line).position());
    setCursorPosition(line, column);
    selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, column);
    selectWordUnderCursor();
    search_selections.append(selection);
    setExtraSelections(search_selections);
}

void PlainTextEdit::completerInsertText(const QString &text) {
    if (completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = text.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(text.right(extra));
    setTextCursor(tc);
}

void PlainTextEdit::deleteLine(){
    selectLineUnderCursor();
    QTextCursor cur = textCursor();
    if(cur.selectedText() != ""){
        cur.removeSelectedText();
    }
    setTextCursor(cur);
}

void PlainTextEdit::toggleComment(){

    //QString file_extension;  // default is cpp
    QTextCursor cur = textCursor();
    if (!cur.hasSelection()){
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
            QString line = getLineUnderCursor();

            if(line.endsWith(commentEnd)){
                for(int i=0; i < commentEnd.length(); i++){
                    cur.deletePreviousChar();
                }
            }
        }else{
            cur.insertText(commentStart);
            endSelection += commentStart.length();

            cur.setPosition(endSelection);
            cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
            setTextCursor(cur);
            QString line = getLineUnderCursor();

            if(!line.endsWith(commentEnd)){
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
            cursor.insertText(TABS_TO_SPACES ? QString(TAB_STOP_WIDTH, ' ') + text : QChar('\t') + text);
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

void PlainTextEdit::moveCursor(const bool &end)
{
    QTextCursor cursor = textCursor();
    int length = cursor.block().text().length();
    if (length != 0) {
        int original  = cursor.position();
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

QList<QPoint> PlainTextEdit::getParenthessesPairPositions(){
    QTextCursor cursor = textCursor();

    cursor = document()->find("{", cursor);
    QPoint first(cursor.blockNumber(), cursor.columnNumber());

    cursor = document()->find("}", cursor);
    QPoint second(cursor.blockNumber(), cursor.columnNumber());

    // case nested parenthesses { { } } ; if { is before second / }
    cursor.setPosition(first.x() + 1);   // return to first to look between them
    cursor = document()->find("{", cursor);
    QPoint temp(cursor.blockNumber(), cursor.columnNumber());
    int nested = 0;
    while(true){
        nested++;
        if(temp.x() < second.x() && temp.y() < second.y()){
            // reset second and search again
            cursor = document()->find("}", cursor);
            second.setX(cursor.blockNumber());
            second.setY(cursor.columnNumber());
            cursor.setPosition(first.x() + 1);   // jump over found }
        }
        else{break;}
    }

    QList<QPoint> positions;
    positions.append(first);
    positions.append(second);

    return positions;
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
    search_selections.clear();
    search_results.clear();
    // set to start, 1 file search
    textCursor().movePosition(QTextCursor::Start);
    // entry position, since we are changing cursor position, regardless findNext also sets cursor for start
    int pos = textCursor().position();

    // results do not contain search text, it is simply got where it is used and parsed as argument
    while(find(search, find_options)){
        //QTextEdit::ExtraSelection selection;
        searchResult search_result_data;
        /*
        QColor highlight = palette().color(QPalette::Dark);
        highlight.setAlpha(25);
        selection.format.setBackground(highlight);
        selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
        //selection.cursor = textCursor();
        selection.cursor = document()->find(search, selection.cursor, find_options);
        //selection.cursor.select(QTextCursor::WordUnderCursor);
        search_selections.push_back(selection);
        selection.cursor.clearSelection();
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        */
        textCursor().movePosition(QTextCursor::NextCharacter);

        // get data
        QPoint point = getCursorPosition();
        search_result_data.fileName = file;
        search_result_data.row = point.x();
        search_result_data.col = point.y();

        // later search_results for multifile search

        search_results.push_back(search_result_data);
    }
    textCursor().setPosition(pos);
    // set selections
    /*
    if(!search_selections.isEmpty()){
        setExtraSelections(search_selections);
    }
    */
    qDebug() << search_results.size();
}

bool PlainTextEdit::find(const QString &search, const QTextDocument::FindFlags &find_options)
{
    QTextCursor cursor = textCursor();
    cursor = document()->find(search, cursor, find_options);

    if(!cursor.isNull()){
        // cursor.movePosition(QTextCursor::NextCharacter);
        setTextCursor(cursor);
        return true;
    }else{
        return false;   // no occurrences found
    }
}

void PlainTextEdit::findNext(const QString &search, const QTextDocument::FindFlags &find_options)
{
    // findStoreAndSelectAll(search, find_options); // find all results and select them first

    QTextCursor cursor = textCursor();
    // find_options |= QTextDocument::FindBackward;   // is const now -> auditing in slot in widget
    cursor = document()->find(search, cursor, find_options);

    //qDebug() << cursor.isNull();
    if(!cursor.isNull()){
        setTextCursor(cursor);
    }else{
        // try to start at start of document in case that are there some occurrences
        cursor.movePosition(QTextCursor::Start);
        cursor = document()->find(search, cursor);
        if(!cursor.isNull()){
            setTextCursor(cursor);
        }else{
            return;   // no occurrences found
        }
    }
}

void PlainTextEdit::replace(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options)
{
    QTextCursor cursor = textCursor();

    if(cursor.hasSelection() && !isReadOnly()/* && cursor.selectedText() == oldText */)
    {
        findNext(oldText, find_options);

        cursor.beginEditBlock();
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.endEditBlock();
        setTextCursor(cursor);
    }
}

void PlainTextEdit::replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options)
{
    replace(oldText, newText, find_options);
    findNext(oldText, find_options);
}

int PlainTextEdit::replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options)
{
    int count = 0;
    textCursor().movePosition(QTextCursor::Start); // on start, works yet only with one file

    while(find(oldText, find_options)){
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
    viewMenu->addAction(QIcon(IconFactory::Collapse), "Collapse", this, SLOT(collapse()));
    viewMenu->addAction(QIcon(IconFactory::Expand), "Expand", this, SLOT(expand()));
    viewMenu->addSeparator();
    viewMenu->addAction(QIcon(IconFactory::Comment), "Comment Code", this, SLOT(toggleComment()), Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    viewMenu->addAction("Generate...", this, SLOT(slotGenerate()), Qt::CTRL + Qt::SHIFT + Qt::Key_G);
    viewMenu->addAction("Format File", this, SLOT(formatFile()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);
    viewMenu->addAction("Go to Definition", this, SLOT(slotGoToDefinition()), Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    viewMenu->addAction("Find References", this, SLOT(slotFindReferences()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotShowMenu(const QPoint&)));
}

void PlainTextEdit::SetupCompleter() {
    QStringList words;
    words << "void" << "voidd" << "vaid" << "bool" << "int";
    //completer->setModel(new QStringListModel(words, completer));

    completer = new QCompleter(words, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setMaxVisibleItems(8);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setWrapAround(false);
    completer->setFilterMode(Qt::MatchContains);

    auto *view = new QListView(this);
    view->setViewMode(QListView::ListMode);
    view->setLayoutMode(QListView::Batched);
    completer->setPopup(view);
    //auto *model = new QFileSystemModel(this);
    //model->setRootPath(QDir::homePath());
    //completer->popup()->setMinimumSize(200, 100);

    connect(completer, SIGNAL(activated(const QString&)), this, SLOT(completerInsertText(const QString&)));
}

void PlainTextEdit::setCompletionData(const std::vector<std::string> &data) {
    QStringList buffer;
    for (int i = 0; i < data.size(); i++) {
        buffer.push_back(QString::fromStdString(data[i]));
    }
    completer->setModel(new QStringListModel(buffer, completer));
}

void PlainTextEdit::setFileExtension(const QString &extension){
    file_extension = extension;
}

void PlainTextEdit::setFilePath(const QString &file_path) {
    file = file_path;
}

QString PlainTextEdit::getFilePath(){
    return file;
}

int PlainTextEdit::setBreakPoint() {
    int line = getCursorPosition().y();
    BreakpointArea->blocks.push_back(line);

    return line;
}

void PlainTextEdit::removeBreakPoint(const int &line) {
    // create temp vector, first check if line is valid
    //BreakpointArea->blocks.push_back(line);
}



/* slots
 ------------------------------------------------------------------------- */
void PlainTextEdit::slotShowMenu(const QPoint &pos) {
    viewMenu->exec(viewport()->mapToGlobal(pos));
}

void PlainTextEdit::formatFile() {

}

void PlainTextEdit::slotGenerate() {

}

void PlainTextEdit::slotGoToDefinition() {

}

void PlainTextEdit::slotFindReferences() {

}

void PlainTextEdit::expand(){
    indentText(true);
}

void PlainTextEdit::collapse(){
    indentText(false);
}

// increase size when number are 10, 100, 1000, ...
void PlainTextEdit::slotBlockCountChanged(const int count)
{
    Q_UNUSED(count)
    // + BreakpointArea->sizeHint().width()
    setViewportMargins(LineArea->sizeHint().width(), 0, 0, 0);
}

void PlainTextEdit::slotHighlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> selections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        static QColor highlight = palette().color(QPalette::Text);
        highlight.setAlpha(25);
        selection.format.setBackground(highlight);
        selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selections.append(selection);
    }
    setExtraSelections(selections);
}

// update automatically whole view -> all widgets contained
void PlainTextEdit::slotUpdateRequest(const QRect &rect, const int column)
{
    if (column) {
        LineArea->scroll(0, column);
        BreakpointArea->scroll(0, column);
    }
    //LineArea->update(0, rect.y(), LineArea->width(), rect.height());
    //BreakpointArea->update(0, rect.y(), BreakpointArea->width(), rect.height());
    LineArea->update();
    BreakpointArea->update();
    if (rect.contains(viewport()->rect())) {
        slotBlockCountChanged(0);
    }
}

void PlainTextEdit::slotTextChanged()
{
    slotHighlightCurrentLine();
    slotBlockCountChanged(0);
}


/* plaintextedit protected functions
------------------------------------------------------------------------- */

void PlainTextEdit::dropEvent(QDropEvent *e){
    e->acceptProposedAction();
    QPlainTextEdit::dropEvent(e);
}

void PlainTextEdit::mouseReleaseEvent(QMouseEvent *e){
    /*
    if(getWordUnderCursor() == "{"){
        QList<QPoint> points = getParenthessesPairPositions();
        selectWord(points[0].x(), points[0].y());
        selectWord(points[1].x(), points[1].y());
    }
    */

    //selectWordUnderCursor();


    //qDebug() << getCursorPosition();
    //qDebug() << getWordUnderCursor();
    QPlainTextEdit::mouseReleaseEvent(e);
}

void PlainTextEdit::wheelEvent(QWheelEvent *event)
{
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

void PlainTextEdit::paintEvent(QPaintEvent *event)
{
    QPainter line(viewport());
    // symbolic line that represents width of 1 row --> 80 characters
    const int offset = static_cast<int>((fontMetrics().width('8') * 80)
                                        + contentOffset().x()
                                        + document()->documentMargin());
    QPen pen = line.pen();
    static QColor eol = palette().color(QPalette::Text);
    eol.setAlpha(50);
    pen.setColor(eol);
    // line made from dots
    pen.setStyle(Qt::DotLine);
    line.setPen(pen);
    line.drawLine(offset, 0, offset, viewport()->height());
    QPlainTextEdit::paintEvent(event);
}

void PlainTextEdit::resizeEvent(QResizeEvent *event)
{
    // whole text edit rect
    const QRect rect = contentsRect();
    // LineArea->setGeometry(QRect(rect.left(), rect.top(), LineArea->sizeHint().width(), rect.height()));
    LineArea->resize(LineArea->sizeHint().width(), rect.height());
    //BreakpointArea->setGeometry(QRect(rect.left(), rect.top(), BreakpointArea->sizeHint().width(), rect.height()));
    // BreakPoint width is fixed, since it is an image --> but still i can change for another BreakPoint
    BreakpointArea->resize(BreakpointArea->sizeHint().width(), rect.height());

    QPlainTextEdit::resizeEvent(event);
}

void PlainTextEdit::keyReleaseEvent(QKeyEvent *event)
{
    QTextCursor cursor = textCursor();
    /*
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Space){
        auto popup = completer->popup();
        popup->setCurrentIndex(completer->completionModel()->index(0,0));

        QRect rect = cursorRect();  // text cursor position
        rect.setWidth(50);
        rect.setHeight(30);
        completer->complete(rect);
    }
    if(event->modifiers() == Qt::ControlModifier){
        this->cursor().setShape(Qt::PointingHandCursor);
    }
    */
    /*
    bool isShortcut = ((event->modifiers() && Qt::ControlModifier) && event->key() == Qt::Key_E); // CTRL+E
    if (!completer || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyReleaseEvent(event);
    const bool ctrlOrShift = event->modifiers() && (Qt::ControlModifier | Qt::ShiftModifier);
    if (!completer || (ctrlOrShift && event->text().isEmpty()))
        return;
    if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 3
                        || eow.contains(event->text().right(1)))) {
        completer->popup()->hide();
        return;
    }
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    */
    QString completionPrefix = getWordUnderCursor();

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Space &&
        completionPrefix != completer->completionPrefix())
    {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));

        QRect cr = cursorRect();
        cr.setWidth(completer->popup()->sizeHintForColumn(0)
                    + completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(cr); // popup it up!
    }

    if (completer && completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (event->key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                completerInsertText(completionPrefix);
                completer->popup()->hide();  // complete done, hide popup and continue
                break;
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                event->ignore();
                QPlainTextEdit::keyReleaseEvent(event);
                //return; // let the completer do default behavior
                break;
            default:
                event->ignore();
                QPlainTextEdit::keyReleaseEvent(event);
                break;
        }
    }

    switch (event->key()) {
        case Qt::Key_Backtab:
        case Qt::Key_Tab: {
            bool forward = !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
            if (indentText(forward)) {
                event->accept();
                return;
            } else if (forward) {
                QString text = TABS_TO_SPACES ? QString(TAB_STOP_WIDTH, ' ') : QChar('\t');
                //QTextCursor cursor = textCursor();
                cursor.insertText(text);
                setTextCursor(cursor);
                event->accept();
                return;
            }
            break;
        }
        case Qt::Key_Enter:
        case Qt::Key_Return:
            break;
        case Qt::Key_Down:
        case Qt::Key_Up:
            if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))  {
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
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("]");
            break;
        case Qt::Key_BraceLeft:
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("}");
            break;
        case Qt::Key_ParenLeft:
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText(")");
            break;
        case Qt::Key_Less:
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("> ");
            break;
        case Qt::Key_Slash:     // kind a stupid idea.
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("/");
            break;
        case Qt::Key_Backslash:     // kind a stupid idea.
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("\\");
            break;
        case Qt::Key_QuoteDbl:
        case Qt::Key_QuoteLeft:
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.insertText("'");
            break;

        default:
            break;
    }
    //QPlainTextEdit::keyReleaseEvent(event);
}
/*
void PlainTextEdit::keyPressEvent(QKeyEvent *event) {
    QPlainTextEdit::keyPressEvent(event);
}
 */

void PlainTextEdit::focusInEvent(QFocusEvent *e) {

    if(completer){
        completer->setWidget(this);
    }

    QPlainTextEdit::focusInEvent(e);

}


/* LineNumberArea widget
------------------------------------------------------------------------- */

LineNumberArea::LineNumberArea(PlainTextEdit *edit) : QWidget(edit), m_Edit(edit){}

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
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(m_Edit->contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    QRect full = e->rect();
    painter.fillRect(full, palette().color(QPalette::Base));
    while (block.isValid() && (top <= full.bottom())) {
        if (block.isVisible() && (bottom >= full.top())) {
            // !!! since BreakPoint is on left, i need to move first x pos, or on left -> change break area x pos
            QRect box(0, top, width(), m_Edit->fontMetrics().height());
            QFont font = painter.font();
            font.setFamily(m_Edit->font().family());
            font.setPointSize(m_Edit->font().pointSize());
            if (m_Edit->textCursor().blockNumber() == i) {
                painter.fillRect(box, palette().color(QPalette::Highlight));
                painter.setPen(palette().color(QPalette::HighlightedText));
                font.setWeight(QFont::Bold);
            } else {
                font.setWeight(QFont::Normal);
                painter.setPen(palette().color(QPalette::Text));
            }
            painter.setFont(font);
            painter.drawText(box.left(), box.top(), box.width(), box.height(), Qt::AlignRight, QString::number(i + 1).append(' '));
            painter.setPen(palette().color(QPalette::Highlight));
            painter.drawLine(full.topRight(), full.bottomRight());
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
        ++i;
    }
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

QSize LineNumberArea::sizeHint() const
{
    int digits = 1;
    int blocks = qMax(1, m_Edit->blockCount());
    while (blocks >= 10) {
        blocks /= 10;
        digits++;
    }
    digits++;
    digits++;
    return QSize((3 + (m_Edit->fontMetrics().width('8') * digits)), 0);
}

void LineNumberArea::wheelEvent(QWheelEvent *e)
{
    QApplication::sendEvent(m_Edit->viewport(), e);
}

/* BreakPointArea widget
------------------------------------------------------------------------- */

BreakPointArea::BreakPointArea(PlainTextEdit *edit) : QWidget(edit), m_Edit(edit){
    breakpoint.load(IconFactory::BreakPoint);
    setFixedWidth(breakpoint.width());
    blocks.reserve(5);
}

bool BreakPointArea::canCreateBreakPoint(const QTextBlock &block) {
    //int count;
    for (int i = 0; i < blocks.size(); ++i) {
        // if already contain -> remove it, paint only background pixels, else create
        if(blocks[i] == block.blockNumber()){
            // remove it from list
            //breakPointRemoved();
            //qDebug() << blocks;
            //count++;
            return true;
        }
    }

    // can create
    return false;
}

QSize BreakPointArea::sizeHint() const {
    return QSize((breakpoint.width()), 0);
}

void BreakPointArea::mouseEvent(QMouseEvent *event) {
    QTextCursor cursor = m_Edit->cursorForPosition(QPoint(0, event->pos().y()));
    QTextBlock touched_block = cursor.block();
    if ((event->type() == QEvent::MouseButtonPress) && (event->button() == Qt::LeftButton)) {
        // record touched block so in paintEvent i can create, remove at clicked line(block)
        blocks.push_back(touched_block.blockNumber());
        cursor.setVisualNavigation(true);
        m_Edit->setTextCursor(cursor);
        //emit breakPointCreated();
        //qDebug() << touched_block.blockNumber();
        //qDebug() << blocks;
    }
}

void BreakPointArea::mousePressEvent(QMouseEvent *event) {
    mouseEvent(event);
}

void BreakPointArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QTextBlock block = m_Edit->firstVisibleBlockProxy();
    // y1, y2 coordinates of rectangle we are going to paint in: top, top + height of 1 line(block)
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(m_Edit->contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    // widget area rectangle
    QRect area_rect = event->rect();
    // while not all lines are painted, at the end increasing, checking if valid
    while (block.isValid() && (top <= area_rect.bottom())) {
        if (block.isVisible() && (bottom >= area_rect.top())) {
            // width is fixed in constructor, height -> i want to fill by font size, since it can increase
            // TODO: first x is problem, since line numbers are also like that painted
            if(canCreateBreakPoint(block)){
                // was 0
                QRect box(0, top, width(), m_Edit->fontMetrics().height());
                painter.drawPixmap(box, breakpoint);
            }
        }
        block = block.next();
        top = bottom;
        bottom = (top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height()));
    }
    //QWidget::paintEvent(event);
}

