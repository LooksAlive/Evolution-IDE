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
public:
    ItemDelegate(QObject *poParent = Q_NULLPTR, int iHeight = -1) :
        QItemDelegate(poParent), m_iHeight(iHeight)
    {
    }

    void SetHeight(int iHeight)
    {
        m_iHeight = iHeight;
    }

    // Use this for setting tree item height.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize oSize = QItemDelegate::sizeHint(option, index);

        if (m_iHeight != -1)
        {
            // Tree item height.
            oSize.setHeight(45);
        }

        return oSize;
    }
};



#endif // ELEMENTSLIST_H
