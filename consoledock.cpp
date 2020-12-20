#include "consoledock.h"

/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    ConsoleDock::setWindowTitle("Console");
    ConsoleDock::setVisible(false);
    ConsoleDock::setFeatures(AllDockWidgetFeatures);
    // setAllowedAreas(Qt::BottomDockWidgetArea);

    OuterTab = new QTabWidget(this);
    ConsoleDock::setWidget(OuterTab);
    // features
    OuterTab->setMovable(false);
    OuterTab->setTabsClosable(false);
    OuterTab->setUsesScrollButtons(true);
    OuterTab->setTabPosition(QTabWidget::South);
    OuterTab->setTabShape(QTabWidget::Rounded);


    OuterTab->addTab(BuildOutput(), "Compiler");
    OuterTab->addTab(BuildSearchResults(), "Search Results");

}

QWidget *ConsoleDock::BuildOutput()
{
    InnerTab = new QTabWidget(this);

    // features
    InnerTab->setMovable(false);
    InnerTab->setTabsClosable(false);
    InnerTab->setUsesScrollButtons(true);
    InnerTab->setTabPosition(QTabWidget::West);
    TabStyle = new CustomTabStyle();
    InnerTab->tabBar()->setStyle(TabStyle);

    Logs = new QPlainTextEdit(this);
    CompileOutput = new QPlainTextEdit(this);
    RawOutput = new QPlainTextEdit(this);

    InnerTab->addTab(Logs, "Logs");
    InnerTab->addTab(CompileOutput, "Compile Output");
    InnerTab->addTab(RawOutput, "Raw Output");

    return InnerTab;
}

QWidget *ConsoleDock::BuildSearchResults()
{
    find_replace = new FindReplaceWidget(true, this);

    return find_replace;
}
