#include "tab.h"

Tab::Tab(QWidget *parent) : QTabWidget(parent)
{
    /* QTabWidget inherited functions */
    Tab::setMovable(true);
    Tab::setTabsClosable(true);
    Tab::setUsesScrollButtons(true);
    Tab::setTabPosition(QTabWidget::North);
    Tab::setTabShape(QTabWidget::Rounded);
    // setElideMode(Qt::ElideRight); // not enoght space for tab name -> smth...


    /* add + button to add a tab; connect with it */
    AddNewTabButton = new QToolButton(this);
    Tab::setCornerWidget(AddNewTabButton, Qt::TopRightCorner);/* button position */
    AddNewTabButton->setText("+");
}

Tab::~Tab(){}
