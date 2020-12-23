#include "consoledock.h"

/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    ConsoleDock::setWindowTitle("Console");
    ConsoleDock::setVisible(false);
    ConsoleDock::setFeatures(AllDockWidgetFeatures);
    // setAllowedAreas(Qt::BottomDockWidgetArea);

    BuildOutput();

}

void ConsoleDock::BuildOutput()
{
    InnerTab = new QTabWidget(this);
    ConsoleDock::setWidget(InnerTab);

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
}
