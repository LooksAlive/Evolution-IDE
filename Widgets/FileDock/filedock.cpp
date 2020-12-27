#include "filedock.h"

FileDock::FileDock(QWidget *parent) : QDockWidget(parent)
{
    DockerFileList = new QListWidget(this);

    /* QDockWidget inherited functions */
    FileDock::setWindowTitle("Opened files");
    FileDock::setVisible(false);
    FileDock::setFeatures(AllDockWidgetFeatures);
    FileDock::setWidget(DockerFileList);
}

FileDock::~FileDock(){
    delete DockerFileList;
}
