#ifndef SPELLCHECKLIST_H
#define SPELLCHECKLIST_H

#include <QListWidget>
#include <QMenu>

#include <iostream>
#include <string>
#include <vector>

class SpellCheckList : public QListWidget {
Q_OBJECT
public:
    explicit SpellCheckList(QWidget *parent = nullptr);

    ~SpellCheckList() = default;

    // in insertion list automatically reset itself
    void setItemList(const std::vector<std::string> &list);

    // insertion is on a click

    // TODO: consider ignore list
    QMenu *menu;

private slots:

    void slotShowMenu(const QPoint &);

};

#endif // SPELLCHECKLIST_H
