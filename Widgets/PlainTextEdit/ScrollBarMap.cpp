#include "plaintextedit.h"

#include "ScrollBarMap.h"


ScrollBarMap::ScrollBarMap(PlainTextEdit *edit, QWidget *parent) : QWidget(parent), m_Edit(edit) {
    smallEdit = new SmallRoundedEdit(edit);


}


void ScrollBarMap::paintEvent(QPaintEvent *event) {

}

void ScrollBarMap::mousePressEvent(QMouseEvent *event) {

}

void ScrollBarMap::leaveEvent(QEvent *event) {
    hide();
    m_Edit->verticalScrollBar()->show();
    QWidget::leaveEvent(event);
}

void ScrollBarMap::mouseMoveEvent(QMouseEvent *event) {

}
