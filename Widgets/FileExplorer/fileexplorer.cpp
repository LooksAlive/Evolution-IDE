#include <QSettings>

#include "fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent) : QDockWidget(parent)
{
    FileModel = new QFileSystemModel(this);
    FileModel->setRootPath(QDir::homePath());
    //FileModel->setFilter(QDir::NoDotAndDotDot);
    FileModel->sort(0, Qt::AscendingOrder);

    FileView  = new QTreeView(this); /* explore by list */
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
    // here add small 2 buttons (file, dir) and QLineEdit for name + vertical layout and setLayout
    setWidget(FileView);
}

void FileExplorer::setRootDirectory(const QString &path){
    FileView->setRootIndex(FileModel->index(path));
    QSettings settings("Evolution");
    settings.setValue("Evolution/Project_Root_Dir", path);
    bool def = settings.value("Evolution/SetDefaultSettings").toBool();
    if(!def){
        settings.setValue("Evolution/SetDefaultSettings", true);
    }
}

void FileExplorer::newWindow() {
    window = new NewWindow(this);

    QString dirname = window->new_dirname;
    // create new dir
    // FileModel->mkdir(, dirname);
}

void FileExplorer::slotExpand() {
    FileView->expandAll();
}

void FileExplorer::slotCollapse() {
    FileView->collapseAll();
}

void FileExplorer::slotNew() {
    newWindow();
}
