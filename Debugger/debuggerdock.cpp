#include "debuggerdock.h"

DebuggerDock::DebuggerDock(QWidget *parent) : QWidget(parent)
{
    base = new QWidget(this);
    base->setWindowTitle("debugger");

    base->setLayout(buildForm());


}

DebuggerDock::~DebuggerDock(){}

QLayout *DebuggerDock::buildForm(){
    // buildDebuggerWindow();
    source_view = new PlainTextEdit();
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(source_view);

    return layout;
}

QWidget *DebuggerDock::buildDebuggerWindow(){

    debugwindow = new QWidget();

    next = new QPushButton();

    return debugwindow;
}
