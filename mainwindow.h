#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
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
#include <QString>

#include <QToolButton>

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/FileExplorer/fileexplorer.h"
#include "Widgets/FileDock/filedock.h"
#include "Widgets/Tab/tab.h"
#include "Widgets/Settings/settingswindow.h"

#include "Widgets/ConsoleDock/findreplace.h"
#include "Widgets/ConsoleDock/consoledock.h"

#include "EnvironmentSettings.h"
#include "highlighter.h"
#include "filemanager.h"
#include "Widgets/Converter/converter.h"
#include "commandlineexecutor.h"

#include "Widgets/HexView/hexview.h"
#include "Debugger/debuggerdock.h"
#include "Widgets/BinaryInfo/binaryview.h"
//#include "Debugger/Decompiler"

#include <QDebug>

#include <iostream>
#include <vector>


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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:

    /* vertical_bar stack  */
    QStackedWidget *vertical_stack;

    Ui::MainWindow *ui;
    /* base tab widget for plaintext */
    Highlighter* highlighter;

    /* toolbar -> views */
    QToolBar *vertical_bar;     // all kind of views: hex, binary, debugger, decompiler ---> widgets

    // editorView are Tabs
    HexView *hexview;
    DebuggerDock *debuggerView;
    BinaryView *binaryView;
    // Decompiler *decompilerView;

    /* settings window */
    SettingsWindow *Settings;

    /* Tab widget stuffs */
    Tab* Tabs;
    /* file manager */
    FileManager file_manager;

    /* explorer stuffs */
    FileExplorer *Explorer;
    /* Docker stuffs */
    FileDock *Docker;

    /* Compile dock stuffs */
    ConsoleDock *ConsoleOutput;
    FindReplaceWidget *find_replace;

    /* Converter - small widget */
    Converter *converter;

    void dragEnterEvent(QDragEnterEvent* drag_event) override;
    void dropEvent(QDropEvent* drop_event) override;


    void SetupTabWidget();
    void SetupMenuBar();
    void SetupToolBar();
    void SetupFileExplorer();
    void SetupFileDocker();
    void SetupCompileDock();

    void SetupVerticalBar();
    void SetupDebuggerView();
    void SetupBinaryView();
    //void SetupDecompilerView();

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

    void SetupSettingsWindow();
    void SetupConverter();
    void SetFont();

    void showEditorView();
    void showHexView();
    void showDebuggerView();
    void showBinaryView();
    void showDecompilerView();

    void ChangeTabIndexInList(int, int);
    void DeleteTabFromList(int);
    void UpdateCurrentIndex(int);
    void UpdateCurrentIndex(QListWidgetItem*);
    void UpdateCurrentIndexOnDelete(int);


    // commandline executor , for now ... later programatically
    void slotBuild();
    void slotRun();
    void slotClangFormat();
    void slotClangTidy();
    void slotClangCheck();
    void slotValgrind();
    void slotGdbGui();


    void slotCut();
    void slotCopy();
    void slotUndo();
    void slotRedo();
    void slotPaste();
    void slotSelectAll();
    void slotClear();
};





#endif // MAINWINDOW_H
