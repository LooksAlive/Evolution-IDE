#ifndef NODESCENE_H
#define NODESCENE_H

#include <QGraphicsScene>

#include "Connection.h"
#include "Node.h"

class Node;


class NodeScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit NodeScene(QObject *parent = nullptr);
    ~NodeScene() = default;

    QList<QPair<int, Node*>> nodes;

    // caption -> symbol name (FUNCTION, CLASS, ENUM, ...)
    // returns Node index
    int spawnNode(const QString& caption, const QString& content);
    int spawnNode(const QString& content);

    // might be in Node class, also take cares for its connections
    // if success returns true
    bool removeNode(Node* node);

    // TODO: how to manage distance here , default is im the middle
    void connectNodes(const int& nodeID1, const Node::PortPosition& pos1, const int& nodeID2, const Node::PortPosition& pos2);


    Connection *activeConnectionPath = nullptr;


    // Style
    // for painting

public:
    QColor BackgroundColor = QColor(53, 53, 53);
    QColor FineGridColor = QColor(60, 60, 60);
    QColor CoarseGridColor = QColor(25, 25, 25);

};

#endif // NODESCENE_H
