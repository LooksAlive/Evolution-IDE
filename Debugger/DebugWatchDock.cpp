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
    VariableTreeValues = new QTreeWidget(this);
    TitleBar = new QToolBar(this);

    TitleBar->setOrientation(Qt::Horizontal);
    TitleBar->setFixedHeight(35);

    // not editable yet, later possible to change values
    VariableTreeValues->setEditTriggers(QAbstractItemView::NoEditTriggers);
    VariableTreeValues->setColumnCount(2);
    VariableTreeValues->setHeaderLabels(QStringList() << "Name"
                                                      << "Value");

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    addWatch = new QToolButton(this);
    addWatch->setIcon(QIcon(IconFactory::AddWatch));
    addWatch->setToolTip("Add Watch");
    TitleBar->addWidget(addWatch);
    removeWatch = new QToolButton(this);
    removeWatch->setIcon(QIcon(IconFactory::RemoveWatch));
    removeWatch->setToolTip("Remove Watch");
    TitleBar->addWidget(removeWatch);
    modifyWatch = new QToolButton(this);
    modifyWatch->setIcon(QIcon(IconFactory::ModifyWatch));
    modifyWatch->setToolTip("Modify Watch");
    TitleBar->addWidget(modifyWatch);
    enableAll = new QToolButton(this);
    enableAll->setIcon(QIcon(IconFactory::ModifyWatch));
    enableAll->setToolTip("Enable all");
    TitleBar->addWidget(enableAll);
    disableAll = new QToolButton(this);
    disableAll->setIcon(QIcon(IconFactory::Mute));
    disableAll->setToolTip("Disable All");
    TitleBar->addWidget(disableAll);
    removeAll = new QToolButton(this);
    removeAll->setIcon(QIcon(IconFactory::RemoveAll));
    removeAll->setToolTip("Remove All");
    TitleBar->addWidget(removeAll);

    addWatch->setEnabled(false);
    modifyWatch->setEnabled(false);
    removeWatch->setEnabled(false);

    MainLayout->addWidget(VariableTreeValues);

    WatchWidget->setLayout(MainLayout);

    setWidget(WatchWidget);


    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
}
