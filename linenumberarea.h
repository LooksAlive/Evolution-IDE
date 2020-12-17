#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "plaintextedit.h"


class LineNumberArea : public QWidget {
    Q_OBJECT
public:
    LineNumberArea(PlainTextEdit *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    PlainTextEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
