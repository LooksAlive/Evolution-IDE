#include <QShortcut>
#include <QFileDialog>

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Clang/ClangBridge.h"
#include "Widgets/CodeInfoDock/CodeInfoDock.h"

#include "NodeView.h"

NodeView::NodeView(QWidget *parent) : QGraphicsView(parent) {
    createNodeView();
    createMainMenu();
    connectAll();

    setInteractive(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // connect shortcuts
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] {
        if(scene->pendingConnection.connection) {
            scene->removeItem(scene->pendingConnection.connection);
            delete scene->pendingConnection.connection;
            scene->pendingConnection.connection = nullptr;
            // last in row was added in touch and is not connected to other node
            scene->pendingConnection.node->activePorts.removeLast();
        }
    });
}

void NodeView::createNodeView() {
    MainLayout = new QVBoxLayout(this);
    elementList = new ElementsList(this);
    structureTree = new StructureTree(this);
    scene = new NodeScene(this);

    setScene(scene);

    MainLayout->addWidget(elementList);
    MainLayout->addWidget(structureTree);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
    MainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void NodeView::createMainMenu() {
    mainMenu = new QMenu(this);

    mainMenu->addAction("new File"/*, Qt::CTRL + Qt::Key_N*/);
    mainMenu->addAction("Reset Scene", [=] { scene->resetScene(); });
    mainMenu->addAction("Duplicate Node", [=] { scene->duplicateNode(); }, Qt::CTRL + Qt::Key_C);
    mainMenu->addAction("Remove Node", [=] { scene->removeNode(); }, Qt::Key_Delete);
    mainMenu->addAction("Save Image As", [=] { takePicture(); });
    mainMenu->addAction("Scale To Fit", [=] { fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio); });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotShowMenu(const QPoint &)));
}

void NodeView::connectAll() {
    // structureTree
    connect(structureTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(structureTreeShowDocumentation(QTreeWidgetItem *, int)));
    connect(structureTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(structureTreeOpen(QTreeWidgetItem *, int)));
    // custom signal
    connect(structureTree, SIGNAL(removeItem(QTreeWidgetItem*)), this, SLOT(structureTreeRemove(QTreeWidgetItem *)));

    // elementList



}


void NodeView::showNodeFromBlock(const QPoint &pos) {
    textNodeManager.createNodesFromAST();
    Node* node = textNodeManager.getNodeIDFromBlock(pos);

    // TODO: show it to main screen
}

void NodeView::showTextFromNode(Node *node) {
    const QPair<QString, int> data = textNodeManager.getTextPostitionFromNode(node);
    // connect OpenFile() outside
    emit openFile(data.first, data.second);
}


void NodeView::scaleUp() {
    const float step = 1.2;
    const float factor = std::pow(step, 1.0);

    const QTransform t = transform();

    if (t.m11() > 2.0)
        return;

    scale(factor, factor);
}


void NodeView::scaleDown() {
    const float step = 1.2;
    const float factor = std::pow(step, -1.0);

    scale(factor, factor);
}

void NodeView::takePicture() {
    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath());
    if (!fileName.isNull()) {
        // all view
        QPixmap pixMap = grab();
        pixMap.save(fileName);

        // scene
        /*
        QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        image.fill(Qt::transparent);
        scene->render(&painter);
        image.save(fileName);
        */
    }
}


// Protected functions ************************************************

void NodeView::drawBackground(QPainter* painter, const QRectF& r) {
    QGraphicsView::drawBackground(painter, r);

    auto drawGrid =
            [&](double gridStep)
    {
        QRect   windowRect = rect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left   = std::floor(tl.x() / gridStep - 0.5);
        double right  = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top    = std::floor (br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * gridStep, bottom * gridStep,
                        xi * gridStep, top * gridStep );

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * gridStep, yi * gridStep,
                        right * gridStep, yi * gridStep );
            painter->drawLine(line);
        }
    };

    const QBrush bBrush = backgroundBrush();

    const QPen pfine(FineGridColor, 1.0);

    painter->setPen(pfine);
    drawGrid(15);

    const QPen p(CoarseGridColor, 1.0);

    painter->setPen(p);
    drawGrid(150);

    if(drawingSelection) {
        // selectionStart ...
        // TODO: handle in click event SHIFT
    }
}

void NodeView::wheelEvent(QWheelEvent *event) {
    QPoint delta = event->angleDelta();

    if (delta.y() == 0) {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());

    if (d > 0.0)
        scaleUp();
    else
        scaleDown();
}

void NodeView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::RubberBandDrag);
        break;

    default:
        break;
    }

    QGraphicsView::keyPressEvent(event);
}


void NodeView::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::ScrollHandDrag);
        break;

    default:
        break;
    }
    QGraphicsView::keyReleaseEvent(event);
}


void NodeView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    QApplication::setOverrideCursor(Qt::DragMoveCursor);
    if (event->button() == Qt::LeftButton) {
        clickPos = mapToScene(event->pos());
    }
}

void NodeView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    QApplication::restoreOverrideCursor();
}

void NodeView::mouseMoveEvent(QMouseEvent *event) {
    scene->currentPos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
    if (scene->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
        // Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            QPointF difference = clickPos - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}

void NodeView::showEvent(QShowEvent *event) {
    // _scene->setSceneRect(this->rect());
    QGraphicsView::showEvent(event);
}

void NodeView::dropEvent(QDropEvent *event) {
    event->setDropAction(Qt::MoveAction);
    event->accept();
    event->acceptProposedAction();
    // event->widget();
    if(QTreeWidget* lWidget = qobject_cast<QTreeWidget*> (event->source())) {
        lWidget->currentItem()->text(0);
        qDebug() << "tree....";
    }
    QGraphicsView::dropEvent(event);
}




void NodeView::structureTreeOpen(QTreeWidgetItem *item, int column) {

}

void NodeView::structureTreeShowDocumentation(QTreeWidgetItem *item, int column) {

}

void NodeView::structureTreeRemove(QTreeWidgetItem *item) {

}

void NodeView::slotShowMenu(const QPoint &pos) {
    mainMenu->exec(mapToGlobal(pos));
}

