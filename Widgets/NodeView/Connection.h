#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsObject>
#include <QGraphicsBlurEffect>


#include "NodeScene.h"

class Connection : public QGraphicsObject {
Q_OBJECT
public:
    explicit Connection(NodeScene *scene = nullptr);

    ~Connection();

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void
    setGeometryChanged();

    /// Updates the position of both ends
    void move();

    void lock(bool locked);

protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = 0) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    NodeScene &_scene;

    void
    addGraphicsEffect();




    // Connection

public:
    /// New Connection is attached to the port of the given Node.
    /// The port has parameters (portType, portIndex).
    /// The opposite connection end will require anothre port.
    void newConnection(Node::PortType portType,
                       Node &node,
                       int portIndex);

    void newConnection(Node &nodeIn, int portIndexIn, Node &nodeOut, int portIndexOut);

public:
    /// Remembers the end being dragged.
    /// Invalidates Node address.
    /// Grabs mouse.
    void setRequiredPort(Node::PortType portType);

    /// Assigns a node to the required port.
    /// It is assumed that there is a required port, no extra checks
    void setNodeToPort(Node &node,
                       Node::PortType portType,
                       int portIndex);

    void removeFromNodes() const;

public:
    int getPortIndex(PortType portType) const;

    void
    clearNode(PortType portType);

    NodeDataType
    dataType(PortType portType) const;

    bool complete() const;

public: // data propagation
    void propagateData(std::shared_ptr<NodeData> nodeData) const;

    void propagateEmptyData() const;

signals:

    void connectionCompleted(Connection const &) const;

    void connectionMadeIncomplete(Connection const &) const;

private:
    QUuid _uid;

private:
    Node *_outNode = nullptr;
    Node *_inNode = nullptr;

    int _outPortIndex;
    int _inPortIndex;

private:
    TypeConverter _converter;

signals:

    void updated(Connection &conn) const;






    // Node Connection Interaction

    // Class performs various operations on the Node and Connection pair.
    // An instance should be created on the stack and destroyed when
    // the operation is completed
public:
    /// Can connect when following conditions are met:
    /// 1) Connection 'requires' a port
    /// 2) Connection's vacant end is above the node port
    /// 3) Node port is vacant
    /// 4) Connection type equals node port type, or there is a registered type conversion that can translate between the two
    bool canConnect(int &portIndex,
                    TypeConverter &converter) const;

    /// 1)   Check conditions from 'canConnect'
    /// 1.5) If the connection is possible but a type conversion is needed, add a converter node to the scene, and connect it properly
    /// 2)   Assign node to required port in Connection
    /// 3)   Assign Connection to empty port in NodeState
    /// 4)   Adjust Connection geometry
    /// 5)   Poke model to initiate data transfer
    bool tryConnect() const;


    /// 1) Node and Connection should be already connected
    /// 2) If so, clear Connection entry in the NodeState
    /// 3) Propagate invalid data to IN node
    /// 4) Set Connection end to 'requiring a port'
    bool disconnect(Node::PortType portToDisconnect) const;


private:
    Node::PortType connectionRequiredPort() const;

    QPointF connectionEndScenePosition(Node::PortType) const;

    QPointF nodePortScenePosition(Node::PortType portType,
                                  int portIndex) const;

    // returns portIndex
    int nodePortIndexUnderScenePoint(Node::PortType portType,
                                     QPointF const &p) const;

    bool nodePortIsEmpty(Node::PortType portType, int portIndex) const;

    Node *_node;
    Connection *_connection;




    // Connection State

    // Stores currently draggind end.
    // Remembers last hovered Node.
public:
    void interactWithNode(Node *node);

    void setLastHoveredNode(Node *node);

    void resetLastHoveredNode();

    bool requiresPort() const { return _requiredPort != Node::PortType::None; }

private:
    Node::PortType _requiredPort;
    Node *_lastHoveredNode{nullptr};



    // Connection Geomtetry

public:

    QPointF const &getEndPoint(Node::PortType portType) const;

    void setEndPoint(Node::PortType portType, QPointF const &point);

    void moveEndPoint(Node::PortType portType, QPointF const &offset);

    QRectF boundingRect() const;

    std::pair<QPointF, QPointF> pointsC1C2() const;

    QPointF source() const { return _out; }

    QPointF sink() const { return _in; }

    void setHovered(bool hovered) { _hovered = hovered; }

private:
    // local object coordinates
    QPointF _in{0, 0};
    QPointF _out{0, 0};

    //int _animationPhase;

    double _lineWidth{3.0};

    bool _hovered = false;


};


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


#endif // CONNECTION_H
