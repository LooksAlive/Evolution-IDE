#include "SmallRoundedEdit.h"


SmallRoundedEdit::SmallRoundedEdit(PlainTextEdit *edit, QWidget *parent) : QPlainTextEdit(parent), m_Edit(edit) {
    setStyleSheet("border-radius: 15px; border: 3px solid gray; background-color: gray;");
    // background:transparent;      cause no background
    setVisible(false);

    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}

void SmallRoundedEdit::enterEvent(QEvent *event) {
    inWidget = true;
    QWidget::enterEvent(event);
}

void SmallRoundedEdit::leaveEvent(QEvent *event) {
    inWidget = false;
    hide();
    QWidget::leaveEvent(event);
}

void SmallRoundedEdit::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const int delta = event->delta();
        if (delta > 0) {
            timesAplified--;
        } else if (delta < 0) {
            timesAplified++;
        }
    } else {
        QWidget::wheelEvent(event);
    }
}
