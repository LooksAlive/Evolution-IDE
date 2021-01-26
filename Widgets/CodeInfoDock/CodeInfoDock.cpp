#include "CodeInfoDock.h"

CodeInfoDock::CodeInfoDock(QWidget *parent) : QDockWidget(parent){
    setWindowTitle("Code Informations");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
    createWindow();

    MainWidget = new QWidget(this);
    setMinimumHeight(150);

    setWidget(MainWidget);
}

void CodeInfoDock::createWindow() {

}


