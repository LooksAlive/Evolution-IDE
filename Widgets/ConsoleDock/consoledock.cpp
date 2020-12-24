#include "consoledock.h"

/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    setWindowTitle("Compile Output");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    // setAllowedAreas(Qt::BottomDockWidgetArea);

    BuildOutput();

    // connect(InnerTab->tabBar(),SIGNAL(currentChanged(int)),this, SLOT(setRawOutput(QString)));
}

// change to QListWidget most probably, bc. of specific widget shows its own data and do not know
// how to do that with tabs, at least yet
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

void ConsoleDock::setRawOutput(const QString &raw){
    RawOutput->setPlainText(raw);
    FormatRawOutput(raw);
}

void ConsoleDock::addLogMessage(const QString &log_msg){
    Logs->setPlainText(log_msg);
}

void ConsoleDock::FormatRawOutput(const QString &raw){
    // """"""""""""""""""""""""""""
    CompileOutput->setPlainText(raw);
}
