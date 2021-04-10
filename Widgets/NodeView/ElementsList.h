#ifndef ELEMENTSLIST_H
#define ELEMENTSLIST_H

#include <QTreeWidget>
#include <QItemDelegate>
#include <QWidget>
#include <QPainter>
#include <QApplication>

class ElementsList : public QTreeWidget {
    Q_OBJECT
public:
    explicit ElementsList(QWidget *parent = nullptr);
    ~ElementsList() = default;



private:
    void createWindow();

};



// increase height of an item
class ItemDelegate : public QItemDelegate
{
public:
    ItemDelegate(QObject *poParent = Q_NULLPTR) :
        QItemDelegate(poParent)
    {
    }

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {

        if(index.child(0, 0).isValid()) {
            QStyleOptionViewItem viewOptions;
            viewOptions.rect = option.rect;
            viewOptions.displayAlignment = Qt::AlignLeft;
            viewOptions.decorationAlignment = Qt::AlignLeft;
            viewOptions.viewItemPosition = QStyleOptionViewItem::Beginning;
            viewOptions.decorationPosition = QStyleOptionViewItem::Left;
            viewOptions.textElideMode = Qt::ElideLeft;
            viewOptions.decorationSize = QSize(15, 30);
            QApplication::style()->drawControl(QStyle::CE_PushButton, &viewOptions, painter);
        }
        QItemDelegate::paint(painter, option, index);
    }

    // Use this for setting tree item height.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize oSize = QItemDelegate::sizeHint(option, index);
        oSize.setHeight(30);

        return oSize;
    }
};



#endif // ELEMENTSLIST_H
