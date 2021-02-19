// have to be there, parsing error with some .inc files, edit was also not found
#include "Clang/ClangBridge.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include <QToolBar>
#include "icons/IconFactory.h"

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
    MainLayout = new QHBoxLayout();// this,  to be set for dock
    SignatureActionLayout = new QVBoxLayout();
    CompleterLinterLayout = new QVBoxLayout();

    signature = new QLabel(this);
    documentation = new QLabel(this);
    calls = new QListWidget(this);
    actions = new QListWidget(this);
    auto *t_bar = new QToolBar(this);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    t_bar->addWidget(spacer);
    t_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    t_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    t_bar->setFixedHeight(35);
    //t_bar->setIconSize(QSize(25, 35));
    setTitleBarWidget(t_bar);
    SignatureActionLayout->addWidget(signature);
    SignatureActionLayout->addWidget(documentation);
    SignatureActionLayout->addWidget(calls);
    SignatureActionLayout->addWidget(actions);

    completer = new QListWidget(this);
    linter = new QListWidget(this);
    CompleterLinterLayout->addWidget(completer);
    CompleterLinterLayout->addWidget(linter);

    MainLayout->addLayout(SignatureActionLayout);
    MainLayout->addLayout(CompleterLinterLayout);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}
