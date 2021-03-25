#include "InformativeMessage.h"

InformativeMessage::InformativeMessage(QWidget *parent) : QWidget(parent) {
    setVisible(false);
    setStyleSheet("background-color:red; border: 1px solid black; padding: 2px;");

    // setMinimumSize(100, 35);
    auto *layout = new QHBoxLayout(this);
    label = new QLabel(this);
    layout->addWidget(label);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
}

void InformativeMessage::setMessage(const QString &message) {
    setMinimumWidth(fontMetrics().boundingRect(message).width() + 15);
    setMinimumHeight(fontMetrics().boundingRect(message).height() + 15);
    label->setText(message);
    Message = message;
}
/*
void InformativeMessage::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawText(QPointF(0, 0), Message);

    QWidget::paintEvent(event);
}
*/
