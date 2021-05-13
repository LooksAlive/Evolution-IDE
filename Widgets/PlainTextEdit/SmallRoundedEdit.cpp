#include "SmallRoundedEdit.h"


SmallRoundedEdit::SmallRoundedEdit(PlainTextEdit *edit) : m_Edit(edit) {
    setStyleSheet("border-radius: 15px; border: 3px solid gray; background-color: gray; margin: 0px; padding: 2px;");
    // background:transparent;      cause no background
    setVisible(false);
    setWordWrapMode(QTextOption::NoWrap);

    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}
/*
SmallRoundedEdit::SmallRoundedEdit(PlainTextEditExtra *edit) : editExtra(edit) {
    setStyleSheet("border-radius: 15px; border: 3px solid gray; background-color: gray;");
    // background:transparent;      cause no background
    setVisible(false);

    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}
*/
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
