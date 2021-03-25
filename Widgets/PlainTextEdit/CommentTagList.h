#ifndef COMMENTTAGLIST_H
#define COMMENTTAGLIST_H

#include <QListWidget>
#include "EnvironmentSettings.h" // tags

class CommentTagList : public QListWidget {
Q_OBJECT
public:
    explicit CommentTagList(QWidget *parent = nullptr);

    ~CommentTagList() = default;


};

#endif // COMMENTTAGLIST_H
