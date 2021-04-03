#ifndef COMMENTTAGLIST_H
#define COMMENTTAGLIST_H

#include <QComboBox>
#include "EnvironmentSettings.h" // tags

class CommentTagList : public QComboBox {
Q_OBJECT
public:
    explicit CommentTagList(QWidget *parent = nullptr);
    ~CommentTagList() = default;


};

#endif // COMMENTTAGLIST_H
