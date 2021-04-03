#include <QApplication>

#include "NodeScene.h"
#include "Connection.h"
#include "Node.h"

Node::Node(NodeScene *scene) : scene(scene) {
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    setAcceptHoverEvents(true);
    setGraphicsEffect(new QGraphicsDropShadowEffect);

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

void Node::startToDrawConnectionLine() {
    auto *connection = new Connection(scene);
    connection->startPos = cursor().pos();
    scene->activeConnectionPath = connection;
}


void Node::lockNode(bool locked) {
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

    // only shadow mode
    animePort = new NewPort(this);
    animePort->setVisible(false);
    animePort->setBrush(Qt::lightGray);
    animePort->setPen(QPen(Qt::gray));

    toolBar->addWidget(removeNode);
    toolBar->setFixedHeight(20);
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

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const QPen pen(Qt::darkGray, 3);
    // pen.setCapStyle(Qt::RoundCap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    painter->drawRoundedRect(boundingRect(), 10.0, 10.0);
    // painter->drawRect(boundingRect());
    // QGraphicsWidget::paint(painter, option, widget);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    const QRectF rect = boundingRect();
    if((event->pos().x() < 15) || (event->pos().x() > rect.width() -15)){
        // TODO: figure this out
        // addPort(Left, event->pos().y());
        // startToDrawConnectionLine();
    }

    QGraphicsWidget::mousePressEvent(event);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsWidget::hoverEnterEvent(event);
    // match port area, take only x
    const QRectF rect = boundingRect();
    if(!(event->pos().x() < 15) || !(event->pos().x() > rect.width() -15)){
        return;
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    if(event->pos().x() < 15) {
        animePort->setPos(rect.x(), event->pos().y());
    }
    if(event->pos().x() > rect.width() -15) {
        animePort->setPos(rect.width() - 15, event->pos().y());
    }
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsWidget::hoverLeaveEvent(event);
    QApplication::restoreOverrideCursor();
    animePort->hide();
}




NewPort::NewPort(Node *parent) : node(parent) {
    // setRect(0,0,1,1);
    brush.setColor(Qt::red);
    pen.setColor(Qt::gray);
    pen.setWidth(3);
}


void NewPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(boundingRect());
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

