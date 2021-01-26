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
    // textinfoarea = new TextInfoArea(this);

    //setCornerWidget(reinterpret_cast<QWidget*>(textinfoarea));
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


    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &PlainTextEdit::slotHighlightCurrentLine);
    connect(this, &QPlainTextEdit::blockCountChanged, this, &PlainTextEdit::slotBlockCountChanged);
    connect(this, &QPlainTextEdit::textChanged, this, &PlainTextEdit::slotTextChanged);
    connect(this, &QPlainTextEdit::updateRequest, this, &PlainTextEdit::slotUpdateRequest);

    connect(new QShortcut(Qt::CTRL | Qt::Key_U, this), &QShortcut::activated, [=] {
        transformText(true);
    });
    connect(new QShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_U, this), &QShortcut::activated, [=] {
        transformText(false);
    });
    connect(new QShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Up, this), &QShortcut::activated, [=] {
        moveSelection(true);
    });
    connect(new QShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Down, this), &QShortcut::activated, [=] {
        moveSelection(false);
    });
    connect(new QShortcut(Qt::CTRL | Qt::Key_Plus, this), &QShortcut::activated, [=] {
        zoomIn();
    });
    connect(new QShortcut(Qt::CTRL | Qt::Key_Minus, this), &QShortcut::activated, [=] {
        zoomOut();
    });

    createMenu();
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
    const QTextBlock block = document()->findBlockByLineNumber(row - 1);
    if(block.isValid())
    {
        QTextCursor cursor = textCursor();
        cursor.setPosition(block.position() + col + 1);
        setTextCursor(cursor);
        ensureCursorVisible();
    }
}

QPoint PlainTextEdit::getCursorPosition(){
    QTextCursor cursor = textCursor();
    int row = cursor.blockNumber() + 1;     // +1
    int col = cursor.columnNumber() + 1;    // +1 , ending position

    return QPoint(row, col);
}

void PlainTextEdit::setCursorAtLine(const int &line)
{
    setCursorPosition(line + 1, 0);   // yet no idea how ? jumps as it wants
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

bool PlainTextEdit::indentText(const bool forward)
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
void PlainTextEdit::moveSelection(const bool up)
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

void PlainTextEdit::transformText(const bool upper)
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

void PlainTextEdit::highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background,
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
        selections.append(selection);
    }
}

// search
void PlainTextEdit::findStoreAndSelectAll(const QString &search, const QTextDocument::FindFlags &find_options)
{
    //search_selections.clear();

    while(find(search, find_options)){
        QTextEdit::ExtraSelection selection;
        selection.cursor.movePosition(QTextCursor::Start);
        QColor highlight = palette().color(QPalette::Dark);
        highlight.setAlpha(25);
        selection.format.setBackground(highlight);
        selection.format.setProperty(QTextCharFormat::FullWidthSelection, true);
        //selection.cursor = textCursor();
        selection.cursor = document()->find(search, selection.cursor, find_options);
        //selection.cursor.select(QTextCursor::WordUnderCursor);
        search_selections.push_back(selection);
        selection.cursor.clearSelection();

        search_results_positions.push_back(selection.cursor.position());
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 1);
        // later search_results for multifile search
    }
    /*
    for (int i = 0; i < search_selections.size(); ++i) {
        qDebug() << search_selections[i].cursor.selectedText();
    }
    */
    if(!search_selections.isEmpty()){
        setExtraSelections(search_selections);
    }
}

bool PlainTextEdit::find(const QString &search, const QTextDocument::FindFlags &find_options)
{
    QTextCursor cursor = textCursor();
    cursor = document()->find(search, cursor, find_options);

    if(!cursor.isNull()){
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
    findNext(oldText, find_options);
    QTextCursor cursor = textCursor();

    if(cursor.hasSelection() && !isReadOnly()/* && cursor.selectedText() == oldText */)
    {
        cursor.beginEditBlock();
        cursor.removeSelectedText();
        cursor.insertText(newText);
        cursor.endEditBlock();
        setTextCursor(cursor);
    }
}

void PlainTextEdit::replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options)
{
    if(find(oldText, find_options)){
        replace(oldText, newText, find_options);
    }
}

int PlainTextEdit::replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options)
{
    int count = 0;
    textCursor().movePosition(QTextCursor::Start); // on start

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
    /*
    viewMenu->addAction("Format File", this, SLOT(formatFile()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);
    viewMenu->addAction("Go to Definition", this, SLOT(slotGoToDefinition()), Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    viewMenu->addAction("Find References", this, SLOT(slotFindReferences()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);
    */

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotShowMenu(const QPoint&)));
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

/* slots
 ------------------------------------------------------------------------- */

void PlainTextEdit::slotShowMenu(const QPoint &pos) {
    viewMenu->exec(viewport()->mapToGlobal(pos));
}

void PlainTextEdit::formatFile() {

}

void PlainTextEdit::expand(){
    indentText(true);
}

void PlainTextEdit::collapse(){
    indentText(false);
}

void PlainTextEdit::slotBlockCountChanged(const int count)
{
    Q_UNUSED(count)
    setViewportMargins(LineArea->sizeHint().width(), 0, 0, 0);
}

void PlainTextEdit::slotHighlightCurrentLine()
{
    // for tracking outside
    emit cursorPositionHasChanged();
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

void PlainTextEdit::slotUpdateRequest(const QRect &rect, const int column)
{
    if (column) {
        LineArea->scroll(0, column);
    }
    LineArea->update(0, rect.y(), LineArea->width(), rect.height());
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
    const int offset = static_cast<int>((fontMetrics().width('8') * 80)
                                        + contentOffset().x()
                                        + document()->documentMargin());
    QPen pen = line.pen();
    static QColor eol = palette().color(QPalette::Text);
    eol.setAlpha(50);
    pen.setColor(eol);
    pen.setStyle(Qt::DotLine);
    line.setPen(pen);
    line.drawLine(offset, 0, offset, viewport()->height());
    QPlainTextEdit::paintEvent(event);
}

void PlainTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect rect = contentsRect();
    LineArea->setGeometry(QRect(rect.left(), rect.top(), LineArea->sizeHint().width(), rect.height()));
}

void PlainTextEdit::keyReleaseEvent(QKeyEvent *event)
{
    QTextCursor cursor = textCursor();

    switch (event->key()) {
        case Qt::Key_Backtab:
        case Qt::Key_Tab: {
            bool forward = !QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
            if (indentText(forward)) {
                event->accept();
                return;
            } else if (forward) {
                QString text = TABS_TO_SPACES ? QString(TAB_STOP_WIDTH, ' ') : QChar('\t');
                QTextCursor cursor = textCursor();
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
            /* // solved by connect shortcut
            case Qt::Key_Plus:
            case Qt::Key_Minus:
                if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)){
                    if (event->key() == Qt::Key_Plus){
                        // setReadOnly(true);
                        zoomIn();
                    }
                    if(event->key() == Qt::Key_Minus){
                        // setReadOnly(true);
                        zoomOut();
                    }
                    event->accept();
                }
                break;
                */
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
    QPlainTextEdit::keyReleaseEvent(event);
}

/* LineNumberArea widget
------------------------------------------------------------------------- */

LineNumberArea::LineNumberArea(PlainTextEdit *edit)
    : QWidget(edit), m_Edit(edit)
{
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
    int top = static_cast<int>(m_Edit->blockBoundingGeometryProxy(block).translated(m_Edit->contentOffsetProxy()).top());
    int bottom = top + static_cast<int>(m_Edit->blockBoundingRectProxy(block).height());
    QRect full = e->rect();
    painter.fillRect(full, palette().color(QPalette::Base));
    while (block.isValid() && (top <= full.bottom())) {
        if (block.isVisible() && (bottom >= full.top())) {
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

/* TextInfoArea widget
------------------------------------------------------------------------- */

TextInfoArea::TextInfoArea(PlainTextEdit *edit) : QWidget(edit), m_Edit(edit)
{
    buildTextInfoArea();
}

void TextInfoArea::buildTextInfoArea() {
    position = new QLabel(this);
    QPoint poss = m_Edit->getCursorPosition();
    QString pos = "row: " + QString::number(poss.x()) + " col: " + QString::number(poss.y());
    position->setText(pos);
}