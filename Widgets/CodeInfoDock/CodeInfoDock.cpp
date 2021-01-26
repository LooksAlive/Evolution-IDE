#include "CodeInfoDock.h"

CodeInfoDock::CodeInfoDock(QWidget *parent) : QDockWidget(parent){
    setWindowTitle("Code Informations");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);

    MainWidget = new QWidget(this);
    setMinimumHeight(250);

    setWidget(MainWidget);
}

void CodeInfoDock::buildCodeInfo() {

}


