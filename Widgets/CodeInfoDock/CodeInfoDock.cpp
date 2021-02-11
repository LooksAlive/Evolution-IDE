#include "CodeInfoDock.h"

CodeInfoDock::CodeInfoDock(QWidget *parent) : QDockWidget(parent){
    setWindowTitle("Code Informations");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
    createWindow();

    MainWidget = new QWidget(this);
    setMinimumHeight(150);
    MainWidget->setLayout(MainLayout);

    setWidget(MainWidget);
}

void CodeInfoDock::createWindow() {
    MainLayout = new QHBoxLayout();  // this,  to be set for dock
    SignatureActionLayout = new QVBoxLayout();
   
    signature = new QLabel(this);
    calls = new QListWidget(this);
    actions = new QListWidget(this);
    SignatureActionLayout->addWidget(signature);
    SignatureActionLayout->addWidget(calls);
    SignatureActionLayout->addWidget(actions);
    
    completer = new QListWidget(this);
    MainLayout->addLayout(SignatureActionLayout);
    MainLayout->addWidget(completer);
}


