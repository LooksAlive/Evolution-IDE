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

    auto *n1 = spawnNode("caption", "content");
    auto *n2 = spawnNode("caption", "content");

    // connectNodes(n1, Node::Right, n2, Node::Left);
}

Node* NodeScene::spawnNode(const QString &caption, const QString &content) {
    auto *node = new Node(this);
    node->caption->setText(caption);
    node->textEdit->setPlainText(content);
    allNodes.append(node);
    addItem(node);

    // connect all node actions
    connect(node->removeNode, &QAbstractButton::clicked, this, [=](){ removeNode(); });

    return node;
}

Node* NodeScene::spawnNode(const QString &content) {
    auto *node = new Node(this);
    node->textEdit->setPlainText(content);
    node->caption->setVisible(false);
    allNodes.append(node);
    addItem(node);

    return node;
}

void NodeScene::connectNodes(Node *node1, const Node::PortPosition &pos1, Node *node2, const Node::PortPosition &pos2) {
    auto* conn = new Connection(this);
    // Will there be pendingConnection empty ?
    conn->setFirstNode(node1, node1->suggestPortPosition(pos1));
    conn->setSecondNode(node2, node1->suggestPortPosition(pos2));

    node1->activePorts.append(node1->suggestPortPosition(pos1));
    node2->activePorts.append(node2->suggestPortPosition(pos2));

    allConnections.append(QPair(Node::NodeConnection{ node1, 0, node1->suggestPortPosition(pos1), pos1, conn },
                                Node::NodeConnection{ node2, 0, node1->suggestPortPosition(pos2), pos2, conn }));
}

void NodeScene::duplicateNode() {
    if(selectedItems().isEmpty()) {
        return;
    }
    Node *node = dynamic_cast<Node*>(selectedItems()[0]);
    // create copy
    Node *newNode = spawnNode(node->caption->text(), node->textEdit->toPlainText());
    // + change position, move right
    newNode->setPos(node->pos().x() + node->minimumWidth(), node->pos().y());
}

void NodeScene::removeNode() {
    if(selectedItems().isEmpty()) {
        return;
    }
    Node *selectedNode = dynamic_cast<Node*>(selectedItems()[0]);
    // TODO: which group
    for(int i = 0; i < allNodes.size(); i++) {
        if(allNodes[i] == selectedNode) { // fix this
            // FIXME: first remove all connections; this will update everything, text, reparse all
            // FIXME: copying
            for(int z = 0; z < allConnections.size(); z++) {
                if(allConnections[z].first.connection != nullptr) {
                    removeItem(allConnections[z].first.connection);
                    delete allConnections[z].first.connection;
                    allConnections[z].first.connection = nullptr;
                    allConnections.removeAt(i);
                }
            }
            // by removing node itself, all ports, data, etc (children) will vanish together
            removeItem(allNodes[i]);
            delete allNodes[i];
            allNodes[i] = nullptr;
            allNodes.removeAt(i);
        }
    }
}

void NodeScene::resetScene() {
    clear();

    allNodes.clear();
    allConnections.clear();
    nodeGroups.clear();

    pendingConnection.connection = nullptr;
    pendingConnection.portPos = QPointF();
    pendingConnection.node = nullptr;
    pendingConnection.portPos = QPointF();
    // position not matter.
}

void NodeScene::setNodeSizeByContentMetrics(Node *node) {
    QFontMetrics fontMetrics(font());
    const int width = qMax(fontMetrics.boundingRect(node->caption->text()).width(), fontMetrics.boundingRect(node->textEdit->toPlainText()).width());
    const int height = qMax(fontMetrics.boundingRect(node->caption->text()).height(), fontMetrics.boundingRect(node->textEdit->toPlainText()).height());

    // TODO: +++ add some space +
    node->setGeometry(node->pos().x(), node->pos().y(), width, height);
    // node->setMinimumSize(width, height);
}

void NodeScene::setNodeCoordinates(Node *node) {

}

void NodeScene::createNodesGroup(const QList<Node *> &nodes) {
    if(nodes.empty()) {
        auto *group = createItemGroup(selectedItems());
        nodeGroups.append(group);
        return;
    }

    QGraphicsItemGroup *group =  createItemGroup((QList<struct QGraphicsItem *> &&) nodes);

    nodeGroups.append(group);
}



