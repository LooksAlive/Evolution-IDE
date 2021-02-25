#ifndef EVOLUTION_IDE_DELEGATE_H
#define EVOLUTION_IDE_DELEGATE_H

#include <QPainter>
#include <QStyleOptionViewItemV4>
#include <QStyledItemDelegate>
#include <QTextDocument>

class Delegate : public QStyledItemDelegate {
    Q_OBJECT
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif//EVOLUTION_IDE_DELEGATE_H
