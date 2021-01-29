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

#include <iostream>
#include <vector>
#include <unordered_map>
#include "completer.h"

class LineNumberArea;
class BreakPointArea;
class Arrow;

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit() = default;

    QRectF blockBoundingGeometryProxy(const QTextBlock &block);
    QRectF blockBoundingRectProxy(const QTextBlock &block);
    QPointF contentOffsetProxy();
    QTextBlock firstVisibleBlockProxy();
    void moveCursor(const bool &end);

    // search
    std::unordered_map<QString, int> search_results;
    std::vector<int> search_results_positions;
    void findStoreAndSelectAll(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);
    // find() does not set text cursor, findNext() does
    bool find(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);
    void findNext(const QString &search, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);
    void replace(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);
    void replaceAndFind(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);
    int replaceAll(const QString &oldText, const QString &newText, const QTextDocument::FindFlags &find_options = QTextDocument::FindWholeWords);

    // cursor
    void setCursorPosition(const int &row, const int &col);
    QPoint getCursorPosition();
    void setCursorAtLine(const int &line);

    // text manipulation
    void selectLineUnderCursor();
    QString getLineUnderCursor();
    void selectWordUnderCursor();
    QString getWordUnderCursor();
    void selectWord(const int &line, const int &column);
    // missing selected text for return -> no use for now
    void deleteLine();
    void highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background,
                   const QColor &color = QColor::fromRgb(0, 255, 0));
    QList<QPoint> getParenthessesPairPositions();   // set cursor selection on them in update request

    // other
    void setFileExtension(const QString &extension = "cpp");
    void setFilePath(const QString &file_path);
    QString getFilePath();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dropEvent(QDropEvent *e) override;

private:
    LineNumberArea *LineArea;
    BreakPointArea *BreakpointArea;
    Arrow *arrowArea;

    QMenu *viewMenu;
    void createMenu();

    int indentSize(const QString &text);
    bool indentText(const bool forward);
    QString indentText(QString text, int count) const;
    void moveSelection(const bool up);
    void transformText(const bool upper);

    QString file_extension;
    QString file;
    QList<QTextEdit::ExtraSelection> search_selections;  // pairs identical words

public slots:
    void toggleComment();
    void formatFile();
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

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void breakPointCreated();
    void breakPointDeleted();

private:
    PlainTextEdit *m_Edit;

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
