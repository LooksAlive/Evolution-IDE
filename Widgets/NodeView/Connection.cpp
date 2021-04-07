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

    painter->setPen(QPen(Qt::black, 8));
    painter->setBrush(Qt::blue);

    const QPointF n1Pos = node1->scenePos();
    QPainterPath path(QPointF(n1Pos.x() + port1Pos.x(), n1Pos.y() + port1Pos.y()));
    if(node2) {
        const QPointF n2Pos = node2->scenePos();
        path.lineTo(QPointF(n2Pos.x() + port2Pos.x(), n2Pos.y() + port2Pos.y()));
    }
    else {
        path.lineTo(scene->currentPos);
    }

    painter->drawPath(path);

    // QGraphicsObject::mouseMoveEvent(event);
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    //if(touchedPort) {
    //    draw = false;
    //}
    QGraphicsObject::mousePressEvent(event);
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsObject::mouseMoveEvent(event);
}
QPainterPath Connection::shape() const {
    const QPointF n1Pos = node1->scenePos();
    QPainterPath path(QPointF(n1Pos.x() + port1Pos.x(), n1Pos.y() + port1Pos.y()));
    if(node2) {
        const QPointF n2Pos = node2->scenePos();
        path.lineTo(QPointF(n2Pos.x() + port2Pos.x(), n2Pos.y() + port2Pos.y()));
    }
    else {
        path.lineTo(scene->currentPos);
    }

    QPainterPathStroker stroker;
    stroker.setWidth(8);

    return stroker.createStroke(path);
}
