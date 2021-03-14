#include <QToolBar>
#include <QLayout>  // layout management
#include "icons/IconFactory.h"
#include "filedock.h"

FileDock::FileDock(QWidget *parent) : QDockWidget(parent) {
    DockerFileList = new QListWidget(this);

    /* QDockWidget inherited functions */
    setWindowTitle("Opened files");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    // DockerFileList->setViewMode(QListView::IconMode);

    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);

    setWidget(DockerFileList);

    auto *TitleBar = new QToolBar(this);
    auto *spacer = new QWidget(this);   // align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    TitleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    TitleBar->setFixedHeight(30);
    TitleBar->layout()->setContentsMargins(0, 0, 0, 0);
    TitleBar->layout()->setSpacing(0);
    setTitleBarWidget(TitleBar);
}

