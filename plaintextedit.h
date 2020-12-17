#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QRect>
#include <QPainter>
#include <QTextBlock>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPlainTextEdit>

#include <QCompleter>
#include <QLineEdit>


class PlainTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    PlainTextEdit(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QCompleter *Completer;
    QWidget *lineNumberArea;

    void GetCompletion();
};

#endif // CODEEDITOR_H
