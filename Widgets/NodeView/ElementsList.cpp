#include "ElementsList.h"


ElementsList::ElementsList(QWidget *parent) : QTreeWidget(parent){
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setHeaderHidden(true);
    // setMouseTracking(true);
    setMaximumSize(135, 250);

    createWindow();
    setItemDelegate(new ItemDelegate(this));
}

void ElementsList::createWindow() {
    // TODO: add icons
    auto *elements = new QTreeWidgetItem(this);
    elements->setText(0, "Elements");
    addTopLevelItem(elements);

    auto *variable = new QTreeWidgetItem(this);
    variable->setText(0, "Variable");
    elements->addChild(variable);

    auto *function = new QTreeWidgetItem(this);
    function->setText(0, "Function");
    elements->addChild(function);

    auto *enumm = new QTreeWidgetItem(this);
    enumm->setText(0, "Enum");
    elements->addChild(enumm);

    auto *classs = new QTreeWidgetItem(this);
    classs->setText(0, "Class");
    elements->addChild(classs);

    auto *structt = new QTreeWidgetItem(this);
    structt->setText(0, "Struct");
    elements->addChild(structt);

    auto *templatee = new QTreeWidgetItem(this);
    templatee->setText(0, "Template");
    elements->addChild(templatee);



    auto *codeActions = new QTreeWidgetItem(this);
    codeActions->setText(0, "Code Actions");
    addTopLevelItem(codeActions);

    auto *documentation = new QTreeWidgetItem(this);
    documentation->setText(0, "Documentation");
    codeActions->addChild(documentation);

    auto *commentTag = new QTreeWidgetItem(this);
    commentTag->setText(0, "Comment Tag");
    codeActions->addChild(commentTag);

    auto *testItem = new QTreeWidgetItem(this);
    testItem->setText(0, "Test Item");
    codeActions->addChild(testItem);


    collapseAll();
}
