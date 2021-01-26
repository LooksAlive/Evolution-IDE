#include <QSettings>
#include <QMessageBox>
#include "icons/IconFactory.h"
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

    /* QDockWidget inherited functions */
    setWindowTitle("File Explorer");
    // setTitleBarWidget();
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    createMenu();
    // here add small 2 buttons (file, dir) and QLineEdit for name + vertical layout and setLayout
    MainLayout = new QVBoxLayout();
    bar_buttons = new QToolBar(this);
    createSearchBox();

    bar_buttons->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    bar_buttons->setIconSize(QSize(20, 20));
    bar_buttons->setFixedHeight(35);
    bar_buttons->setOrientation(Qt::Horizontal);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    bar_buttons->addWidget(searchBox);
    bar_buttons->addAction(QIcon(IconFactory::Back), "Back",  this, SLOT(slotBack()));
    bar_buttons->addAction(QIcon(IconFactory::DefaultDir), "Set Default Dir",  this, SLOT(slotSetDefaultDir()));
    bar_buttons->addAction(QIcon(IconFactory::Collapse), "Collapse All",  this, SLOT(slotCollapse()));
    bar_buttons->addAction(QIcon(IconFactory::Expand), "Expand All",  this, SLOT(slotExpand()));
    bar_buttons->addAction(QIcon(IconFactory::Remove), "Close Explorer",  this, SLOT(hide()));

    /*
    MainLayout->addWidget(searchBox, 0, Qt::AlignLeft);
    MainLayout->addWidget(bar_buttons, 0, Qt::AlignRight);
    MainLayout->addWidget(FileView);
    QWidget *base = new QWidget(this);
    base->setLayout(MainLayout);
    setWidget(base);
    */

    setTitleBarWidget(bar_buttons);
    setWidget(FileView);
}

void FileExplorer::createMenu() {
    viewMenu = new QMenu(this);
    copyMenu = new QMenu("Copy", this);

    copyMenu->addAction(QIcon(IconFactory::Copy), "Copy File Content", this, SLOT(slotCopyFileContent()));
    copyMenu->addAction("Copy File Path", this, SLOT(slotCopyFilePath()));
    copyMenu->addAction("Copy File Name", this, SLOT(slotCopyFileName()));

    viewMenu->addAction(QIcon(IconFactory::NewDirectory), "New Directory", this, SLOT(slotNewDir()));
    viewMenu->addAction(QIcon(IconFactory::NewFile), "New File", this, SLOT(slotNewFile()));
    viewMenu->addAction("New Class", this, SLOT(slotNewC_CPP_CLASS()));
    viewMenu->addAction(QIcon(IconFactory::Copy), "Duplicate", this, SLOT(slotDuplicate()));
    viewMenu->addAction(QIcon(IconFactory::Remove), "Remove", this, SLOT(slotRemove()), Qt::Key_Delete);
    viewMenu->addMenu(copyMenu);
    viewMenu->addAction("Rename", this, SLOT(slotRename()));
    viewMenu->addAction(QIcon(IconFactory::Expand), "Expand All", this, SLOT(slotExpand()));
    viewMenu->addAction(QIcon(IconFactory::Collapse), "Collapse All", this, SLOT(slotCollapse()));

    FileView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(FileView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotShowMenu(const QPoint&)));
}

void FileExplorer::createSearchBox() {
    searchBox = new QLineEdit(this);
    searchBox->setToolTip("Search");
    // searchBox->setFixedWidth(130);

    connect(searchBox, SIGNAL(returnPressed()), this, SLOT(slotTreeSearch()));

    // later set completer etc.
    //completer = new QCompleter(this);
    //completer->setCompletionMode(QCompleter::PopupCompletion);
    //completer->setModel(FileModel);
    //searchBox->setCompleter(completer);
}

void FileExplorer::setRootDirectory(const QString &path){
    FileView->setRootIndex(FileModel->index(path));

    QSettings settings("Evolution");
    settings.setValue("Evolution/Project_Root_Dir", path);
    bool def = settings.value("Evolution/SetDefaultSettings").toBool();
    if(def){
        // set to default path /root
        FileView->setRootIndex(FileModel->index(QDir::homePath()));
    }

}

void FileExplorer::slotShowMenu(const QPoint &pos) {
    touched_index = FileView->indexAt(pos);
    if (touched_index.isValid()/* && index.row() % 2 == 0 */) {
        //if(!FileModel->isDir(touched_index)){
        viewMenu->exec(FileView->viewport()->mapToGlobal(pos));


        // file touched, cannot create dir !!!!
        // else
    }
    // not touched in TreeView
    else{

    }
}

void FileExplorer::slotExpand() {
    FileView->expandAll();
}

void FileExplorer::slotCollapse() {
    FileView->collapseAll();
}

void FileExplorer::slotRemove() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove", "Are you sure you want to remove? ", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        FileModel->remove(touched_index);
    }else{
        return;
    }
}

void FileExplorer::slotRename() {
    window = new NewWindow(this);
    window->rename_Window(FileModel->fileName(touched_index));
    connect(window, SIGNAL(PartialWidgetDestroyed()), this, SLOT(slotCreate()));
}

void FileExplorer::slotNewDir() {
    window = new NewWindow(this);
    window->dir_Window();
    connect(window, SIGNAL(PartialWidgetDestroyed()), this, SLOT(slotCreate()));
    // no window delete needed, private function object ??????
}

void FileExplorer::slotNewFile() {
    window = new NewWindow(this);
    window->file_Window();
    connect(window, SIGNAL(PartialWidgetDestroyed()), this, SLOT(slotCreate()));
}

void FileExplorer::slotDuplicate() {
    FileManager fmanager;
    QString path = FileModel->filePath(touched_index);
    fmanager.duplicate(path);
}

void FileExplorer::slotNewC_CPP_CLASS() {
    window = new NewWindow(this);
    window->C_CPP_CLASS_Window();
    connect(window, SIGNAL(PartialWidgetDestroyed()), this, SLOT(slotCreate()));
}

void FileExplorer::slotCreate() {
    QString dirname = window->getDir();
    QString CPP_name = window->getCPP();
    QString H_name = window->getH();
    QString file_name = window->getName();
    QString new_rename = window->getNewName();

    if(!dirname.isEmpty()){
        FileModel->mkdir(touched_index, dirname);
    }
    if(!file_name.isEmpty()){
        FileManager fmanager;
        // dir touched
        if(FileModel->isDir(touched_index)){
            QString path = FileModel->filePath(touched_index);
            fmanager.write(path + "/" + file_name, "");
        }
        else{
            // file touched, create in the same location
            QString path = FileModel->filePath(touched_index);
            int len = FileModel->fileName(touched_index).length();
            path.remove(path.length() - len, len);      // path without touched file name
            fmanager.write(path + "/" + file_name, "");
        }
    }
    if(!CPP_name.isEmpty()){
        FileManager fmanager;
        // dir touched
        if(FileModel->isDir(touched_index)){
            QString path = FileModel->filePath(touched_index);
            fmanager.write(path + "/" + CPP_name, SourceTemplates::CPP);
        }
        else{
            // file touched, create in the same location
            QString path = FileModel->filePath(touched_index);
            int len = FileModel->fileName(touched_index).length();
            path.remove(path.length() - len, len);      // path without touched file name
            fmanager.write(path + "/" + CPP_name, SourceTemplates::CPP);   // .cpp template
        }
    }
    if(!H_name.isEmpty()){
        FileManager fmanager;
        // dir touched
        if(FileModel->isDir(touched_index)){
            QString path = FileModel->filePath(touched_index);
            fmanager.write(path + "/" + H_name, SourceTemplates::H);
        }
        else{
            // file touched, create in the same location
            QString path = FileModel->filePath(touched_index);
            int len = FileModel->fileName(touched_index).length();
            path.remove(path.length() - len, len);      // path without touched file name
            fmanager.write(path + "/" + H_name, SourceTemplates::H);   // .h template
        }
    }
    if(!new_rename.isEmpty()){
        FileManager fmanager;
        fmanager.rename(FileModel->filePath(touched_index), new_rename);
    }
}

void FileExplorer::slotBack() {
    // FileView->setHeader(new QHeaderView());
}

void FileExplorer::slotSetDefaultDir() {
    QFileInfo info = FileModel->fileInfo(touched_index);
    if(info.isDir()){
        setRootDirectory(info.dir().absolutePath());
    }
}

// on enter
void FileExplorer::slotTreeSearch() {
    /*
    QSettings settings("Evolution");
    QStringList sources = settings.value("Evolution/sources").toStringList();
    QStringList other_files = settings.value("Evolution/other_files").toStringList();
    */

    // FileView->findChild<QString>(searchBox->text());
}

void FileExplorer::slotCopyFileContent() {

}

void FileExplorer::slotCopyFilePath() {

}

void FileExplorer::slotCopyFileName() {

}


