#ifndef SOURCECODEEDIT_H
#define SOURCECODEEDIT_H

#include <QPlainTextEdit>

#include "completer.h"

class LineNumberArea;

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    QRectF blockBoundingGeometryProxy(const QTextBlock &block);
    QRectF blockBoundingRectProxy(const QTextBlock &block);
    QPointF contentOffsetProxy();
    QTextBlock firstVisibleBlockProxy();
    void gotoLine(const int no);
    void moveCursor(const bool end);
    void findInTextEdit(const QString &search, const QTextDocument::FindFlags &find_options);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dropEvent(QDropEvent *e) override;

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
    ~LineNumberArea();
    QSize sizeHint() const override;

protected:
    void leaveEvent(QEvent *event) override;
    void mouseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    PlainTextEdit *m_Edit;
};

#endif // SOURCECODEEDIT_H
