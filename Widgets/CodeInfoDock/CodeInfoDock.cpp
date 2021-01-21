#include "CodeInfoDock.h"

CodeInfoDock::CodeInfoDock(QWidget *parent) : QDockWidget(parent){
    setWindowTitle("Code Informations");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
}

void CodeInfoDock::buildCodeInfo() {

}


