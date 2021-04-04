#ifndef ELEMENTSLIST_H
#define ELEMENTSLIST_H

#include <QTreeWidget>
#include <QItemDelegate>
#include <QWidget>

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
private:
    int m_iHeight;
    //QStyleOptionViewItem *viewOptions;
public:
    ItemDelegate(QObject *poParent = Q_NULLPTR, int iHeight = -1) :
        QItemDelegate(poParent), m_iHeight(iHeight)
    {
        /*
        viewOptions = new QStyleOptionViewItem();
        viewOptions->displayAlignment = Qt::AlignLeft;
        viewOptions->viewItemPosition = QStyleOptionViewItem::Beginning;
        viewOptions->decorationPosition = QStyleOptionViewItem::Left;
        */
    }

    void SetHeight(int iHeight)
    {
        m_iHeight = iHeight;
    }
    /*
    void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                     const QRect &rect, const QString &text) const override {
        QItemDelegate::drawDisplay(painter, *viewOptions, rect, text);
    }
    */
    // Use this for setting tree item height.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize oSize = QItemDelegate::sizeHint(option, index);

        if (m_iHeight != -1)
        {
            // Tree item height.
            oSize.setHeight(25);
        }
        oSize.setHeight(25);

        return oSize;
    }
};



#endif // ELEMENTSLIST_H
