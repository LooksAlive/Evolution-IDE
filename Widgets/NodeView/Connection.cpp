
#include "ConnectionPainter.h"

#include "Connection.h"

Connection::Connection(NodeScene *scene) : _scene(*scene) {
    _scene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    // addGraphicsEffect();

    setZValue(-1.0);

    resetLastHoveredNode();
}

Connection::~Connection() {
    // Connection
    if (complete()) connectionMadeIncomplete(*this);
    propagateEmptyData();

    if (_inNode) {
        _inNode->nodeGraphicsObject().update();
    }

    if (_outNode) {
        _outNode->nodeGraphicsObject().update();
    }


    _scene.removeItem(this);
}

QRectF Connection::boundingRect() const {
    return _connection->boundingRect();
}


QPainterPath Connection::shape() const {
#ifdef DEBUG_DRAWING

    //QPainterPath path;

    //path.addRect(boundingRect());
    //return path;

#else
    auto const &geom =
            _connection.connectionGeometry();

    return ConnectionPainter::getPainterStroke(geom);

#endif
}


void Connection::setGeometryChanged() {
    prepareGeometryChange();
}


void Connection::move() {
    for (PortType portType: {PortType::In, PortType::Out}) {
        if (auto node = _connection.getNode(portType)) {
            auto const &nodeGraphics = node->nodeGraphicsObject();

            auto const &nodeGeom = node->nodeGeometry();

            QPointF scenePos =
                    nodeGeom.portScenePosition(_connection.getPortIndex(portType),
                                               portType,
                                               nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            _connection.connectionGeometry().setEndPoint(portType,
                                                         connectionPos);

            _connection.getConnectionGraphicsObject().setGeometryChanged();
            _connection.getConnectionGraphicsObject().update();
        }
    }

}

void Connection::lock(bool locked) {
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}


void Connection::paint(QPainter *painter,
                       QStyleOptionGraphicsItem const *option,
                       QWidget *) {
    painter->setClipRect(option->exposedRect);

    ConnectionPainter::paint(painter,
                             _connection);
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    //event->ignore();
}


void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    prepareGeometryChange();

    auto view = reinterpret_cast<QGraphicsView *>(event->widget());
    auto node = locateNodeAt(event->scenePos(),
                             _scene,
                             view->transform());

    auto &state = _connection.connectionState();

    state.interactWithNode(node);
    if (node) {
        node->reactToPossibleConnection(state.requiredPort(),
                                        _connection.dataType(oppositePort(state.requiredPort())),
                                        event->scenePos());
    }

    //-------------------

    QPointF offset = event->pos() - event->lastPos();

    auto requiredPort = _connection.requiredPort();

    if (requiredPort != PortType::None) {
        _connection.connectionGeometry().moveEndPoint(requiredPort, offset);
    }

    //-------------------

    update();

    event->accept();
}


void Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    ungrabMouse();
    event->accept();

    auto node = locateNodeAt(event->scenePos(), _scene,
                             _scene.views()[0]->transform());

    NodeConnectionInteraction interaction(*node, _connection, _scene);

    if (node && interaction.tryConnect()) {
        node->resetReactionToConnection();
    }

    if (_connection.connectionState().requiresPort()) {
        _scene.deleteConnection(_connection);
    }
}


void Connection::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    _connection.connectionGeometry().setHovered(true);

    update();
    _scene.connectionHovered(connection(), event->screenPos());
    event->accept();
}


void Connection::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    _connection.connectionGeometry().setHovered(false);

    update();
    _scene.connectionHoverLeft(connection());
    event->accept();
}


void Connection::addGraphicsEffect() {
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    //auto effect = new QGraphicsDropShadowEffect;
    //auto effect = new ConnectionBlurEffect(this);
    //effect->setOffset(4, 4);
    //effect->setColor(QColor(Qt::gray).darker(800));
}





// Connection

void Connection::newConnection(Node::PortType portType, Node &node, int portIndex) {
    _uid = QUuid::createUuid();
    _outPortIndex = Node::INVALID;
    _inPortIndex = Node::INVALID;
    // _connectionState();

    setNodeToPort(node, portType, portIndex);
    setRequiredPort(oppositePort(portType));
}

void Connection::newConnection(Node &nodeIn, int portIndexIn, Node &nodeOut, int portIndexOut) {
    _uid = QUuid::createUuid();
    _outNode = &nodeOut;
    _inNode = &nodeIn;
    _outPortIndex = portIndexOut;
    _inPortIndex = portIndexIn;

    setNodeToPort(nodeIn, Node::PortType::In, portIndexIn);
    setNodeToPort(nodeOut, Node::PortType::Out, portIndexOut);
}

bool
Connection::
complete() const {
    return _inNode != nullptr && _outNode != nullptr;
}


void
Connection::
setRequiredPort(Node::PortType dragging) {
    _requiredPort = dragging;

    switch (dragging) {
        case Node::PortType::Out:
            _outNode = nullptr;
            _outPortIndex = Node::INVALID;
            break;

        case Node::PortType::In:
            _inNode = nullptr;
            _inPortIndex = Node::INVALID;
            break;

        default:
            break;
    }
}

void
Connection::
setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject> &&graphics) {
    _connectionGraphicsObject = std::move(graphics);

    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in Connection G.O. coordinates. The position of the whole
    // Connection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (_requiredPort != Node::PortType::None) {

        Node::PortType attachedPort = oppositePort(requiredPort());

        // PortIndex
        int attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform =
                node->nodeGraphicsObject().sceneTransform();

        QPointF pos = node->nodeGeometry().portScenePosition(attachedPortIndex,
                                                             attachedPort,
                                                             nodeSceneTransform);

        _connectionGraphicsObject->setPos(pos);
    }

    _connectionGraphicsObject->move();
}


int
Connection::
getPortIndex(PortType portType) const {
    PortIndex result = INVALID;

    switch (portType) {
        case PortType::In:
            result = _inPortIndex;
            break;

        case PortType::Out:
            result = _outPortIndex;

            break;

        default:
            break;
    }

    return result;
}


void
Connection::
setNodeToPort(Node &node,
              PortType portType,
              PortIndex portIndex) {
    bool wasIncomplete = !complete();

    auto &nodeWeak = getNode(portType);

    nodeWeak = &node;

    if (portType == PortType::Out)
        _outPortIndex = portIndex;
    else
        _inPortIndex = portIndex;

    _connectionState.setNoRequiredPort();

    updated(*this);
    if (complete() && wasIncomplete) {
        connectionCompleted(*this);
    }
}


void
Connection::
removeFromNodes() const {
    if (_inNode)
        _inNode->nodeState().eraseConnection(PortType::In, _inPortIndex, id());

    if (_outNode)
        _outNode->nodeState().eraseConnection(PortType::Out, _outPortIndex, id());
}

void
Connection::
clearNode(PortType portType) {
    if (complete()) {
        connectionMadeIncomplete(*this);
    }

    getNode(portType) = nullptr;

    if (portType == PortType::In)
        _inPortIndex = INVALID;
    else
        _outPortIndex = INVALID;
}


NodeDataType
Connection::
dataType(PortType portType) const {
    if (_inNode && _outNode) {
        auto const &model = (portType == PortType::In) ?
                            _inNode->nodeDataModel() :
                            _outNode->nodeDataModel();
        PortIndex index = (portType == PortType::In) ?
                          _inPortIndex :
                          _outPortIndex;

        return model->dataType(portType, index);
    } else {
        Node *validNode;
        PortIndex index = INVALID;

        if ((validNode = _inNode)) {
            index = _inPortIndex;
            portType = PortType::In;
        } else if ((validNode = _outNode)) {
            index = _outPortIndex;
            portType = PortType::Out;
        }

        if (validNode) {
            auto const &model = validNode->nodeDataModel();

            return model->dataType(portType, index);
        }
    }

    Q_UNREACHABLE();
}

void
Connection::
propagateData(std::shared_ptr<NodeData> nodeData) const {
    if (_inNode) {
        if (_converter) {
            nodeData = _converter(nodeData);
        }

        _inNode->propagateData(nodeData, _inPortIndex);
    }
}


void
Connection::
propagateEmptyData() const {
    std::shared_ptr<NodeData> emptyData;

    propagateData(emptyData);
}










// Node Connection Interaction

bool
Connection::
canConnect(PortIndex &portIndex, TypeConverter &converter) const {
    // 1) Connection requires a port

    PortType requiredPort = connectionRequiredPort();


    if (requiredPort == PortType::None) {
        return false;
    }

    // 1.5) Forbid connecting the node to itself
    Node *node = _connection->getNode(oppositePort(requiredPort));

    if (node == _node)
        return false;

    // 2) connection point is on top of the node port

    QPointF connectionPoint = connectionEndScenePosition(requiredPort);

    portIndex = nodePortIndexUnderScenePoint(requiredPort,
                                             connectionPoint);

    if (portIndex == INVALID) {
        return false;
    }

    // 3) Node port is vacant

    // port should be empty
    if (!nodePortIsEmpty(requiredPort, portIndex))
        return false;

    // 4) Connection type equals node port type, or there is a registered type conversion that can translate between the two

    auto connectionDataType =
            _connection->dataType(oppositePort(requiredPort));

    auto const &modelTarget = _node->nodeDataModel();
    NodeDataType candidateNodeDataType = modelTarget->dataType(requiredPort, portIndex);

    if (connectionDataType.id != candidateNodeDataType.id) {
        if (requiredPort == PortType::In) {
            converter = _scene->registry().getTypeConverter(connectionDataType, candidateNodeDataType);
        } else if (requiredPort == PortType::Out) {
            converter = _scene->registry().getTypeConverter(candidateNodeDataType, connectionDataType);
        }

        return (converter != nullptr);
    }

    return true;
}


bool
Connection::
tryConnect() const {
    // 1) Check conditions from 'canConnect'
    PortIndex portIndex = INVALID;

    TypeConverter converter;

    if (!canConnect(portIndex, converter)) {
        return false;
    }

    // 1.5) If the connection is possible but a type conversion is needed,
    //      assign a convertor to connection
    if (converter) {
        _connection->setTypeConverter(converter);
    }

    // 2) Assign node to required port in Connection
    PortType requiredPort = connectionRequiredPort();
    _node->nodeState().setConnection(requiredPort,
                                     portIndex,
                                     *_connection);

    // 3) Assign Connection to empty port in NodeState
    // The port is not longer required after this function
    _connection->setNodeToPort(*_node, requiredPort, portIndex);

    // 4) Adjust Connection geometry

    _node->nodeGraphicsObject().moveConnections();

    // 5) Poke model to intiate data transfer

    auto outNode = _connection->getNode(PortType::Out);
    if (outNode) {
        PortIndex outPortIndex = _connection->getPortIndex(PortType::Out);
        outNode->onDataUpdated(outPortIndex);
    }

    return true;
}


/// 1) Node and Connection should be already connected
/// 2) If so, clear Connection entry in the NodeState
/// 3) Set Connection end to 'requiring a port'
bool
Connection::
disconnect(PortType portToDisconnect) const {
    PortIndex portIndex =
            _connection->getPortIndex(portToDisconnect);

    NodeState &state = _node->nodeState();

    // clear pointer to Connection in the NodeState
    state.getEntries(portToDisconnect)[portIndex].clear();

    // 4) Propagate invalid data to IN node
    _connection->propagateEmptyData();

    // clear Connection side
    _connection->clearNode(portToDisconnect);

    _connection->setRequiredPort(portToDisconnect);

    _connection->getConnectionGraphicsObject().grabMouse();

    return true;
}


// ------------------ util functions below

PortType
Connection::
connectionRequiredPort() const {
    auto const &state = _connection->connectionState();

    return state.requiredPort();
}


QPointF
Connection::
connectionEndScenePosition(PortType portType) const {
    auto &go =
            _connection->getConnectionGraphicsObject();

    ConnectionGeometry &geometry = _connection->connectionGeometry();

    QPointF endPoint = geometry.getEndPoint(portType);

    return go.mapToScene(endPoint);
}


QPointF
Connection::
nodePortScenePosition(PortType portType, PortIndex portIndex) const {
    NodeGeometry const &geom = _node->nodeGeometry();

    QPointF p = geom.portScenePosition(portIndex, portType);

    NodeGraphicsObject &ngo = _node->nodeGraphicsObject();

    return ngo.sceneTransform().map(p);
}


PortIndex
Connection::
nodePortIndexUnderScenePoint(PortType portType,
                             QPointF const &scenePoint) const {
    NodeGeometry const &nodeGeom = _node->nodeGeometry();

    QTransform sceneTransform =
            _node->nodeGraphicsObject().sceneTransform();

    PortIndex portIndex = nodeGeom.checkHitScenePoint(portType,
                                                      scenePoint,
                                                      sceneTransform);
    return portIndex;
}


bool
Connection::
nodePortIsEmpty(PortType portType, PortIndex portIndex) const {
    NodeState const &nodeState = _node->nodeState();

    auto const &entries = nodeState.getEntries(portType);

    if (entries[portIndex].empty()) return true;

    const auto outPolicy = _node->nodeDataModel()->portOutConnectionPolicy(portIndex);
    return (portType == PortType::Out && outPolicy == NodeDataModel::ConnectionPolicy::Many);
}






// Connection State

void Connection::interactWithNode(Node *node) {
    if (node) {
        _lastHoveredNode = node;
    } else {
        resetLastHoveredNode();
    }
}

void Connection::setLastHoveredNode(Node *node) {
    _lastHoveredNode = node;
}

void Connection::resetLastHoveredNode() {
    if (_lastHoveredNode)
        _lastHoveredNode->resetReactionToConnection();

    _lastHoveredNode = nullptr;
}





// Connection Geometry

QPointF const &Connection::getEndPoint(Node::PortType portType) const {
    Q_ASSERT(portType != Node::PortType::None);

    return (portType == Node::PortType::Out ?
            _out :
            _in);
}


void Connection::setEndPoint(Node::PortType portType, QPointF const &point) {
    switch (portType) {
        case Node::PortType::Out:
            _out = point;
            break;

        case Node::PortType::In:
            _in = point;
            break;

        default:
            break;
    }
}


void Connection::moveEndPoint(Node::PortType portType, QPointF const &offset) {
    switch (portType) {
        case Node::PortType::Out:
            _out += offset;
            break;

        case Node::PortType::In:
            _in += offset;
            break;

        default:
            break;
    }
}


QRectF Connection::boundingRect() const {
    auto points = pointsC1C2();

    QRectF basicRect = QRectF(_out, _in).normalized();

    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    auto const &connectionStyle =
            StyleCollection::connectionStyle();

    float const diam = connectionStyle.pointDiameter();

    QRectF commonRect = basicRect.united(c1c2Rect);

    QPointF const cornerOffset(diam, diam);

    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}


std::pair<QPointF, QPointF> Connection::pointsC1C2() const {
    const double defaultOffset = 200;

    double xDistance = _in.x() - _out.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

    double verticalOffset = 0;

    double ratioX = 0.5;

    if (xDistance <= 0) {
        double yDistance = _in.y() - _out.y() + 20;

        double vector = yDistance < 0 ? -1.0 : 1.0;

        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(_out.x() + horizontalOffset,
               _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset,
               _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}




// Style

QColor Connection::normalColor(const QString& typeId) const {
  std::size_t hash = qHash(typeId);

  std::size_t const hue_range = 0xFF;

  qsrand(hash);
  std::size_t hue = qrand() % hue_range;

  std::size_t sat = 120 + hash % 129;

  return QColor::fromHsl(hue,
                         sat,
                         160);
}
