#include "SmallRoundedEdit.h"


SmallRoundedEdit::SmallRoundedEdit(QWidget *parent) : QPlainTextEdit(parent) {
    setStyleSheet("border-radius: 15px; border: 2px solid gray; background-color: gray; margin: 0px; padding: 2px;");
    // background:transparent;      cause no background
    setVisible(false);
    setWordWrapMode(QTextOption::NoWrap);

    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
