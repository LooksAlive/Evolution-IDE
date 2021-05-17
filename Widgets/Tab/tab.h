#ifndef TAB_H
#define TAB_H

/*

  TabText -> filename
  tabWhatsThis -> No changes | != No changes
  TabToolTip -> filepath

*/


#include <QTabWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class Tab : public QTabWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab() = default;

    QToolButton *AddNewTabButton;



};





class TabLayout : public QWidget {
Q_OBJECT
public:
    explicit TabLayout(QWidget *parent = nullptr);
    ~TabLayout() = default;


    bool isOnlyOneEditor = true;


private:
    QHBoxLayout *horizontal;
    QVBoxLayout *vertical;


};









#endif // TAB_H
