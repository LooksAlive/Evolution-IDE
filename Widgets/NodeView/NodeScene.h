#ifndef NODESCENE_H
#define NODESCENE_H

#include <QGraphicsScene>

#include "Node.h"

class NodeDataModel;

class FlowItemInterface;

class Node;

class NodeGraphicsObject;

class Connection;

class ConnectionGraphicsObject;

class NodeStyle;

class Connection;

class DataModelRegistry;

struct TypeConverter {
};

class NodeScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit NodeScene(QObject *parent = nullptr);

    ~NodeScene() = default;

    std::shared_ptr<Connection>
    createConnection(Node::PortType connectedPort,
                     Node &node,
                     int portIndex);

    std::shared_ptr<Connection>
    createConnection(Node &nodeIn,
                     int portIndexIn,
                     Node &nodeOut,
                     int portIndexOut,
                     TypeConverter const &converter = TypeConverter{});

    std::shared_ptr<Connection> restoreConnection(QJsonObject const &connectionJson);

    void deleteConnection(Connection &connection);

    Node &createNode(std::unique_ptr<NodeDataModel> &&dataModel);

    Node &restoreNode(QJsonObject const &nodeJson);

    void removeNode(Node &node);

    DataModelRegistry &registry() const;

    void setRegistry(std::shared_ptr<DataModelRegistry> registry);

    void iterateOverNodes(std::function<void(Node *)> const &visitor);

    void iterateOverNodeData(std::function<void(NodeDataModel *)> const &visitor);

    void iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel *)> const &visitor);

    QPointF getNodePosition(Node const &node) const;

    void setNodePosition(Node &node, QPointF const &pos) const;

    QSizeF getNodeSize(Node const &node) const;

public:
    std::unordered_map<QUuid, std::unique_ptr<Node> > const &nodes() const;

    std::unordered_map<QUuid, std::shared_ptr<Connection> > const &connections() const;

    std::vector<Node *> allNodes() const;

    std::vector<Node *> selectedNodes() const;

public:
    void clearScene();

    void save() const;

    void load();

    QByteArray saveToMemory() const;

    void loadFromMemory(const QByteArray &data);

signals:

    /**
       * @brief Node has been created but not on the scene yet.
       * @see nodePlaced()
       */
    void nodeCreated(Node &n);

    /**
       * @brief Node has been added to the scene.
       * @details Connect to this signal if need a correct position of node.
       * @see nodeCreated()
       */
    void nodePlaced(Node &n);

    void nodeDeleted(Node &n);

    void connectionCreated(Connection const &c);

    void connectionDeleted(Connection const &c);

    void nodeMoved(Node &n, const QPointF &newLocation);

    void nodeDoubleClicked(Node &n);

    void connectionHovered(Connection &c, QPoint screenPos);

    void nodeHovered(Node &n, QPoint screenPos);

    void connectionHoverLeft(Connection &c);

    void nodeHoverLeft(Node &n);

    void nodeContextMenu(Node &n, const QPointF &pos);

private:
    using SharedConnection = std::shared_ptr<Connection>;
    using UniqueNode = std::unique_ptr<Node>;

    std::unordered_map<QUuid, SharedConnection> _connections;
    std::unordered_map<QUuid, UniqueNode> _nodes;
    std::shared_ptr<DataModelRegistry> _registry;

private slots:

    void setupConnectionSignals(Connection const &c);

    void sendConnectionCreatedToNodes(Connection const &c);

    void sendConnectionDeletedToNodes(Connection const &c);

    Node *locateNodeAt(QPointF scenePoint, NodeScene &scene,
                       QTransform const &viewTransform);



    // Style
    // for painting

public:
    QColor BackgroundColor = QColor(53, 53, 53);
    QColor FineGridColor = QColor(60, 60, 60);
    QColor CoarseGridColor = QColor(25, 25, 25);

};

#endif // NODESCENE_H
