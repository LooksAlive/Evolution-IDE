#include "compiledock.h"

/* enum QTabBar::Shape */

CompileDock::CompileDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    setWindowTitle("Terminal");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    // setAllowedAreas(Qt::BottomDockWidgetArea);

    OuterTab = new QTabWidget(this);
    setWidget(OuterTab);
    // features
    OuterTab->setMovable(false);
    OuterTab->setTabsClosable(false);
    OuterTab->setUsesScrollButtons(true);
    OuterTab->setTabPosition(QTabWidget::South);
    OuterTab->setTabShape(QTabWidget::Rounded);

    InnerTab = new QTabWidget(this);
    // features
    InnerTab->setMovable(false);
    InnerTab->setTabsClosable(false);
    InnerTab->setUsesScrollButtons(true);
    InnerTab->setTabPosition(QTabWidget::West);
    style = new CustomTabStyle();
    InnerTab->tabBar()->setStyle(style);

    Logs = new QPlainTextEdit(this);
    CompileOutput = new QPlainTextEdit(this);
    RawCompileOutput = new QPlainTextEdit(this);
    SearchResults = new QWidget(this);


    OuterTab->addTab(InnerTab, "Compiler");
    OuterTab->addTab(SearchResults, "Search Results");

    InnerTab->addTab(Logs, "Logs");
    InnerTab->addTab(CompileOutput, "Compile Output");
    InnerTab->addTab(RawCompileOutput, "Raw Compile Output");

}

