#include "fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent) : QDockWidget(parent)
{

    FileModel = new QDirModel; /* explore by directories */
    FileView  = new QTreeView; /* explore by list */
    FileView->setModel(FileModel); /* set model */
    FileView->setRootIndex(FileModel->index("/home")); /* default location #TODO set with OpenFile */
    FileView->setColumnHidden(1, true);
    FileView->setColumnHidden(2, true);
    FileView->setColumnHidden(3, true);
    FileView->setHeaderHidden(true);

    /* QDockWidget inherited functions */
    setWindowTitle("File Explorer");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setWidget(FileView);
}
