#include "CommentTagList.h"

CommentTagList::CommentTagList(QWidget *parent) : QComboBox(parent) {
    setVisible(false);
    addItems(CommentTags);
    setMaxVisibleItems(6);
    // setFixedSize(125, 125);

}
