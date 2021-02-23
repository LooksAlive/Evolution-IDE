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

    void setRootDirectory(const QString &path) const;

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

    void slotExpand() const;
    void slotCollapse() const;
    void slotRemove();
    void slotRename();
    void slotDuplicate();
    void slotNewDir();
    void slotNewFile();// ordinary file ( .txt, ... )
    void slotNewC_CPP_CLASS();

    void slotCopyFileContent() const;
    void slotCopyFilePath() const;
    void slotCopyFileName() const;

    void slotCreate();// private variables, used for all operations

    void slotBack() const;
    void slotSetDefaultDir() const;
    void slotTreeSearch();
};

#endif // FILEEXPLORER_H
