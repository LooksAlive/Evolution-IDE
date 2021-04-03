#include "NodeScene.h"
//#include "NodePainter.h"

#include "Node.h"
//#include "Connection.h"

Node::Node(NodeScene *scene) : scene(scene) {
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    setMaximumSize(200, 200);
    createWindow();
}

void Node::addPort(const Node::PortPosition& position, const int &distance) {
    // first consider only left
    auto *newPort = new NewPort(this);  // parent is this node
    const auto rect = boundingRect();
    // width, height is 10x10

    switch (position) {
        case Left:
            newPort->setRect(rect.x(), rect.y() + distance, 10, 10);
            break;
        case Right:
            newPort->setRect(rect.width() - 15, rect.y() + distance, 10, 10);
            break;
    }
}


void Node::lock(bool locked) {
    _locked = locked;

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void Node::createWindow() {
    textEdit = new QTextEdit;
    textEdit->setStyleSheet("border: 3px solid red; background-color: green;");
    toolBar = new QToolBar;
    removeNode = new QToolButton;
    caption = new QLineEdit;

    toolBar->addWidget(removeNode);
    toolBar->setFixedHeight(30);
    caption->setText("caption");

    QGraphicsProxyWidget *bar = scene->addWidget(toolBar);
    QGraphicsProxyWidget *capt = scene->addWidget(caption);
    QGraphicsProxyWidget *edit = scene->addWidget(textEdit);

    windowLayout = new QGraphicsLinearLayout(Qt::Vertical);
    // QGraphicsGridLayout *grid = new QGraphicsGridLayout(windowLayout);
    // grid->setMinimumSize(100, 100);
    windowLayout->addItem(bar);
    windowLayout->addItem(capt);
    windowLayout->addItem(edit);

    // left and right are ports areas
    windowLayout->setContentsMargins(15, 0, 15, 0);
    windowLayout->setSpacing(0);

    setLayout(windowLayout);

    setWindowTitle(tr("Basic Graphics Layouts Example"));
}




NewPort::NewPort(Node *parent) : node(parent) {
    // setRect(0,0,1,1);
    setBrush(Qt::red);
    setPen(QPen(Qt::yellow));
}


void NewPort::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mousePressEvent(event);
}

void NewPort::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setBrush(Qt::green);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void NewPort::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setBrush(Qt::red);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

