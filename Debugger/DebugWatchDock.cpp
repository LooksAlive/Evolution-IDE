#include "DebugWatchDock.h"
#include "icons/IconFactory.h"

DebugWatchDock::DebugWatchDock(QWidget *parent) : QDockWidget(parent) {
    createDock();
    setWindowTitle("Watch Dock");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setMaximumWidth(250);// for now ... later change responsibly

    setTitleBarWidget(TitleBar);
}

void DebugWatchDock::createDock() {

    MainLayout = new QVBoxLayout();
    WatchWidget = new QWidget(this);
    WatchToolBar = new QToolBar(this);
    WatchedVariables = new QListWidget(this);
    VariableTreeValues = new QTreeView(this);

    WatchToolBar->setOrientation(Qt::Horizontal);
    WatchToolBar->setFixedHeight(35);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    addWatch = new QToolButton(this);
    addWatch->setIcon(QIcon(IconFactory::AddWatch));
    addWatch->setToolTip("Add Watch");
    WatchToolBar->addWidget(addWatch);
    removeWatch = new QToolButton(this);
    removeWatch->setIcon(QIcon(IconFactory::RemoveWatch));
    removeWatch->setToolTip("Remove Watch");
    WatchToolBar->addWidget(removeWatch);
    modifyWatch = new QToolButton(this);
    modifyWatch->setIcon(QIcon(IconFactory::ModifyWatch));
    modifyWatch->setToolTip("Modify Watch");
    WatchToolBar->addWidget(modifyWatch);

    addWatch->setEnabled(false);
    modifyWatch->setEnabled(false);
    removeWatch->setEnabled(false);

    MainLayout->addWidget(WatchedVariables);
    MainLayout->addWidget(WatchToolBar);
    MainLayout->addWidget(VariableTreeValues);

    WatchWidget->setLayout(MainLayout);

    setWidget(WatchWidget);


    TitleBar = new QToolBar(this);
    TitleBar->setOrientation(Qt::Horizontal);
    TitleBar->setFixedHeight(35);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
}
