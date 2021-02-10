#ifndef SOURCECODEEDIT_H
#define SOURCECODEEDIT_H

/*
 * make 1 QTextCursor and init him in constructor, not every time initializing, it takes time
 *
 *
 *
*/


#include <QPlainTextEdit>
#include <QTextCursor>
#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QCompleter>
#include <QAbstractItemView>
#include <QStringListModel>

#include <QFileSystemModel>
#include <QTableView>
#include <QListView>

#include <iostream>
#include <vector>

class LineNumberArea;
class BreakPointArea;
class Arrow;

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit() = default;

    BreakPointArea *BreakpointArea;
    Arrow *ArrowArea;

    QRectF blockBoundingGeometryProxy(const QTextBlock &block);
    QRectF blockBoundingRectProxy(const QTextBlock &block);
    QPointF contentOffsetProxy();
    QTextBlock firstVisibleBlockProxy();
    void moveCursor(const bool &end);

    // search
    struct searchResult{
        QString fileName;
        int row;
        int col;
    };
    std::vector<searchResult> search_results;
    void findStoreAndSelectAll(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    // find() does not set text cursor, findNext() does
    bool find(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void findNext(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void replace(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    // returns how many occurrences were replaces
    int replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);

    // cursor
    void setCursorPosition(const int &row, const int &col);
    QPoint getCursorPosition();
    void setCursorAtLine(const int &line);

    // text manipulation
    void selectLineUnderCursor();
    QString getLineUnderCursor();
    QString getLineContent(const int &row);
    void selectWordUnderCursor();
    QString getWordUnderCursor();
    void deleteLine();

    // other
    void setFileExtension(const QString &extension = "cpp");
    void setFilePath(const QString &file_path);
    QString getFilePath();

    // return line where BreakPoint was created, also set it into view
    // called outside
    int setBreakPoint();
    void removeBreakPoint(const int &line);

    // completer words, will get from clang parser
    void setCompletionData(const std::vector<std::string> &data);


    // Selections:

    // required, bc if i set selection somewhere this selection is overwritten by current_line selection, which already
    // is in update requests
    // manages extra selections, and set all of them in update requests, !! merges all of them together !!
    // redundancy and updating each selection list is done within functions/slots they are filled ...  .clear()
    void manageExtraSelections();
    QList<QTextEdit::ExtraSelection> extra_selections_search_results;
    // search selections; false -> clear them when find widget is closed && ??
    bool clearSelectionsBySearch = false;
    QList<QTextEdit::ExtraSelection> extra_selections_search_touched_results;  // also {} contained here
    // mouse click 1 file selections;  false -> clear when text cursor is no longer on the text && typing
    bool clearSelectionsByTouch = false;
    // tracking word we are on
    QString wordUnderCursor;
    QString tempWordUnderCursor;
    QList<QTextEdit::ExtraSelection> extra_selections_warning_line;
    bool clearWarningLineSelections = false;
    QList<QTextEdit::ExtraSelection> extra_selections_error_line;
    bool clearErrorLineSelections = false;
    // this one is fixed
    QList<QTextEdit::ExtraSelection> extra_selections_current_line;

    enum lineSelection{
        Warning = 0,
        Error
    };
    // remove specific line selection is tricky, so remove them all and again call this for all selections
    // clears warning or error selection list
    void setLineSelection(const int &line, const lineSelection& type, const bool& removeAll = false);
    // works with extra_selections_search_touched_results, select pairs, nested also
    // lookup pairs like <> () {} ""
    void addPairsSelections(const QString &first);
    // missing selected text for return -> no use for now
    void highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background, const int &line,
                   const QColor &color = QColor::fromRgb(0, 255, 0));

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    //void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void dropEvent(QDropEvent *e) override;
private:

    LineNumberArea *LineArea;
    QMenu *viewMenu;

    void createMenu();
    QCompleter *completer;

    void SetupCompleter();
    int indentSize(const QString &text);
    bool indentText(const bool &forward);
    QString indentText(QString text, int count) const;
    void moveSelection(const bool &up);

    void transformText(const bool &upper);
    QString file_extension;

    QString file;

public slots:
    void completerInsertText(const QString &text);

    void toggleComment();
    void formatFile();
    void slotGoToDefinition();
    void slotFindReferences();
    void slotGenerate();
    void expand();
    void collapse();

private slots:
    void slotShowMenu(const QPoint &pos);
    void slotBlockCountChanged(const int count);
    void slotHighlightCurrentLine(); // cursor position changed
    void slotUpdateRequest(const QRect &rect, const int column);
    void slotTextChanged();
};



class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(PlainTextEdit *edit);
    ~LineNumberArea() = default;
    // reprezents height, width or area even while changing its dimensions
    QSize sizeHint() const override;

protected:
    void leaveEvent(QEvent *event) override;
    // this event is done in all types of mouse events on widget
    void mouseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    PlainTextEdit *m_Edit;
};


class BreakPointArea : public QWidget
{
Q_OBJECT
public:
    explicit BreakPointArea(PlainTextEdit *edit);
    ~BreakPointArea() = default;
    // reprezents height, width or area even while changing its dimensions
    QSize sizeHint() const override;
    QPixmap breakpoint;
    // represents breakpoints at lines
    std::vector<int> B_blocks;

protected:
    void mouseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    //void breakPointCreated();
    //void breakPointRemoved();

private:
    PlainTextEdit *m_Edit;

    bool canCreateBreakPoint(const QTextBlock &block);

};


class Arrow : public QWidget
{
Q_OBJECT
public:
    explicit Arrow(PlainTextEdit *edit);
    ~Arrow() = default;
    // reprezents height, width or area even while changing its dimensions
    //QSize sizeHint() const override;

protected:
    //void mouseReleaseEvent(QMouseEvent *event) override;
    //void paintEvent(QPaintEvent *event) override;

private:
    PlainTextEdit *m_Edit;

};

#endif // SOURCECODEEDIT_H
