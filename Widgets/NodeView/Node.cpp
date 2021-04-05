#include <QApplication>
#include <QDebug>

#include "icons/IconFactory.h"
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

void Node::addPort(const Node::PortPosition& position) {
    // first consider only left
    // TODO: port
    const auto rect = boundingRect();
    // width, height is 10x10

    switch (position) {
        case Left:
            //newPort->setRect(rect.x(), rect.y() + distance, 10, 10);
            break;
        case Right:
            //newPort->setRect(rect.width() - 15, rect.y() + distance, 10, 10);
            break;
    }
}

QPointF Node::suggestPortPosition(const Node::PortPosition &position) {


    return QPointF(0, 50);
}

void Node::startToDrawConnectionLine(const QPointF& portPos) {
    auto *connection = new Connection(scene);
    // scene->addItem(connection);
    connection->startPos = cursor().pos();
    connection->setFirstNode(this, portPos); // FIXME: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!§§
    scene->pendingConnection.connection = connection;
}

void Node::lockNode(bool locked) {
    _locked = locked;

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void Node::createWindow() {
    textEdit = new QTextEdit;
    toolBar = new QToolBar;
    removeNode = new QToolButton;
    caption = new QLineEdit;

    textEdit->setStyleSheet("border: 2px solid black; /*background-color: green;*/");
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    removeNode->setIcon(QIcon(IconFactory::Remove));

    toolBar->addWidget(removeNode);
    toolBar->setFixedHeight(25);
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

    setWindowTitle("Basic Graphics Layouts Example");
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QGraphicsWidget::paint(painter, option, widget);

    QPen pen(Qt::darkGray, 3);
    // pen.setCapStyle(Qt::RoundCap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    painter->drawRoundedRect(boundingRect(), 10.0, 10.0);

    // shadow port if hovered
    if(drawShadowPort) {
        pen.setBrush(Qt::white);
        pen.setColor(Qt::gray);
        painter->setPen(pen);
        painter->drawEllipse(animePortPosition.x(), animePortPosition.y(), 15, 15);
    }

    // draw already connected ports and active ports
    for(const QPointF& point : activePorts) {
        pen.setBrush(Qt::blue);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->drawEllipse(point.x(), point.y(), 15, 15);
    }

    // painter->drawRect(boundingRect());
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // setSelected(true);
    const QRectF rect = boundingRect();
    if((event->pos().x() < 15)) {
        // TODO: figure this out
        // addPort(Left);
        activePorts.append(event->pos());

        if(!scene->pendingConnection.connection) {
            // first time
            scene->pendingConnection = { this, 0, event->pos(), Left, nullptr };
            startToDrawConnectionLine(event->pos());
        }
        else {
            // add connection
            // FIXME: event->pos() certain position
            scene->pendingConnection.connection->setSecondNode(this, event->pos());
            scene->allConnections.append(QPair(scene->pendingConnection, NodeConnection{ this, 0, event->pos(), Left, scene->pendingConnection.connection }));
            // second node connection established
            scene->pendingConnection.connection = nullptr;
        }
    }
    if(event->pos().x() > rect.width() -15) {
        activePorts.append(event->pos());

        if(!scene->pendingConnection.connection) {
            // first time
            scene->pendingConnection = { this, 0, event->pos(), Right, nullptr };
            startToDrawConnectionLine(event->pos());
        }
        else {
            // add connection
            scene->pendingConnection.connection->setSecondNode(this, event->pos());
            scene->allConnections.append(QPair(scene->pendingConnection, NodeConnection{ this, 0, event->pos(), Right, scene->pendingConnection.connection }));
            // second node connection established
            scene->pendingConnection.connection = nullptr;
        }
    }

    QGraphicsWidget::mousePressEvent(event);
}

void Node::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    // match port area, take only x
    const QRectF rect = boundingRect();
    qDebug() << rect;
    qDebug() << event->pos();

    if(event->pos().x() < 15) {
        drawShadowPort = true;
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        animePortPosition = QPointF(0, event->pos().y());
    }
    if(event->pos().x() > rect.width() -15) {
        drawShadowPort = true;
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        animePortPosition = QPointF(rect.width() - 15, event->pos().y());
    }
    else {
        drawShadowPort = false;
        QApplication::overrideCursor();
        QGraphicsWidget::hoverMoveEvent(event);
    }

    QGraphicsWidget::hoverMoveEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    drawShadowPort = false;
    QApplication::overrideCursor();

    QGraphicsWidget::hoverLeaveEvent(event);
}
