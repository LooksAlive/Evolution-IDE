#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsProxyWidget>
#include <QUuid>
#include <QFontMetrics>
#include <QTransform>


#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
//#include <GraphicsItemChange>


#include <iostream>
#include <cmath>


class NodeScene;

class Connection;

class NodePainter;


/*
 * new node is initialized for a scene and added to it
*/

class Node : public QGraphicsObject {
Q_OBJECT
public:
    explicit Node(NodeScene *scene = nullptr);     // scene
    ~Node();

    // Real Object

protected:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget = 0) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    void setGeometryChanged();

    /// Visits all attached connections and corrects
    /// their corresponding end points.
    void moveConnections() const;

    enum {
        Type = UserType + 1
    };

    int type() const override { return Type; }

    void lock(bool locked);


    // Port type
    static const int INVALID = -1;

    enum class PortType {
        None,
        In,
        Out
    };

private:
    void embedQWidget();

    NodeScene &_scene;
    bool _locked = false;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget *_proxyWidget = nullptr;


    // Actual Node

    enum ReactToConnectionState {
        REACTING,
        NOT_REACTING
    };

    struct NodeDataType {
        QString id;
        QString name;
    };

    enum class NodeValidationState {
        Valid,
        Warning,
        Error
    };


    struct Port {
        PortType type;
        int index;

        Port()
                : type(PortType::None), index(INVALID) {}

        Port(PortType type, int portIndex)
                : type(type), index(portIndex) {}

        bool indexIsValid() { return index != INVALID; }

        bool portTypeIsValid() { return type != PortType::None; }
    };

    PortType oppositePort(PortType port) {
        PortType result = PortType::None;
        switch (port) {
            case PortType::In:
                result = PortType::Out;
                break;
            case PortType::Out:
                result = PortType::In;
                break;

            default: /* None */
                break;
        }
        return result;
    }


    // Node Data model

    unsigned int nPorts(PortType portType) const;


public:
    void reactToPossibleConnection(PortType, NodeDataType const &, QPointF const &scenePoint);

    void resetReactionToConnection();

public slots: // data propagation
    /// Propagates incoming data to the underlying model.
    void
    propagateData(std::shared_ptr<NodeData> nodeData,
                  PortIndex inPortIndex) const;

    /// Fetches data from model's OUT #index port
    /// and propagates it to the connection
    void onDataUpdated(int PortIndex);

    /// update the graphic part if the size of the embeddedwidget changes
    void onNodeSizeUpdated();


private:
    // addressing
    QUuid _uid;





    // Node State:

public:
    // Contains vectors of connected input and output connections.
    // Stores bool for reacting on hovering connections
    using ConnectionPtrSet =
    std::unordered_map<QUuid, Connection *>;

    /// Returns vector of connections ID.
    /// Some of them can be empty (null)
    std::vector<ConnectionPtrSet> const &
    getEntries(PortType) const;

    std::vector<ConnectionPtrSet> &
    getEntries(PortType);

    ConnectionPtrSet connections(PortType portType, int portIndex) const;

    void setConnection(PortType portType,
                       int portIndex,
                       Connection &connection);

    void eraseConnection(PortType portType,
                         int portIndex,
                         QUuid id);

    void setReaction(ReactToConnectionState reaction,
                     PortType reactingPortType = PortType::None,
                     NodeDataType reactingDataType =
                     NodeDataType());

private:
    std::vector<ConnectionPtrSet> _inConnections;
    std::vector<ConnectionPtrSet> _outConnections;

    ReactToConnectionState _reaction;
    PortType _reactingPortType;
    NodeDataType _reactingDataType;

    bool _resizing;





    // Node Geometry ; all data from nodeModel

public:
    QRectF entryBoundingRect() const;

    /// Updates size unconditionally
    void recalculateSize();

    /// Updates size if the QFontMetrics is changed
    void recalculateSize(QFont const &font);

    // TODO removed default QTransform()
    QPointF portScenePosition(int portIndex,
                              PortType portType,
                              const QTransform &t = QTransform()) const;

    // returns portIndex
    int checkHitScenePoint(PortType portType,
                           QPointF point,
                           const QTransform &t = QTransform()) const;

    QRect resizeRect() const;

    /// Returns the position of a widget on the Node surface
    QPointF widgetPosition() const;

    /// Returns the maximum height a widget can be without causing the node to grow.
    int equivalentWidgetHeight() const;

    unsigned int captionWidth() const;

    unsigned int captionHeight() const;

    unsigned int validationHeight() const;

    unsigned int validationWidth() const;

    unsigned int portWidth(PortType portType) const;

    static QPointF calculateNodePositionBetweenNodePorts(int targetPortIndex, PortType targetPort, Node *targetNode,
                                                         int sourcePortIndex, PortType sourcePort, Node *sourceNode,
                                                         Node &newNode);

private:
    unsigned int _width = 100;
    unsigned int _height = 150;
    unsigned int _entryWidth;
    unsigned int _inputPortWidth = 70;
    unsigned int _outputPortWidth = 70;
    unsigned int _entryHeight = 20;
    unsigned int _spacing = 20;

    bool _hovered = false;

    unsigned int _nSource/* = dataModel->nPorts(PortType::Out)*/;
    unsigned int _nSinks/* = dataModel->nPorts(PortType::In)*/;

    QPointF _draggingPos = {-1000, -1000};

    QFontMetrics _fontMetrics;
    QFontMetrics _boldFontMetrics;


signals:

};

#endif // NODE_H
