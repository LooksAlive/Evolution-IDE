#ifndef TAB_H
#define TAB_H

/*

  TabText -> filename
  tabWhatsThis -> No changes | != No changes
  TabToolTip -> filepath

*/


#include <QTabWidget>
#include <QToolButton>

class Tab : public QTabWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab() = default;

    QToolButton *AddNewTabButton;
};

#endif // TAB_H
