#include "ElementsList.h"


ElementsList::ElementsList(QWidget *parent) : QTreeWidget(parent){
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setHeaderHidden(true);
    // setMouseTracking(true);
    setMaximumSize(145, 250);
    setIndentation(15);

    createWindow();
    setItemDelegate(new ItemDelegate(this));

    setRootIsDecorated(true);

    // setStyleSheet("QListView::item { outline: none; padding: 0px; margin: 0px; }");
}

void ElementsList::createWindow() {
    // TODO: add icons
    auto *elements = new QTreeWidgetItem();
    elements->setText(0, "Elements");
    addTopLevelItem(elements);

    auto *variable = new QTreeWidgetItem();
    variable->setText(0, "Variable");
    variable->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    elements->addChild(variable);

    auto *function = new QTreeWidgetItem();
    function->setText(0, "Function");
    elements->addChild(function);

    auto *enumm = new QTreeWidgetItem();
    enumm->setText(0, "Enum");
    elements->addChild(enumm);

    auto *classs = new QTreeWidgetItem();
    classs->setText(0, "Class");
    elements->addChild(classs);

    auto *structt = new QTreeWidgetItem();
    structt->setText(0, "Struct");
    elements->addChild(structt);

    auto *templatee = new QTreeWidgetItem();
    templatee->setText(0, "Template");
    elements->addChild(templatee);



    auto *codeActions = new QTreeWidgetItem();
    codeActions->setText(0, "Code Actions");
    addTopLevelItem(codeActions);

    auto *documentation = new QTreeWidgetItem();
    documentation->setText(0, "Documentation");
    codeActions->addChild(documentation);

    auto *commentTag = new QTreeWidgetItem();
    commentTag->setText(0, "Comment Tag");
    codeActions->addChild(commentTag);

    auto *testItem = new QTreeWidgetItem();
    testItem->setText(0, "Test Item");
    codeActions->addChild(testItem);


    collapseAll();
}
