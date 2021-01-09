#include "fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent) : QDockWidget(parent)
{

    FileModel = new QDirModel; /* explore by directories */
    FileModel->setSorting(QDir::DirsFirst);
    FileModel->sort(0, Qt::AscendingOrder);
    // FileModel->setFilter(QDir::NoDotAndDotDot | QDir::Executable | QDir::NoSymLinks);

    FileView  = new QTreeView; /* explore by list */
    FileView->setUpdatesEnabled(true);
    FileView->setModel(FileModel); /* set model */
    FileView->setColumnHidden(1, true);
    FileView->setColumnHidden(2, true);
    FileView->setColumnHidden(3, true);
    FileView->setHeaderHidden(true);
    FileView->acceptDrops();        // dirs and files
    FileView->showDropIndicator();

    /*
    connect() // some button
    FileView->expandAll();
    FileView->collapseAll();
    // create file, dir, ...
    */


    /* QDockWidget inherited functions */
    setWindowTitle("File Explorer");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setWidget(FileView);
}

FileExplorer::~FileExplorer(){
    delete FileModel;
    delete FileView;
}

void FileExplorer::setRootDirectory(const QString &path){
    FileView->setRootIndex(FileModel->index(path));
}
