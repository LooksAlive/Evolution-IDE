#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QGraphicsEllipseItem>

#include <QGraphicsProxyWidget>
#include <QUuid>
#include <QFontMetrics>

#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

#include <QTextEdit>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>


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

class NewPort;

class Node : public QGraphicsWidget {
    Q_OBJECT
public:
    explicit Node(NodeScene *scene = nullptr);     // scene
    ~Node() = default;

    NodeScene *scene;

    enum PortPosition {
        Left = 0,
        Right
    };

    struct NodeConnection {
        int nodeID;
        int portID;
        int distance;   // does not matter if in same port
        PortPosition position;
    };

    // all connections: data for this node and other node
    // NOTE: empty connections(not connected ports) are not contained here
    QList<QPair<NodeConnection, NodeConnection>> connections;


    // new created port waiting for connection with node to be accepted
    // contains animePortPosition from click event
    QPair<PortPosition, QPointF> waitingPort;

    // paints a new port
    // distance from [0,0]; [x,0]   -> corners; distance depends on position
    // por ex. Left 30 --> top left corner 30px down
    // Right 30  --> top right corner 30px down
    void addPort(const PortPosition& position);

    // this invokes Connection to paint +
    void connectPorts(Node *first, Node *second, const int& portIDfirst, const int& portIDsecond);

    void startToDrawConnectionLine();


    // locks widget, no moves, editing, etc.
    void lockNode(bool locked);

    unsigned int numConnectionsForPort(int portID) const;


    bool _hovered = false;
    bool _locked = false;

    // base index for node
    int nodeID;
    bool changesOccured = false;


    // Style
    // for painting

public:

    QColor NormalBoundaryColor = QColor(255, 255, 255);
    QColor SelectedBoundaryColor = QColor(255, 165, 0);
    QColor GradientColor0 = Qt::gray;
    QColor GradientColor1 = QColor(80, 80, 80);
    QColor GradientColor2 = QColor(64, 64, 64);
    QColor GradientColor3 = QColor(58, 58, 58);
    QColor ShadowColor = QColor(20, 20, 20);
    QColor FontColor = Qt::white;
    QColor FontColorFaded = Qt::gray;

    QColor ConnectionPointColor = QColor(169, 169, 169);
    QColor FilledConnectionPointColor = Qt::cyan;

    QColor WarningColor = QColor(128, 128, 0);
    QColor ErrorColor = Qt::red;

    float PenWidth = 1.0;
    float HoveredPenWidth = 1.5;

    float ConnectionPointDiameter = 8.0;

    float Opacity = 0.8;


    QGraphicsLinearLayout *windowLayout;
    QToolBar *toolBar;
    QToolButton *removeNode;
    QTextEdit *textEdit;
    QLineEdit *caption;

private:
    void createWindow();
    // animating port with hover event

    bool drawShadowPort = false;
    QPointF animePortPosition;

protected:
    // style
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    // in margin (port) area, click creates port, draw connection line
    // hover shows shadowed port moving in this area + cursor
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};


#endif // NODE_H
