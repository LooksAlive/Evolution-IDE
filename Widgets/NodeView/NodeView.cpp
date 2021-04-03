#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Clang/ClangBridge.h"
#include "Widgets/CodeInfoDock/CodeInfoDock.h"

#include "NodeView.h"

NodeView::NodeView(QWidget *parent) : QGraphicsView(parent) {
    createNodeView();

    scene = new NodeScene(this);
    setScene(scene);
}

void NodeView::createNodeView() {

}


void NodeView::showNodeFromBlock(const QPoint &pos) {
    textNodeManager.createNodesFromAST();
    const int nodeID = textNodeManager.getNodeIDFromBlock(pos);

    // TODO: show it to main screen
}

void NodeView::showTextFromNode(const int &nodeID) {
    const QPair<QString, int> data = textNodeManager.getTextPostitionFromNode(nodeID);
    // connect OpenFile() outside
    emit openFile(data.first, data.second);
}


void NodeView::scaleUp() {
    const float step = 1.2;
    const float factor = std::pow(step, 1.0);

    QTransform t = transform();

    if (t.m11() > 2.0)
        return;

    scale(factor, factor);
}


void NodeView::scaleDown() {
    const float step = 1.2;
    const float factor = std::pow(step, -1.0);

    scale(factor, factor);
}







// Protected functions ************************************************
/*
void NodeView::contextMenuEvent(QContextMenuEvent *event) {
    if (itemAt(event->pos()))
    {
        QGraphicsView::contextMenuEvent(event);
        return;
    }

    QMenu modelMenu;

    auto skipText = QStringLiteral("skip me");

    //Add filterbox to the context menu
    auto *txtBox = new QLineEdit(&modelMenu);

    txtBox->setPlaceholderText(QStringLiteral("Filter"));
    txtBox->setClearButtonEnabled(true);

    auto *txtBoxAction = new QWidgetAction(&modelMenu);
    txtBoxAction->setDefaultWidget(txtBox);

    modelMenu.addAction(txtBoxAction);

    //Add result treeview to the context menu
    auto *treeView = new QTreeWidget(&modelMenu);
    treeView->header()->close();

    auto *treeViewAction = new QWidgetAction(&modelMenu);
    treeViewAction->setDefaultWidget(treeView);

    modelMenu.addAction(treeViewAction);

    QMap<QString, QTreeWidgetItem*> topLevelItems;
    for (auto const &cat : _scene->registry().categories())
    {
        auto item = new QTreeWidgetItem(treeView);
        item->setText(0, cat);
        item->setData(0, Qt::UserRole, skipText);
        topLevelItems[cat] = item;
    }

    for (auto const &assoc : _scene->registry().registeredModelsCategoryAssociation())
    {
        auto parent = topLevelItems[assoc.second];
        auto item   = new QTreeWidgetItem(parent);
        item->setText(0, assoc.first);
        item->setData(0, Qt::UserRole, assoc.first);
    }

    treeView->expandAll();

    connect(treeView, &QTreeWidget::itemClicked, [&](QTreeWidgetItem *item, int)
    {
        QString modelName = item->data(0, Qt::UserRole).toString();

        if (modelName == skipText)
        {
            return;
        }

        auto type = _scene->registry().create(modelName);

        if (type)
        {
            auto& node = _scene->createNode(std::move(type));

            QPoint pos = event->pos();

            QPointF posView = this->mapToScene(pos);

            node.nodeGraphicsObject().setPos(posView);

            _scene->nodePlaced(node);
        }
        else
        {
            qDebug() << "Model not found";
        }

        modelMenu.close();
    });

    //Setup filtering
    connect(txtBox, &QLineEdit::textChanged, [&](const QString &text)
    {
        for (auto& topLvlItem : topLevelItems)
        {
            for (int i = 0; i < topLvlItem->childCount(); ++i)
            {
                auto child = topLvlItem->child(i);
                auto modelName = child->data(0, Qt::UserRole).toString();
                const bool match = (modelName.contains(text, Qt::CaseInsensitive));
                child->setHidden(!match);
            }
        }
    });

    // make sure the text box gets focus so the user doesn't have to click on it
    txtBox->setFocus();

    modelMenu.exec(event->globalPos());
}

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

    auto const &flowViewStyle = StyleCollection::flowViewStyle();

    QBrush bBrush = backgroundBrush();

    QPen pfine(flowViewStyle.FineGridColor, 1.0);

    painter->setPen(pfine);
    drawGrid(15);

    QPen p(flowViewStyle.CoarseGridColor, 1.0);

    painter->setPen(p);
    drawGrid(150);
}
*/
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
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
    if (event->button() == Qt::LeftButton) {
        _clickPos = mapToScene(event->pos());
    }
}

void NodeView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    QApplication::restoreOverrideCursor();
}

void NodeView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    if (scene->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton) {
        // Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0) {
            QPointF difference = _clickPos - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}

void NodeView::showEvent(QShowEvent *event) {
    // _scene->setSceneRect(this->rect());
    QGraphicsView::showEvent(event);
}
