#include "tab.h"

Tab::Tab(QWidget *parent) : QTabWidget(parent)
{
    /* QTabWidget inherited functions */
    setMovable(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);
    setTabPosition(QTabWidget::North);
    setTabShape(QTabWidget::Rounded);
    // setElideMode(Qt::ElideRight); // not enoght space for tab name -> smth...


    /* add + button to add a tab; connect with it */
    AddNewTabButton = new QToolButton(this);
    setCornerWidget(AddNewTabButton, Qt::TopRightCorner);/* button position */
    AddNewTabButton->setText("+");
}
