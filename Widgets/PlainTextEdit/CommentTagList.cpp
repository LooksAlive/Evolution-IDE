#include "CommentTagList.h"

CommentTagList::CommentTagList(QWidget *parent) : QListWidget(parent) {
    setVisible(false);
    setTabKeyNavigation(true);


    setItemAlignment(Qt::AlignCenter);
    addItems(CommentTags);

    setFixedSize(125, 125);
}
