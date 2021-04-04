#include <QTreeWidget>
#include <QDebug>
#include "NodeScene.h"

NodeScene::NodeScene(QObject *parent) : QGraphicsScene(parent) {
    setItemIndexMethod(QGraphicsScene::NoIndex);
    auto node1 = new Node(this);
    node1->setPos(100, 100);
    auto node2 = new Node(this);
    node2->setPos(300, 300);
    addItem(node1);
    addItem(node2);

    // spawnNode("caption", "content");
}

int NodeScene::spawnNode(const QString &caption, const QString &content) {
    auto *node = new Node(this);
    const int index = nodes.size() + 1;

    node->nodeID = index;
    node->caption->setText(caption);
    node->textEdit->setPlainText(content);
    nodes.append(QPair(index, node));
    addItem(node);
}

int NodeScene::spawnNode(const QString &content) {
    auto *node = new Node(this);
    const int index = nodes.size() + 1;

    node->nodeID = index;
    node->textEdit->setPlainText(content);
    node->caption->setVisible(false);
    nodes.append(QPair(index, node));
    addItem(node);
}

bool NodeScene::removeNode(Node* node) {
    for(int i = 0; i < nodes.size(); i++) {
        if(nodes[i].second == node) {
            // FIXME: forst remove all connections; this will update everything, text, reparse all
            removeItem(nodes[i].second);
            delete nodes[i].second;
            nodes[i].second = nullptr;
            nodes.removeAt(i);
            return true;
        }
    }
    return false;
}

void NodeScene::connectNodes(const int &nodeID1, const Node::PortPosition &pos1, const int &nodeID2, const Node::PortPosition &pos2) {
    // Logic where to connect, position nodes
}

