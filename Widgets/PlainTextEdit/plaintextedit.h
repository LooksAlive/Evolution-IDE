#ifndef SOURCECODEEDIT_H
#define SOURCECODEEDIT_H

#include <QPlainTextEdit>
#include <QDebug>

#include "completer.h"

class LineNumberArea;

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
    void find(const QString &search);
    void findNext(const QString &search, const QTextDocument::FindFlags &find_options);
    void replace(const QString &oldText, const QString &newText);
    void replaceAndFind(const QString &oldText, const QString &newText);
    int replaceAll(const QString &oldText, const QString &newText);

    // cursor
    void setCursorPosition(int lineNumber, int columnNumber);
    int getCursorPosition();
    void setCursorAtLine(const int &line);

    // text manipulation
    void selectLineUnderCursor();
    QString getLineUnderCursor();
    void selectWordUnderCursor();
    QString getWordUnderCursor();
    // missing selected text for return -> no use for now
    void deleteLine();
    void highlight(QList<QTextEdit::ExtraSelection> &selections, const bool &Background,
                   const QColor &color = QColor::fromRgb(0, 255, 0));
    void toggleComment();

    // other
    void setFileExtension(const QString &extension = "cpp");

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

    QString file_extension;

private slots:
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
