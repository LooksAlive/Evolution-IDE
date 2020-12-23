#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QTabWidget>
//#include <QSyntaxHighlighter>
#include <QString>
#include <QMenu>

#include <QMessageBox>
#include <QErrorMessage>

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>

#include <QPrinter>
#include <QListWidgetItem>
#include <QListWidget>
#include <QModelIndex>
#include <QToolBar>
#include <QTabBar>

#include <QAction>
#include <QToolButton>
#include <QCloseEvent>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include <QList>
#include <QUrl>


#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "highlighter.h"
#include "Widgets/FileExplorer/fileexplorer.h"
#include "Widgets/FileDock/filedock.h"
#include "Widgets/Tab/tab.h"
#include "Widgets/Settings/settingswindow.h"

#include "Widgets/ConsoleDock/findreplace.h"
#include "Widgets/ConsoleDock/consoledock.h"


#include "EnvironmentSettings.h"


/*
#TODO:
replace default ui namespace by Evolution name,
directories -> subnamespaces of Evolution(main)
each file -> 1 class

new file for file explorer, openned docks, tab widget;
toolbar, menubar also own file

** auto generated file ui_mainwindow intergate here to avoid ui files **

-- all called by constructors there --
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    /* base tab widget for plaintext */
    Highlighter* highlighter;

    /* settings window */
    SettingsWindow *Settings;

    /* Tab widget stuffs */
    Tab* Tabs;
    /* explorer stuffs */
    FileExplorer *Explorer;
    /* Docker stuffs */
    FileDock *Docker;

    /* Compile dock stuffs */
    ConsoleDock *OutputWindow;
    FindReplaceWidget *find_replace;

    void dragEnterEvent(QDragEnterEvent* drag_event) override;
    void dropEvent(QDropEvent* drop_event) override;


    void SetupTabWidget();
    void SetupMenuBar();
    void SetupToolBar();
    void SetupFileExplorer();
    void SetupFileDocker();
    void SetupCompileDock();
    void closeEvent(QCloseEvent*) override;

    // the way to show DockWidgets in app --> select corners, do not take all bottom, top for one dock
    void SetupDockWidgetsLayering();


private slots:
    void UpdateParameter();

    void CreateFile();
    void OpenFile();
    void OpenFile(const QString&);
    void OpenFile(QModelIndex);
    void SaveFile();
    void SaveFileAs();
    void SaveAllFiles();
    void CloseFile();
    void CloseFile(int);
    void CloseAllFiles();
    void CloseWindow();

    void OpenSettingsWindow();
    void SetFont();


    void ChangeTabIndexInList(int, int);
    void DeleteTabFromList(int);
    void UpdateCurrentIndex(int);
    void UpdateCurrentIndex(QListWidgetItem*);
    void UpdateCurrentIndexOnDelete(int);

    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotSelectAll();
    void slotClear();
};





#endif // MAINWINDOW_H
