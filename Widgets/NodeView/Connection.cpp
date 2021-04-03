// #include "ConnectionPainter.h"
#include "NodeScene.h"
#include "Connection.h"

Connection::Connection(NodeScene *sc) : scene(sc) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    scene->addItem(this);
    // setAcceptHoverEvents(true);
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::gray, 2));
    painter->setBrush(Qt::blue);

    QPainterPath path(startPos);
    path.lineTo(currentPos);

    painter->drawPath(path);

    // QGraphicsObject::mouseMoveEvent(event);
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(touchedPort) {
        draw = false;
    }
    QGraphicsObject::mousePressEvent(event);
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    currentPos = event->pos();

    QGraphicsObject::mouseMoveEvent(event);
}
