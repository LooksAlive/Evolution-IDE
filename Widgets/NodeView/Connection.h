#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsObject>
#include <QGraphicsBlurEffect>

class NodeScene;

class Connection : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Connection(NodeScene *scene = nullptr);
    ~Connection() = default;

    // drawing or is fixed ...  triggered only once
    // reset it while not connected to any node after click
    bool draw = true;
    bool touchedPort = false;

    // these are dynamic ... moving when widgets in scene moves.-.
    QPointF startPos;
    QPointF currentPos;


protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = 0) override;
    // end of connection
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // updates coords
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF boundingRect() const override {
        return boundingRect();
    }


private:
    NodeScene *scene;




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
