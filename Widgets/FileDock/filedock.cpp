#include <QToolBar>
#include "icons/IconFactory.h"
#include "filedock.h"

FileDock::FileDock(QWidget *parent) : QDockWidget(parent)
{
    DockerFileList = new QListWidget(this);

    /* QDockWidget inherited functions */
    setWindowTitle("Opened files");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    // DockerFileList->setViewMode(QListView::IconMode);

    setWidget(DockerFileList);

    auto *t_bar = new QToolBar(this);
    auto *spacer = new QWidget(this);   // align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    t_bar->addWidget(spacer);
    t_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    t_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    t_bar->setFixedHeight(35);
    t_bar->setIconSize(QSize(25, 35));
    setTitleBarWidget(t_bar);
}

