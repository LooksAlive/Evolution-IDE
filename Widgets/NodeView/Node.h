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

class Port;

class Node : public QGraphicsWidget {
    Q_OBJECT
public:
    explicit Node(NodeScene *scene = nullptr);     // scene
    ~Node() = default;

    NodeScene *scene;

    enum PortType {
        None,
        In,
        Out
    };

    struct Port {
        PortType type;
        int index;
    };

    enum PortPosition {
        Left = 0,
        Right
    };

    // all ports withing node
    QList<QPair<PortPosition, int>> portIndexes;

    // paints a new port
    // distance from [0,0]; [x,0]   -> corners; distance depends on position
    // por ex. Left 30 --> top left corner 30px down
    // Right 30  --> top right corner 30px down
    void addPort(const PortPosition& position, const int& distance);

    // this invokes Connection to paint +
    void connectPorts(Node *in, Node *out, const int& portIndexIn, const int& portIndexOut);


    // locks widget, no moves, editing, etc.
    void lock(bool locked);

    unsigned int numConnectionsForPort(int portIndex) const;


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


};



class NewPort : public QObject,  public QGraphicsEllipseItem {
    Q_OBJECT
public:
    explicit NewPort(Node *parent = nullptr);     // scene
    ~NewPort() = default;


private:
    Node *node;


protected:
    // void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    // press creates connection line
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // hover changes color
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;




};




#endif // NODE_H
