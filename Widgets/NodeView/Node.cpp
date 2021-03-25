#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include "NodeScene.h"
#include "NodePainter.h"

#include "Node.h"
#include "Connection.h"

Node::Node(NodeScene *scene) : _scene(*scene) {
    _scene.addItem(this);

    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    auto const &nodeStyle = node.nodeDataModel()->nodeStyle();

    {
        auto effect = new QGraphicsDropShadowEffect;
        effect->setOffset(4, 4);
        effect->setBlurRadius(20);
        effect->setColor(nodeStyle.ShadowColor);

        setGraphicsEffect(effect);
    }

    setOpacity(nodeStyle.Opacity);

    setAcceptHoverEvents(true);

    setZValue(0);

    embedQWidget();

    // connect to the move signals to emit the move signals in FlowScene
    auto onMoveSlot = [this] {
        _scene.nodeMoved(*this, pos());
    };
    connect(this, &QGraphicsObject::xChanged, this, onMoveSlot);
    connect(this, &QGraphicsObject::yChanged, this, onMoveSlot);


    recalculateSize();

    // propagate data: model => node
    connect(_nodeDataModel.get(), &NodeDataModel::dataUpdated,
            this, &Node::onDataUpdated);

    connect(_nodeDataModel.get(), &NodeDataModel::embeddedWidgetSizeUpdated,
            this, &Node::onNodeSizeUpdated);
}

Node::~Node() {
    _scene.removeItem(this);
}


void Node::embedQWidget() {
    NodeGeometry &geom = _node.nodeGeometry();

    if (auto w = _node.nodeDataModel()->embeddedWidget()) {
        _proxyWidget = new QGraphicsProxyWidget(this);

        _proxyWidget->setWidget(w);

        _proxyWidget->setPreferredWidth(5);

        recalculateSize();

        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag) {
            // If the widget wants to use as much vertical space as possible, set it to have the geom's equivalentWidgetHeight.
            _proxyWidget->setMinimumHeight(geom.equivalentWidgetHeight());
        }

        _proxyWidget->setPos(geom.widgetPosition());

        update();

        _proxyWidget->setOpacity(1.0);
        _proxyWidget->setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
    }
}

void Node::setGeometryChanged() {
    prepareGeometryChange();
}


void Node::moveConnections() const {
    for (PortType portType: {PortType::In, PortType::Out}) {
        auto const &connectionEntries = getEntries(portType);

        for (auto const &connections : connectionEntries) {
            for (auto &con : connections)
                con.second->move();
        }
    }
}

void Node::lock(bool locked) {
    _locked = locked;

    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    painter->setClipRect(option->exposedRect);
    NodePainter::paint(painter, this, &_scene);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        moveConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (_locked)
        return;

    // deselect all other items after this one is selected
    if (!isSelected() &&
        !(event->modifiers() & Qt::ControlModifier)) {
        _scene.clearSelection();
    }

    for (PortType portToCheck: {PortType::In, PortType::Out}) {
        // TODO do not pass sceneTransform
        int const portIndex = checkHitScenePoint(portToCheck, event->scenePos(), sceneTransform());

        if (portIndex != INVALID) {
            std::unordered_map<QUuid, Connection *> connections = connections(portToCheck, portIndex);

            // start dragging existing connection
            if (!connections.empty() && portToCheck == PortType::In) {
                auto con = connections.begin()->second;

                NodeConnectionInteraction interaction(_node, *con, _scene);

                interaction.disconnect(portToCheck);
            }
                // initialize new Connection
            else {
                if (portToCheck == PortType::Out) {
                    auto const outPolicy = _node.nodeDataModel()->portOutConnectionPolicy(portIndex);
                    if (!connections.empty() &&
                        outPolicy == NodeDataModel::ConnectionPolicy::One) {
                        _scene.deleteConnection(*connections.begin()->second);
                    }
                }

                // todo add to FlowScene
                auto connection = _scene.createConnection(portToCheck, *this, portIndex);
                setConnection(portToCheck, portIndex, *connection);
                connection->grabMouse();
            }
        }
    }

    auto pos = event->pos();
    auto &geom = _node.nodeGeometry();
    auto &state = _node.nodeState();

    if (_node.nodeDataModel()->resizable() &&
        geom.resizeRect().contains(QPoint(pos.x(),
                                          pos.y()))) {
        _resizing = true;
    }
}


void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto &geom = _node.nodeGeometry();
    auto &state = _node.nodeState();

    if (_resizing) {
        auto diff = event->pos() - event->lastPos();

        if (auto w = _node.nodeDataModel()->embeddedWidget()) {
            prepareGeometryChange();
            auto oldSize = w->size();
            oldSize += QSize(diff.x(), diff.y());
            w->setFixedSize(oldSize);

            _proxyWidget->setMinimumSize(oldSize);
            _proxyWidget->setMaximumSize(oldSize);
            _proxyWidget->setPos(geom.widgetPosition());

            recalculateSize();
            update();
            moveConnections();
            event->accept();
        }
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        if (event->lastPos() != event->pos())
            moveConnections();
        event->ignore();
    }

    QRectF r = scene()->sceneRect();
    r = r.united(mapToScene(boundingRect()).boundingRect());
    scene()->setSceneRect(r);
}


void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    _resizing = false;
    QGraphicsObject::mouseReleaseEvent(event);
    // position connections precisely after fast node move
    moveConnections();
}


void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    // bring all the colliding nodes to background
    QList<QGraphicsItem *> overlapItems = collidingItems();

    for (QGraphicsItem *item : overlapItems) {
        if (item->zValue() > 0.0) {
            item->setZValue(0.0);
        }
    }

    // bring this node forward
    setZValue(1.0);

    _hovered = true;
    update();
    _scene.nodeHovered(*this, event->screenPos());
    event->accept();
}


void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    _hovered = false;
    update();
    _scene.nodeHoverLeft(*this);
    event->accept();
}


void Node::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    auto pos = event->pos();
    auto &geom = _node.nodeGeometry();

    if (_node.nodeDataModel()->resizable() && geom.resizeRect().contains(QPoint(pos.x(), pos.y()))) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    } else {
        setCursor(QCursor());
    }
    event->accept();
}


void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseDoubleClickEvent(event);

    _scene.nodeDoubleClicked(*this);
}


void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    _scene.nodeContextMenu(*this, mapToScene(event->pos()));
}




// Actual Node

void Node::reactToPossibleConnection(PortType reactingPortType,
                                     NodeDataType const &reactingDataType,
                                     QPointF const &scenePoint) {
    QTransform const t = sceneTransform();
    _draggingPos = t.inverted().map(scenePoint);
    update();
    setReaction(Node::REACTING, reactingPortType, reactingDataType);
}


void Node::resetReactionToConnection() {
    setReaction(Node::NOT_REACTING);
    update();
}

void Node::propagateData(std::shared_ptr<NodeData> nodeData, int inPortIndex) const {
    _nodeDataModel->setInData(std::move(nodeData), inPortIndex);

    //Recalculate the nodes visuals. A data change can result in the node taking more space than before, so this forces a recalculate+repaint on the affected node
    _nodeGraphicsObject->setGeometryChanged();
    _nodeGeometry.recalculateSize();
    _nodeGraphicsObject->update();
    _nodeGraphicsObject->moveConnections();
}


void Node::onDataUpdated(int PortIndex) {
    auto nodeData = _nodeDataModel->outData(PortIndex);

    auto connections =
            _nodeState.connections(PortType::Out, PortIndex);

    for (auto const &c : connections)
        c.second->propagateData(nodeData);
}

void Node::onNodeSizeUpdated() {
    if (nodeDataModel()->embeddedWidget()) {
        nodeDataModel()->embeddedWidget()->adjustSize();
    }
    recalculateSize();
    for (PortType type: {PortType::In, PortType::Out}) {
        for (auto &conn_set : getEntries(type)) {
            for (auto &pair: conn_set) {
                Connection *conn = pair.second;
                conn->move();
            }
        }
    }
}









// Node State:

std::vector<Node::ConnectionPtrSet> const &Node::getEntries(PortType portType) const {
    if (portType == PortType::In)
        return _inConnections;
    else
        return _outConnections;
}


std::vector<Node::ConnectionPtrSet> &Node::getEntries(PortType portType) {
    if (portType == PortType::In)
        return _inConnections;
    else
        return _outConnections;
}


Node::ConnectionPtrSet Node::connections(PortType portType, int portIndex) const {
    auto const &connections = getEntries(portType);

    return connections[portIndex];
}


void Node::setConnection(PortType portType, int portIndex, Connection &connection) {
    auto &connections = getEntries(portType);
    //connections.at(portIndex).insert(std::make_pair(connection.id(),
    //                                             &connection));
}


void Node::eraseConnection(PortType portType, int portIndex, QUuid id) {
    getEntries(portType)[portIndex].erase(id);
}


void Node::setReaction(ReactToConnectionState reaction, PortType reactingPortType,
                       NodeDataType reactingDataType) {
    _reaction = reaction;
    _reactingPortType = reactingPortType;
    _reactingDataType = std::move(reactingDataType);
}




// Node Geometry

QRectF Node::entryBoundingRect() const {
    double const addon = 0.0;

    return QRectF(0 - addon,
                  0 - addon,
                  _entryWidth + 2 * addon,
                  _entryHeight + 2 * addon);
}


QRectF Node::boundingRect() const {
    auto const &nodeStyle = StyleCollection::nodeStyle();
    double addon = 4 * nodeStyle.ConnectionPointDiameter;
    return QRectF(0 - addon,
                  0 - addon,
                  _width + 2 * addon,
                  _height + 2 * addon);
}


void Node::recalculateSize() {
    _entryHeight = _fontMetrics.height();

    {
        unsigned int maxNumOfEntries = std::max(_nSinks, _nSources);
        unsigned int step = _entryHeight + _spacing;
        _height = step * maxNumOfEntries;
    }

    if (auto w = _dataModel->embeddedWidget()) {
        _height = std::max(_height, static_cast<unsigned>(w->height()));
    }

    _height += captionHeight();

    _inputPortWidth = portWidth(PortType::In);
    _outputPortWidth = portWidth(PortType::Out);

    _width = _inputPortWidth +
             _outputPortWidth +
             2 * _spacing;

    if (auto w = _dataModel->embeddedWidget()) {
        _width += w->width();
    }

    _width = std::max(_width, captionWidth());

    if (_dataModel->validationState() != NodeValidationState::Valid) {
        _width = std::max(_width, validationWidth());
        _height += validationHeight() + _spacing;
    }
}

void Node::recalculateSize(QFont const &font) {
    QFontMetrics fontMetrics(font);
    QFont boldFont = font;
    boldFont.setBold(true);
    QFontMetrics boldFontMetrics(boldFont);

    if (_boldFontMetrics != boldFontMetrics) {
        _fontMetrics = fontMetrics;
        _boldFontMetrics = boldFontMetrics;
        recalculateSize();
    }
}

QPointF Node::portScenePosition(int portIndex, PortType portType, QTransform const &t) const {
    auto const &nodeStyle = StyleCollection::nodeStyle();
    const unsigned int step = _entryHeight + _spacing;
    QPointF result;
    double totalHeight = 0.0;
    totalHeight += captionHeight();
    totalHeight += step * portIndex;

    // TODO: why?
    totalHeight += step / 2.0;

    switch (portType) {
        case PortType::Out: {
            double x = _width + nodeStyle.ConnectionPointDiameter;
            result = QPointF(x, totalHeight);
            break;
        }
        case PortType::In: {
            double x = 0.0 - nodeStyle.ConnectionPointDiameter;
            result = QPointF(x, totalHeight);
            break;
        }

        default:
            break;
    }

    return t.map(result);
}


int Node::checkHitScenePoint(PortType portType,
                             QPointF const scenePoint,
                             QTransform const &sceneTransform) const {
    auto const &nodeStyle = StyleCollection::nodeStyle();
    // PortIndex
    int result = INVALID;
    if (portType == PortType::None)
        return result;

    double const tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;
    unsigned int const nItems = nPorts(portType);

    for (unsigned int i = 0; i < nItems; ++i) {
        auto pp = portScenePosition(i, portType, sceneTransform);
        QPointF p = pp - scenePoint;
        auto distance = std::sqrt(QPointF::dotProduct(p, p));
        if (distance < tolerance) {
            result = PortIndex(i);
            break;
        }
    }

    return result;
}


QRect Node::resizeRect() const {
    unsigned int rectSize = 7;

    return QRect(_width - rectSize,
                 _height - rectSize,
                 rectSize,
                 rectSize);
}


QPointF Node::widgetPosition() const {
    if (auto w = _dataModel->embeddedWidget()) {
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag) {
            // If the widget wants to use as much vertical space as possible, place it immediately after the caption.
            return QPointF(_spacing + portWidth(PortType::In), captionHeight());
        } else {
            if (_dataModel->validationState() != NodeValidationState::Valid) {
                return QPointF(_spacing + portWidth(PortType::In),
                               (captionHeight() + _height - validationHeight() - _spacing - w->height()) / 2.0);
            }

            return QPointF(_spacing + portWidth(PortType::In),
                           (captionHeight() + _height - w->height()) / 2.0);
        }
    }
    return QPointF();
}

int Node::equivalentWidgetHeight() const {
    if (_dataModel->validationState() != NodeValidationState::Valid) {
        return height() - captionHeight() + validationHeight();
    }

    return height() - captionHeight();
}

unsigned int Node::captionHeight() const {
    if (!_dataModel->captionVisible())
        return 0;

    QString name = _dataModel->caption();

    return _boldFontMetrics.boundingRect(name).height();
}


unsigned int Node::captionWidth() const {
    if (!dataModel->captionVisible())
        return 0;

    QString name = _dataModel->caption();

    return _boldFontMetrics.boundingRect(name).width();
}


unsigned int Node::validationHeight() const {
    QString msg = _dataModel->validationMessage();

    return _boldFontMetrics.boundingRect(msg).height();
}


unsigned int Node::validationWidth() const {
    QString msg = _dataModel->validationMessage();

    return _boldFontMetrics.boundingRect(msg).width();
}

QPointF Node::calculateNodePositionBetweenNodePorts(int targetPortIndex, PortType targetPort, Node *targetNode,
                                                    int sourcePortIndex, PortType sourcePort, Node *sourceNode,
                                                    Node &newNode) {
    //Calculating the nodes position in the scene. It'll be positioned half way between the two ports that it "connects".
    //The first line calculates the halfway point between the ports (node position + port position on the node for both nodes averaged).
    //The second line offsets this coordinate with the size of the new node, so that the new nodes center falls on the originally
    //calculated coordinate, instead of it's upper left corner.
    QPointF converterNodePos = (sourceNode->pos() + sourceNode->portScenePosition(sourcePortIndex, sourcePort) +
                                targetNode->pos() + targetNode->portScenePosition(targetPortIndex, targetPort)) / 2.0f;

    converterNodePos.setX(converterNodePos.x() - newNode._width / 2.0f);
    converterNodePos.setY(converterNodePos.y() - newNode._height / 2.0f);
    return converterNodePos;
}

unsigned int Node::portWidth(PortType portType) const {
    unsigned int width = 0;
    for (auto i = 0ul; i < nPorts(portType); ++i) {
        QString name;

        if (_dataModel->portCaptionVisible(portType, i)) {
            name = _dataModel->portCaption(portType, i);
        } else {
            name = _dataModel->dataType(portType, i).name;
        }
        width = std::max(unsigned(_fontMetrics.width(name)), width);
    }
    return width;
}












