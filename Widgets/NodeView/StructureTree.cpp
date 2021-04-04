#include "icons/IconFactory.h"

#include "StructureTree.h"


StructureTree::StructureTree(QWidget *parent) : QTreeWidget(parent) {
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setHeaderHidden(true);
    setSortingEnabled(true);
    // setMouseTracking(true);
    setMaximumSize(145, 250);

    createWindow();


    addClass("myclass1");
    addClass("myclass2");

    // addClassItem("class1", "func1");
    // addClassItem("class2", "func2");

    collapseAll();
}

void StructureTree::createWindow() {
    sortAsc();
}


void StructureTree::addClass(const QString &className) {
    auto *item = new QTreeWidgetItem();
    item->setText(0, className);
    item->setIcon(0, QIcon(IconFactory::Done));
    addTopLevelItem(item);

    item->addChild(new QTreeWidgetItem);
}

void StructureTree::removeClass(const QString &className) {
    takeTopLevelItem(indexOfTopLevelItem(findClassIndex(className)));
}

void StructureTree::addClassItem(const QString &className, const QString &symbolName) {
    auto *classItem = findClassIndex(className);
    auto *item = new QTreeWidgetItem();
    item->setText(0, symbolName);
    item->setIcon(0, QIcon(IconFactory::Done));

    // TODO: remove this
    classItem->addChild(item);
}

void StructureTree::removeClassItem(const QString &className, const QString &symbolName) {
    auto *classItem = findClassIndex(className);
    auto *item = new QTreeWidgetItem();
    item->setText(0, symbolName);
    item->setIcon(0, QIcon(IconFactory::Done));

    classItem->removeChild(item);
}

QTreeWidgetItem* StructureTree::findClassIndex(const QString &className) {
    for(int i = 0; i < topLevelItemCount(); i++) {
        if(topLevelItem(i)->text(0) == className) {
            return topLevelItem(i);
        }
    }
}

QTreeWidgetItem* StructureTree::findsymbolInClass(QTreeWidgetItem* classItem, const QString &symbolName) {
    for(int i = 0; i < classItem->childCount(); i++) {
        if(classItem->child(i)->text(0) == symbolName) {
            return classItem->child(i);
        }
    }
}

void StructureTree::highlightItem(const QString &className, const QString &symbolName, const QString& msg, const clang::DiagnosticsEngine::Level &level) {
    QTreeWidgetItem *item;

    if(symbolName.isEmpty()) {
        // class
        item = findClassIndex(className);
    }
    else {
        item = findsymbolInClass(findClassIndex(className), symbolName);
    }

    switch (level) {
        case clang::DiagnosticsEngine::Level::Warning:
            item->setForeground(0, QColor(Qt::yellow));
            item->setToolTip(0, msg);
            break;
        case clang::DiagnosticsEngine::Level::Error:
            item->setForeground(0, QColor(Qt::red));
            item->setToolTip(0, msg);
            break;
            // TODO: add others
        default:
            item->setToolTip(0, msg);
            break;
    }
}

void StructureTree::removehighlightedItem(const QString &className, const QString &symbolName) {
    QTreeWidgetItem *item;
    if(symbolName.isEmpty()) {
        // class
        item = findClassIndex(className);
    }
    else {
        item = findsymbolInClass(findClassIndex(className), symbolName);
    }

    // TODO: hope it gets back default color
    item->setForeground(0, QBrush());
}

void StructureTree::sortAsc() {
    sortItems(0, Qt::AscendingOrder);
}

void StructureTree::sortDesc() {
    sortItems(0, Qt::DescendingOrder);
}
