#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDockWidget>
#include <QTreeView>
#include <QDirModel>
#include <QDir>


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
    ~FileExplorer();

    QDirModel* FileModel;
    QTreeView* FileView;

    void setRootDirectory(const QString &path);
};

#endif // FILEEXPLORER_H
