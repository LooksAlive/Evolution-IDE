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
#include <QTimer>

#include <QFileSystemModel>
#include <QTableWidget>
#include <QListView>
#include <QTreeWidget>
#include <QImage>
#include <QToolTip>
#include <QWhatsThis>
#include <QHelpEvent>
#include <QDesktopServices>
#include <QEvent>
#include <QScrollBar>

#include <iostream>
#include <vector>

#include "icons/IconFactory.h"
#include "Widgets/Education/Education.h"
#include "Widgets/CommentTagsReminder/CommentTagsReminder.h"
#include "Widgets/DocumentationHelper/DocumentationHelper.h"
#include "GoToLineColumn.h"

#include "completer.h"
#include "HoverInfoWidget.h"
#include "EnvironmentSettings.h"    // completer default data for file type
#include "SpellCheckList.h"         // spell suggestions popup
#include "CommentTagList.h"
#include "InformativeMessage.h"

#include "SmallRoundedEdit.h"

class LineNumberArea;

class BreakPointArea;

class ArrowArea;

class ScrollBar;

class StatusArea;

class ClangBridge;

class NodeView;

class CodeNotifyArea;

class NotifyWidget;

class CodeInfoDock;

class PlainTextEditExtra : public QPlainTextEdit {
Q_OBJECT
public:
    explicit PlainTextEditExtra(QWidget *parent = nullptr);

    ~PlainTextEditExtra() = default;

    BreakPointArea *breakPointArea;
    LineNumberArea *lineNumberArea;
    ArrowArea *arrowArea;
    CodeNotifyArea *codeNotifyArea;
    StatusArea *statusArea;
    ScrollBar *scrollBar;
    // only triggers actions to do with its Handler ...
    CodeInfoDock *code_info;
    // parsing
    ClangBridge *clang;
    NodeView *nodeView;
    // TODO: add option for sample creation into menu
    Education *education;
    NewSampleWindow *newSampleWindow;
    CommentTagsReminder *tagReminder;
    DocumentationHelper *documentationHelper;

    Completer *completer;
    SpellCheckList *spellCheckPopup;
    CommentTagList *tagListPopup;
    // shown for 2000ms -> then hiden; no more handling needed
    InformativeMessage *infoMessage;

    // these will be displayed and set when worker thread finish (find data), but
    // signal is emitted from here.
    HoverInfoWidget *hoverInfo;
    // shows collapsed scopes
    SmallRoundedEdit *smallEdit;

    void setCodeInfo(CodeInfoDock *code_info_widget) { code_info = code_info_widget; }

    void setClang(ClangBridge *clang_bridge) { clang = clang_bridge; }

    void setNodeView(NodeView *nodes) { nodeView = nodes; }

    void setEducation(Education *edu) { education = edu; }

    void setTagReminder(CommentTagsReminder *reminder) { tagReminder = reminder; }

    // tab width
    static constexpr unsigned int TAB_STOP_WIDTH = 4;
    // insert spaces instead of tab or not
    static constexpr bool TABS_TO_SPACES = false;

    // !! protected !! stuffs, call them in side widgets (lines, Bp.)
    QRectF blockBoundingGeometryProxy(const QTextBlock &block) const;

    QRectF blockBoundingRectProxy(const QTextBlock &block) const;

    QPointF contentOffsetProxy() const;
    QTextBlock firstVisibleBlockProxy() const;

    // search
    std::vector<QPoint> search_results;
    void findStoreAndSelectAll(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    // set cursor
    bool find(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    // not setting cursor
    bool find(QTextCursor &cursor, const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void findNext(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void replace(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    void replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);
    // returns how many occurrences were replaces
    int replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindCaseSensitively);

    // only cursor pointer
    void setCursorPosition(QTextCursor &cursor, const int &row, const int &col);
    // will set cursor
    void setCursorPosition(const int &row, const int &col);
    // useless, sets the cursor for position...
    QPoint getCursorPosition();
    // onl with pointer
    QPoint getCursorPosition(QTextCursor &cursor);
    void setCursorAtLine(const int &line);

    // text manipulation
    void selectLineUnderCursor();
    // only pointer
    void selectLineUnderCursor(QTextCursor &cursor);
    // will set cursor
    QString getLineUnderCursor();
    // only pointer
    QString getLineUnderCursor(QTextCursor &cursor);
    // returns content for specified line/row + set cursor there
    QString getLineContent(const int &row, const bool &setCursor = true);
    void selectWordUnderCursor();
    QString getWordUnderCursor();

    void deleteLine();

    void deleteLine(QTextCursor &cursor);

    // other
    void setFileExtension(const QString &extension = "cpp");

    void setFilePath(const QString &file_path);

    QString getFilePath() const;

    // returns how many times there is indentation for line
    int countSpaceTabs(const QString& lineContent, const int& indentation);

    // false -> removed;   true -> created
    bool toggleBreakPoint(const int& line) const;

    // Selections:

    // required, bc if i set selection somewhere this selection is overwritten by current_line selection, which already
    // is in update requests
    // updates extra selections, and set all of them in update requests, !! merges all of them together !!
    // redundancy and updating each selection list is done within functions/slots they are filled ...  .clear()
    void updateExtraSelections();

    // in search
    bool doNotSetSelections = false;
    QList<QTextEdit::ExtraSelection> extra_selections_search_results;
    QList<QTextEdit::ExtraSelection> extra_selections_search_touched_results;// also {} contained here
    // current word
    QString wordUnderCursor;
    // word after next mouse touch
    QString tempWordUnderCursor;
    QList<QTextEdit::ExtraSelection> extra_selections_line;
    QList<QTextEdit::ExtraSelection> extra_selections_underline;
    // this one is fixed
    QList<QTextEdit::ExtraSelection> extra_selections_current_line;

    enum selectionType {
        Warning = 0,
        Error,
        BreakPoint,  // only for line
        SpellCheck   // wave underline
    };

    // remove specific line selection is tricky, so remove them all (in update request) and again call this for all selections
    // clears warning or error selection list
    void setLineSelection(const int &line, const selectionType &type, const bool &removeAll = false);

    // underline text
    void setUnderLineSelection(const QPoint &start, const QPoint &end, const selectionType &type);

    // collapses text and add ... href instead;  NOTE! replaces text is in href=&
    void setCollapsableText(const QPoint &start, const QPoint &end);

    // links, accessible in mouse release, press events, changing cursor, etc.
    QString anchor;

    // works with extra_selections_search_touched_results, select pairs, nested also
    // lookup pairs like <> () {} ""
    void searchPairsSelections(QTextCursor &cursor, const QString &first);

    // missing selected text for return -> no use for now
    void highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background, const int &line,
                   const QColor &color = QColor::fromRgb(0, 255, 0));

    void showMessage(const QString &message);

protected:
    bool event(QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *e) override;

    void focusInEvent(QFocusEvent *e) override;

    void focusOutEvent(QFocusEvent *e) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void dropEvent(QDropEvent *e) override;

private:
    QMenu *viewMenu;

    void createMenu();

    void SetupAdditionalWidgets();

    int indentSize(const QString &text);

    QString indentText(QString text, int count) const;

    bool indentText(const bool &forward);

    // if we are in scope and return/enter pressed --> insert line with indent(code dependent)
    // returns true if spaces, tabs were inserted to next block, also move cursor there
    bool autoEnterTextIndentation();

    // when removing empty line(Backspace) with spaces, tabs in or not in scope
    void autoBlankLineDeletion();

    // from: sometext (selected)    to: "selected"    ; also manages indentation
    // returns true when text were formated
    bool autoTextSurrounding(QTextCursor &cursor, const QString &pair);

    // move text around with ctrl+up/down
    void moveSelection(const bool &up);

    // text to upper, lower
    void transformText(const bool &upper);

    void moveCursor(const bool &end);

    QString file_extension;

    // file associated with editor
    QString filepath;

    QTimer *touchTimer;

public slots:

    void completerInsertText(const QString &text);

    void slotInsertSuggestion(QListWidgetItem *item);

    void slotInsertTagComment(QListWidgetItem *item);

    // search and select mouse touched word in this file, for now, do nothing
    void searchByMouseTouch();

    void toggleComment();

    void formatFile() const;

    void slotGoToDefinition() const;

    void slotFindReferences() const;

    void slotGenerate() const;

    void slotShowCommentTags() const; // for this file
    void slotAddCommentTags();  // toggle tag list
    void slotShowInNodeView();

    void slotExpand();

    void slotCollapse();

    void slotShowNewSampleWindow();

    void slotCreateSample();

private slots:

    void slotShowMenu(const QPoint &pos);

    void slotBlockCountChanged(const int &count);

    void slotHighlightCurrentLine();// cursor position changed
    void slotUpdateRequest(const QRect &rect, const int column);
    void slotTextChanged();

    void slotToggleBreakPoint();
};



class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(PlainTextEditExtra *edit, QWidget *parent = nullptr);

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
    PlainTextEditExtra *m_Edit;
};


class BreakPointArea : public QWidget
{
Q_OBJECT
public:
    explicit BreakPointArea(PlainTextEditExtra *edit, QWidget *parent = nullptr);

    ~BreakPointArea() = default;
    // represents height, width or area even while changing its dimensions
    QSize sizeHint() const override;
    // represents breakpoints at lines
    std::vector<int> B_blocks;

    // true -> was erased;   false -> was created
    bool containBlock(const int &line);

signals:

    void breakPointCreated(const int &line);

    void breakPointRemoved(const int &line);

private:
    PlainTextEditExtra *m_Edit;

    // TODO: consider widgets [ setting their postition ]
    QPixmap breakpoint;

    bool canCreateBreakPoint(const QTextBlock &block);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent *event);

};


class ArrowArea : public QWidget {
Q_OBJECT
public:
    explicit ArrowArea(PlainTextEditExtra *edit, QWidget *parent = nullptr);

    ~ArrowArea() = default;

    // reprezents height, width or area even while changing its dimensions, TODO: handle increasing view
    QSize sizeHint() const override;

    // only collapsed lines, other draw automatically collapse arrow
    std::vector<int> expanded;
    // arrows in line, set by clang
    std::vector<int> arrows;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    PlainTextEditExtra *m_Edit;

    QPixmap arrowCollapse;
    QPixmap arrowExpand;


    // this is similarly done by links, so it is connected
    // if so, ... get line href(word at the end) and expand by anchor href + rm, same algorithm
    // for collapsing use PlainTextEditExtra::setCollapsableText() with boundings from clang + change arrow
    // called when switching from one to other
    void collapseOrExpand(const int &line);

    // return true if expanded, false otherwise(collapsed)
    bool isExpanded(const int &line);

    // return true, when there should be an arrow
    bool isMarked(const int &line);
};


// this sets NotifyWidget position to display. signals are handled here
class CodeNotifyArea : public QWidget {
Q_OBJECT
public:
    explicit CodeNotifyArea(PlainTextEditExtra *edit, QWidget *parent = nullptr);

    ~CodeNotifyArea() = default;

    // represents height, width or area even while changing its dimensions, TODO: handle increasing view
    // width: 30px
    QSize sizeHint() const override;

    enum Notification {
        Error,
        Warning,
        Fixit,
        Color
    };

    struct notifier {
        QString tooltip;
        int line;
        Notification notify;
        QPixmap image;
    };
    std::vector<notifier> notifiers;

protected:
    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent *event);

    void mousePressEvent(QMouseEvent *event) override;

private:
    PlainTextEditExtra *m_Edit;

    // width and height for line, solved increasing view
    QPoint WH;

    // FIXME: Do i have to put it into layout?? or will geometry work in area when scrolling

    // FIXME: get only line number, its height is in WH, compute corner by (num-1)*WH.y() ; add 30, 30

    // linter like things
    // /p line , w, h are 30
    void addNotification(const QString &toolTip, const int &line, const Notification &type);

    // TODO: try to reuse memory, just change toolTip, image and position ... (speed up)
    void removeNotification(const int &line);

    // if returns true, continue with whatToDraw
    bool canDrawImage(const int &line);

    QPixmap whatToDraw(const int &line);

    QPixmap error;
    QPixmap warning;
    QPixmap color;
    QPixmap fixit;

};


// custom scrollBar draws line width=2; where is some notification in text or
// any other notification.
class ScrollBar : public QScrollBar {
Q_OBJECT
public:
    explicit ScrollBar(PlainTextEditExtra *edit, QWidget *parent = nullptr);

    ~ScrollBar() = default;

    // We could increase size and drah text into it with QTextDocument of course,
    // when smallEdit is ON
    // QSize sizeHint() const override { return QSize(55, 22); }
    // QSize minimumSizeHint() const override { return QSize(55, 22); }

protected:
    // painting lines like in scrollBar + TEXT
    void paintEvent(QPaintEvent *event) override;

    // go to line
    void mousePressEvent(QMouseEvent *event) override;

    // show custom edit on TOP viewport with same document *
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // hide when leaving area
    void leaveEvent(QEvent *event) override;

    // when enterred, it is shown when mouse not moving for 2000ms (singleShot timer)
    void enterEvent(QEvent *event) override;

    // show view according to mouse position
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    PlainTextEditExtra *m_Edit;
    SmallRoundedEdit *smallEdit;

    // indicates wheather we are still in scrollBar widget
    bool inWidget = false;
    // scroll could amplify line range
    int HEIGHT = 300;


    // returns height for painter to match exact line
    float computeHeightForLine(const int &line);

    // from event realy mouse position
    void calculateAndSetSmallEditGeometry(const int &posY);

};






// this is top status bar in editor
class StatusArea : public QWidget {
Q_OBJECT
public:
    explicit StatusArea(PlainTextEditExtra *edit, QWidget *parent = nullptr);
    ~StatusArea() = default;

    // QSize sizeHint() const override;

    QToolButton *lineColumn;



private:
    PlainTextEditExtra *m_Edit;
    GoToLineColumn *goToLineColumn;

    QHBoxLayout *MainLayout;
    // top rigth
    QToolButton *collapseAllScopes;
    QToolButton *expandAllScopes;

    // NOTE: for click, positions are still changing so that search and cursor will set by searching pos.
    // also set up its icons
    QComboBox *currentSymbolBox;

    QToolButton *encoding;

    QToolButton *documentCode;
    QToolButton *commentTagsforThisFile;

    void createWindow();

};

/*
    // later consider creating new button here
    // What if on the changed tab is no cursor set -> remain last cursor position
    if (Tabs->isVisible()) {
        const QPoint point = currentWidget->getCursorPosition();
        const QString pos = QString::number(point.y()) + ":" + QString::number(point.x());// row:col

        github_branch->setText(pos);
    } else {
        github_branch->setText("");
    }
*/









#endif // SOURCECODEEDIT_H
