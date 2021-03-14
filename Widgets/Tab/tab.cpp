#include <QTabBar>
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
    // tabBar()->setFixedHeight(15);
    QFont tabFont;
    tabFont.setItalic(true);
    tabFont.setFamily("Ubuntu Mono");
    tabBar()->setFont(tabFont);

    /* add + button to add a tab; connect with it */
    AddNewTabButton = new QToolButton(this);
    setCornerWidget(AddNewTabButton, Qt::TopRightCorner);/* button position */
    AddNewTabButton->setText("+");
}
