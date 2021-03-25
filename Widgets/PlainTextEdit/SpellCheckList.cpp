#include "SpellCheckList.h"

SpellCheckList::SpellCheckList(QWidget *parent) : QListWidget(parent) {
    setVisible(false);
    setTabKeyNavigation(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotShowMenu(const QPoint &)));

    menu = new QMenu(this);
    menu->addAction("Ignore");  //, SLOT(takeItem(currentRow()))

    setMinimumSize(125, 125);
}

void SpellCheckList::setItemList(const std::vector<std::string> &list) {
    reset();
    // int maxWidth;
    for (const std::string &item : list) {
        auto Item = new QListWidgetItem(this);
        Item->setText(QString::fromStdString(item));
        Item->setForeground(Qt::green);
        addItem(Item);
    }
}

void SpellCheckList::slotShowMenu(const QPoint &pos) {
    const QListWidgetItem *item = itemAt(pos);
    takeItem(item->listWidget()->row(item));
    menu->exec(mapToGlobal(pos));
}
