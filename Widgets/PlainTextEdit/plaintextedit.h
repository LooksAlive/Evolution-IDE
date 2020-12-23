#ifndef SOURCECODEEDIT_H
#define SOURCECODEEDIT_H

#include <QPlainTextEdit>

class LineNumberArea;

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    QRectF blockBoundingGeometryProxy(const QTextBlock &block);
    QRectF blockBoundingRectProxy(const QTextBlock &block);
    QPointF contentOffsetProxy();
    QTextBlock firstVisibleBlockProxy();
    void gotoLine(const int no);
    void moveCursor(const bool end);
    void findInTextEdit(const QString &search, const QTextDocument::FindFlags &find_options);

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    LineNumberArea *LineArea;
    int indentSize(const QString &text);
    bool indentText(const bool forward);
    QString indentText(QString text, int count) const;
    void moveSelection(const bool up);
    void transformText(const bool upper);
    void setSelectedWordstyle();

private slots:
    void handleBlockCountChanged(const int count);
    void handleCursorPositionChanged();
    void handleUpdateRequest(const QRect &rect, const int column);
    void handleTextChanged();
};



class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(PlainTextEdit *edit);
    QSize sizeHint() const;

protected:
    void leaveEvent(QEvent *event);
    void mouseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    PlainTextEdit *m_Edit;
};

#endif // SOURCECODEEDIT_H
