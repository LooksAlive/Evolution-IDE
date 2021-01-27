#include "tab.h"

Tab::Tab(QWidget *parent) : QTabWidget(parent)
{
    /* QTabWidget inherited functions */
    setMovable(true);
    setMouseTracking(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);
    setAcceptDrops(true);
    setTabPosition(QTabWidget::North);
    setTabShape(QTabWidget::Rounded);
    // setElideMode(Qt::ElideRight); // not enough space for tab name -> smth...
    setFocusPolicy(Qt::ClickFocus);

    /* add + button to add a tab; connect with it */
    AddNewTabButton = new QToolButton(this);
    Tab::setCornerWidget(AddNewTabButton, Qt::TopRightCorner);/* button position */
    AddNewTabButton->setText("+");
}
