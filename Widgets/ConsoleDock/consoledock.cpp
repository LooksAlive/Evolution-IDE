#include "consoledock.h"

#include <QString>
#include <iostream>
#include <QDebug>

/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    setWindowTitle("Console");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    // setAllowedAreas(Qt::BottomDockWidgetArea);

    BuildConsole();

    setWidget(ConsoleOutput);
}

ConsoleDock::~ConsoleDock(){}

// change to QListWidget most probably, bc. of specific widget shows its own data and do not know
// how to do that with tabs, at least yet
void ConsoleDock::BuildConsole()
{
    ConsoleOutput = new QPlainTextEdit(this);
}

void ConsoleDock::setRawOutput(const QString &raw){
    ConsoleOutput->appendPlainText(raw);
}

void ConsoleDock::clear(){
    ConsoleOutput->clear();
}