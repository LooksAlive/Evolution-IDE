#ifndef NODESCENE_H
#define NODESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "Connection.h"
#include "Node.h"

class NodeScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit NodeScene(QObject *parent = nullptr);
    ~NodeScene() = default;


    // caption -> symbol name (FUNCTION, CLASS, ENUM, ...)
    // returns Node index
    Node* spawnNode(const QString& caption, const QString& content);
    Node* spawnNode(const QString& content);

    // TODO: how to manage distance here , default is im the middle
    // uses suggestPortPosition
    void connectNodes(Node* node1, const Node::PortPosition& pos1, Node* node2, const Node::PortPosition& pos2);

    // basically current selected node

    void duplicateNode();
    // might be in Node class, also take cares for its connections
    void removeNode();

    // FIXME: ports have portID and Connection* in struct, but how to manage more Connections to one port
    // even between 2 same nodes ???
    // iterate allConnections, find match and go on ...
    void removeConnectionForPortRelation(Node *node1, Node *node2);

    void resetScene();


    QPointF currentPos;

    QList<Node*> allNodes;

    // if nullptr, no connection is pending, otherwise we draw and wait for confirmation
    // NOTE: we need connect 2 nodes, therefore it has to be directed here in scene

    // new created port waiting for connection with node to be accepted
    // contains animePortPosition from click event
    Node::NodeConnection pendingConnection;

    // all connections: data for all pair nodes
    // NOTE: empty connections(not connected ports) are not contained here
    QList<QPair<Node::NodeConnection, Node::NodeConnection>> allConnections;

    // class structure, file structure, etc.
    QList<QGraphicsItemGroup*> nodeGroups;


private:
    void setNodeSizeByContentMetrics(Node *node);
    // for ex. when generated or just added new node, has to be sure they are in order, not overlaping themselves
    void setNodeCoordinates(Node *node);

    // selected items if empty list
    void createNodesGroup(const QList<Node*>& nodes = QList<Node*>());






};

#endif // NODESCENE_H
