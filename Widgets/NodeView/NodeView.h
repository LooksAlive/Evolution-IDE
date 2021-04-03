#ifndef EVOLUTION_IDE_NODEVIEW_H
#define EVOLUTION_IDE_NODEVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWidgetAction>
#include <QLineEdit>
#include <QHBoxLayout>

#include "TextNodeRelationManager.h"
#include "ElementsList.h"
#include "NodeScene.h"


/*
 * Each node has nodeID (refers to Node struct with ranges), scopeChange(indicates editing)
 * Each Translation Unit (file) has TUchange (indicates editing)
*/

class PlainTextEdit;

class ClangBridge;

class CodeInfoDock;


class NodeView : public QGraphicsView {
Q_OBJECT
public:
    explicit NodeView(QWidget *parent = nullptr);
    ~NodeView() = default;

    void setCurrentWidget(PlainTextEdit *e) {
        edit = e;
        textNodeManager.setCurrentWidget(e);
    }

    void setClangBridge(ClangBridge *bridge) {
        clang = bridge;
        textNodeManager.setClangBridge(bridge);
    }

    void setCodeInfo(CodeInfoDock *info) { code_info = info; }


    // TODO: add menu item to text editor with this
    // from text editor
    void showNodeFromBlock(const QPoint &pos);

    // from node
    void showTextFromNode(const int &nodeID);


private:
    void createNodeView();
    NodeScene *scene;
    QHBoxLayout *MainLayout;
    ElementsList *elementList;


    PlainTextEdit *edit;
    ClangBridge *clang;
    CodeInfoDock *code_info;

    TextNodeRelationManager textNodeManager;

    // FlowScene *scene;


    void scaleUp();

    void scaleDown();

    QPointF _clickPos;


protected:
    //void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    //void drawBackground(QPainter* painter, const QRectF& r) override;
    void showEvent(QShowEvent *event) override;


signals:

    void openFile(const QString &filepath, const int &line);


};


#endif //EVOLUTION_IDE_NODEVIEW_H
