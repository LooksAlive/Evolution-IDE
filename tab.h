#ifndef TAB_H
#define TAB_H

#include <QTabWidget>
#include <QToolButton>

class Tab : public QTabWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    QToolButton *AddNewTabButton;
};

#endif // TAB_H
