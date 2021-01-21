#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDockWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>

#include <QToolButton>


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
    NewWindow *window;

    // button above the tree view
    QToolButton *collapseAll;
    QToolButton *expandAll;
    QToolButton *New;

    void newWindow();

private slots:
    // right click menu slots
    void slotExpand();
    void slotCollapse();
    void slotNew();


};

#endif // FILEEXPLORER_H
