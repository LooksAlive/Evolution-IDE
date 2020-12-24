#include "fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent) : QDockWidget(parent)
{

    FileModel = new QDirModel; /* explore by directories */
    FileView  = new QTreeView; /* explore by list */
    FileView->setModel(FileModel); /* set model */
    FileView->setRootIndex(FileModel->index(QDir::homePath())); /* default location #TODO set with OpenFile */
    FileView->setColumnHidden(1, true);
    FileView->setColumnHidden(2, true);
    FileView->setColumnHidden(3, true);
    FileView->setHeaderHidden(true);

    /* QDockWidget inherited functions */
    FileExplorer::setWindowTitle("File Explorer");
    FileExplorer::setVisible(false);
    FileExplorer::setFeatures(AllDockWidgetFeatures);
    FileExplorer::setWidget(FileView);
}

void FileExplorer::setRootDirectory(const QString &path){
    FileView->setRootIndex(FileModel->index(path));
}
