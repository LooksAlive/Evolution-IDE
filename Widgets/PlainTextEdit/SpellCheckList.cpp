#include "SpellCheckList.h"

SpellCheckList::SpellCheckList(QWidget *parent) : QComboBox(parent) {
    setVisible(false);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotShowMenu(const QPoint &)));

    menu = new QMenu(this);
    menu->addAction("Ignore");  //, SLOT(takeItem(currentRow()))

    setMaxVisibleItems(6);
    // setMinimumSize(125, 125);
}

void SpellCheckList::setItemList(const std::vector<std::string> &list) {
    clear();
    // int maxWidth;
    for (const std::string &item : list) {
        addItem(item.c_str());
    }
}

void SpellCheckList::slotShowMenu(const QPoint &pos) {
    //const QListWidgetItem *item = itemAt(pos);
    //takeItem(item->listWidget()->row(item));
    //menu->exec(mapToGlobal(pos));
}
