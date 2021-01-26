#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDockWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCompleter>

#include "filemanager.h"
#include "SourceTemplates.h"
#include "NewWindow.h"

/*
class:
  * all settings for Explorer

calls:
  * SetupFileExplorerDock function in mainwidow connects with parent widget
    and add dockwidget as Explorer object
*/

class FileExplorer : public QDockWidget
{
    Q_OBJECT
public:
    explicit FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer() = default;

    QFileSystemModel *FileModel;
    QTreeView *FileView;

    void setRootDirectory(const QString &path);

private:
    QMenu *viewMenu;
    QMenu *copyMenu;
    NewWindow *window;

    QVBoxLayout *MainLayout;
    // button above the tree view
    QToolBar *bar_buttons;

    void createSearchBox();
    QLineEdit *searchBox;
    QCompleter *completer;

    void createMenu();

    QModelIndex touched_index;

private slots:
    // right click menu slots
    void slotShowMenu(const QPoint &pos);

    void slotExpand();
    void slotCollapse();
    void slotRemove();
    void slotRename();
    void slotDuplicate();
    void slotNewDir();
    void slotNewFile(); // ordinary file ( .txt, ... )
    void slotNewC_CPP_CLASS();

    void slotCopyFileContent();
    void slotCopyFilePath();
    void slotCopyFileName();

    void slotCreate();  // private variables, used for all operations

    void slotBack();
    void slotSetDefaultDir();
    void slotTreeSearch();

};

#endif // FILEEXPLORER_H
