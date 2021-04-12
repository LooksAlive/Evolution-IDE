#ifndef EVOLUTION_IDE_NODEVIEW_H
#define EVOLUTION_IDE_NODEVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWidgetAction>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMenu>

#include "TextNodeRelationManager.h"
#include "ElementsList.h"
#include "StructureTree.h"
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
    void showTextFromNode(Node *node);


private:
    void createNodeView();
    void createMainMenu();
    void connectAll();
    NodeScene *scene;
    QVBoxLayout *MainLayout;
    ElementsList *elementList;
    StructureTree *structureTree;

    QMenu *mainMenu;

    PlainTextEdit *edit;
    ClangBridge *clang;
    CodeInfoDock *code_info;

    TextNodeRelationManager textNodeManager;

    void scaleUp();
    void scaleDown();
    void takePicture();

    // moving items arround the scene
    QPointF clickPos;


protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void drawBackground(QPainter* painter, const QRectF& r) override;
    void showEvent(QShowEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    /*
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    */


signals:
    void openFile(const QString &filepath, const int &line);


private slots:
    void structureTreeOpen(QTreeWidgetItem *item, int column);
    // one click show documentation if any
    void structureTreeShowDocumentation(QTreeWidgetItem *item, int column);
    void structureTreeRemove(QTreeWidgetItem *item);

    void slotShowMenu(const QPoint &pos);

private:
    QColor BackgroundColor = QColor(53, 53, 53);
    QColor FineGridColor = QColor(60, 60, 60);
    QColor CoarseGridColor = QColor(25, 25, 25);



};


#endif //EVOLUTION_IDE_NODEVIEW_H
