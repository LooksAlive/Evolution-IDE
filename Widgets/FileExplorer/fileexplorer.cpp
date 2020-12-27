#include "fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent) : QDockWidget(parent)
{

    FileModel = new QDirModel; /* explore by directories */
    FileModel->setSorting(QDir::DirsFirst);
    FileModel->sort(0, Qt::AscendingOrder);
    // FileModel->setFilter(QDir::NoDotAndDotDot | QDir::Executable | QDir::NoSymLinks);

    FileView  = new QTreeView; /* explore by list */
    FileView->setModel(FileModel); /* set model */
    FileView->setColumnHidden(1, true);
    FileView->setColumnHidden(2, true);
    FileView->setColumnHidden(3, true);
    FileView->setHeaderHidden(true);
    FileView->acceptDrops();
    FileView->showDropIndicator();

    /* QDockWidget inherited functions */
    FileExplorer::setWindowTitle("File Explorer");
    FileExplorer::setVisible(false);
    FileExplorer::setFeatures(AllDockWidgetFeatures);
    FileExplorer::setWidget(FileView);
}

FileExplorer::~FileExplorer(){
    delete FileModel;
    delete FileView;
}

void FileExplorer::setRootDirectory(const QString &path){
    FileView->setRootIndex(FileModel->index(path));
}
