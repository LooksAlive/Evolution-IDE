#ifndef CONNECTION_H
#define CONNECTION_H

#include "Node.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsObject>

class NodeScene;
class Node;

class Connection : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Connection(NodeScene *scene = nullptr);
    ~Connection() = default;

    // nodes we are connected to, to track its global scene position
    // and paint correctly
    void setFirstNode(Node* n1, const QPointF& p1) { node1 = n1; port1Pos = p1; }
    void setSecondNode(Node* n2, const QPointF& p2) { node2 = n2; port2Pos = p2; }

    // reset it while not connected to any node after click
    bool stableConnection = false;

protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = 0) override;
    // end of connection
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // updates coords
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF boundingRect() const override {
        return shape().boundingRect();
    }

    QPainterPath shape() const override;

private:
    NodeScene *scene;

    // node1 will be always set, but node2 could not, therefore we track mouse positions
    // these are dynamic ... moving when widgets in scene moves.-.

    Node* node1 = nullptr;
    QPointF port1Pos;
    Node* node2 = nullptr;
    QPointF port2Pos;


};

/*
class ConnectionBlurEffect : public QGraphicsBlurEffect {

public:
    ConnectionBlurEffect(ConnectionGraphicsObject *item);

    void draw(QPainter *painter) override {
        QGraphicsBlurEffect::draw(painter);

        //ConnectionPainter::paint(painter,
        //_object->connectionGeometry(),
        //_object->connectionState());

        //_item->paint(painter, nullptr, nullptr);
    }

private:
};
*/

#endif // CONNECTION_H
