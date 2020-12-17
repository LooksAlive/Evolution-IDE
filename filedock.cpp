#include "filedock.h"

FileDock::FileDock(QWidget *parent) : QDockWidget(parent)
{
    DockerFileList = new QListWidget(this);

    /* QDockWidget inherited functions */
    setWindowTitle("Opened files");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setWidget(DockerFileList);
}
