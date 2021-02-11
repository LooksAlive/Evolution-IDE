#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QWidget>
#include <QTabWidget>
#include <QFileIconProvider>

//#include <QSyntaxHighlighter>
#include <QString>
#include <QMenu>

#include <QMessageBox>
#include <QErrorMessage>

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>

//#include <QPrinter>
#include <QListWidgetItem>
#include <QListWidget>
#include <QModelIndex>
#include <QToolBar>
#include <QTabBar>
#include <QStatusBar>

#include <QAction>
#include <QToolButton>
#include <QCloseEvent>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include <QList>
#include <QUrl>
#include <QString>
#include <QPoint>
#include <QSettings>
#include <QShortcut>

#include <QToolButton>
#include <QPlainTextEdit>

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/PlainTextEdit/GoToLineColumn.h"     // will be used in main window
#include "Widgets/FileExplorer/fileexplorer.h"
#include "Widgets/FileDock/filedock.h"
#include "Widgets/Tab/tab.h"
#include "Widgets/Settings/settingswindow.h"

#include "Widgets/FindReplace/findreplace.h"
#include "Widgets/ConsoleDock/consoledock.h"
#include "Widgets/CodeInfoDock/CodeInfoDock.h"

#include "EnvironmentSettings.h"
#include "highlighter.h"
#include "filemanager.h"
#include "Widgets/Converter/converter.h"
#include "Widgets/Settings/CmakeGenerator/cmakegenerator.h"
#include "commandlineexecutor.h"
#include "Widgets/ProgressBar/ProgressBar.h"
//#include "Clang/ClangBridge.h"

#include "Widgets/NodeView/NodeView.h"
#include "Widgets/HexView/hexview.h"
#include "Debugger/debuggerwidget.h"
#include "Widgets/BinaryInfo/binaryview.h"
//#include "Debugger/Decompiler"

#include "Widgets/Education/Education.h"

#include <QDebug>

#include <iostream>
#include <string>
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

    // describes what iss happening at moment in statusbar beside progressbar
    std::string progress_action;
    
private:

    // top menu bar
    QMenuBar *menuBar;

    /* vertical_bar stack  */
    QStackedWidget *vertical_stack;

    Ui::MainWindow *ui;
    /* base tab widget for plaintext */
    Highlighter* highlighter;

    // top tool bar
    QToolBar *topToolBar;

    /* toolbar -> views */
    QToolBar *vertical_bar;     // all kind of views: hex, binary, debugger, decompiler ---> widgets
    // editorView are Tabs
    HexView *hexview;
    NodeView *nodeview;
    DebuggerWidget *debuggerView;
    BinaryView *binaryView;
    // Decompiler *decompilerView;

    // Status Bar
    QStatusBar *statusbar;
    ProgressBar *progress_bar;
    QLabel *progress_tag;
    QToolButton *btn_position;
    QToolButton *btn_encoding;

    /* settings window */
    SettingsWindow *Settings;

    /* Tab widget stuffs */
    Tab* Tabs;
    // tracking current Tab widget for many other actions on it
    // connected when new file is created and tab changed, disconnected when deleted
    PlainTextEdit *currentWidget;
    /* file manager */
    FileDirManager file_manager;

    /* explorer stuffs */
    FileExplorer *Explorer;
    /* Docker stuffs */
    FileDock *Docker;

    /* Compile dock stuffs */
    ConsoleDock *console_dock;
    FindReplaceWidget *find_replace;

    /* code info related stuffs dock */
    CodeInfoDock *codeInfoDock;
    //ClangBridge clang_parser;     // need some arguments

    /* Converter - small widget */
    Converter *converter;

    Education *education;

    void dragEnterEvent(QDragEnterEvent* drag_event) override;
    void dropEvent(QDropEvent* drop_event) override;


    void SetupTabWidget();
    void SetupMenuBar();
    void SetupToolBar();
    void SetupStatusBar();
    void SetupFileExplorer();
    void SetupFileDocker();
    void SetupCompileDock();
    void SetupCodeInfoDock();
    void SetupEducationWidget();

    void SetupVerticalBar();
    void SetupNodeView();
    void SetupHexView();
    void SetupDebuggerView();
    void SetupBinaryView();
    //void SetupDecompilerView();

    void closeEvent(QCloseEvent*) override;

    // when switching views, make sure not necessary widget are hidden and if returned -> again shown
    void SetupDockWidgetsLayering();
    void HideAllDockWidgets();
    bool explorer_visible = false, docker_visible = false, console_dock_visible = false,
            find_replace_visible = false, code_info_dock_visible = false;
    void ShowHiddenDockWidgets();

    void LoadRegisters();

private slots:

    // PlainTextEdit text operations
    void slotToggleComment();

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
    void slotRestart();
    void CloseWindow();

    void SetupSettingsWindow();
    void SetupConverter();

    void showEditorView();
    void showNodeView();
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
    void slotClangDocGenerate();
    void slotValgrind();
    void slotGdbGui();


    void slotCut();
    void slotCopy();
    void slotUndo();
    void slotRedo();
    void slotPaste();
    void slotSelectAll();
    void slotExpand();
    void slotCollapse();
    void slotRemoveAll();
    void slotFormat();

    void slotAbout();
    void slotFullScreen();

    void slotGoToLine();
    void slotFind();

    void slotStopProcess();
    void slotTextPositionChanged();

    void slotShowEducation();


    // debugger
    void slotStartDebug();
    void slotStopDebug();
    void slotContinue();
    void slotRunToCursor();

    void slotStepOver();
    void slotStepInto();
    void slotStepInstruction();
    void slotStepOut();

    // to and from edit, debugger
    void slotToggleBreakPoint();
    // from edit to debugger;      signals: breakPointCreated, breakPointRemoved
    void slotCreateBreakPoint(const int&);
    void slotDeleteBreakPoint(const int&);

    void slotSetBreakpointAtLine();
    void slotShowBreakpointsList();
    void slotShowAttachToProcess();


private:
    // files operation variables
    bool CHANGES_IN_PROJECT = false;
    bool ALWAYS_SAVE = false;

};





#endif // MAINWINDOW_H
