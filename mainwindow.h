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

#include <QPlainTextEdit>
#include <QToolButton>
#include <QUrl>

#include "Widgets/FileDock/filedock.h"
#include "Widgets/FileExplorer/fileexplorer.h"
#include "Widgets/PlainTextEdit/GoToLineColumn.h"// will be used in main window
#include "Widgets/PlainTextEdit/InvitationScreen.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Settings/settingswindow.h"
#include "Widgets/Tab/tab.h"

#include "Widgets/CodeInfoDock/CodeInfoDock.h"
#include "Widgets/ConsoleDock/consoledock.h"
#include "Widgets/FindReplace/findreplace.h"
#include "Widgets/SearchBox/SearchBox.h"

#include "Widgets/Git/GitDock.h"

#include "EnvironmentSettings.h"
#include "Widgets/Converter/converter.h"
#include "Widgets/ProgressBar/ProgressBar.h"
#include "Widgets/Settings/CmakeGenerator/cmakegenerator.h"
#include "Widgets/Notifier/ContactDeveloper.h"
#include "commandlineexecutor.h"
#include "filemanager.h"
#include "highlighter.h"
//#include "Clang/ClangBridge.h"

#include "Debugger/lldbBridge.h"
#include "Widgets/BinaryInfo/binaryview.h"
#include "Widgets/HexView/hexview.h"
#include "Widgets/NodeView/NodeView.h"
//#include "Debugger/Decompiler"

#include "Widgets/Education/Education.h"

#include "Debugger/DebugWatchDock.h"
#include "Debugger/DebuggerDock.h"

#include <QDebug>
#include <QAction>

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

class ClangBridge;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // describes what iss happening at moment in statusbar beside progressbar
    std::string progress_action;

private:
    DebuggerDock *debuggerDock;
    DebugWatchDock *debuggerWatchDock;

    // top menu bar
    QMenuBar *menuBar;

    /* vertical_bar stack  */
    QStackedWidget *vertical_stack;

    Ui::MainWindow *ui;
    /* base tab widget for plaintext */
    Highlighter *highlighter;

    CommandLineExecutor *executor;

    // top tool bar
    QToolBar *topToolBar;
    SearchBox *searchBox;

    /* toolbar -> views */
    QToolBar *vertical_bar;// all kind of views: hex, binary, debugger, decompiler ---> widgets
    // editorView are Tabs
    HexView *hexview;
    NodeView *nodeview;
    lldbBridge *debuggerBridge;
    BinaryView *binaryView;
    // Decompiler *decompilerView;

    // Status Bar
    QStatusBar *statusbar;
    ProgressBar *progress_bar;
    QToolButton *btn_position;
    QToolButton *btn_encoding;

    /* settings window */
    SettingsWindow *Settings;

    /* Tab widget stuffs */
    Tab *Tabs;
    // tracking current Tab widget for many other actions on it
    // connected when new file is created and tab changed, disconnected when deleted
    PlainTextEdit *currentWidget = nullptr;
    InvitationScreen *invitation_screen;
    // file manager, get recursive files from dir only if dir is opened, provided
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
    ClangBridge *clangBridge;// initialized in SetupCodeInfoDock;   has to be pointer, on heap
    LinterDock *Linter;
    RefactoringDock *Refactor;

    /* Converter - small widget */
    Converter *converter;
    ContactDeveloper *contactDeveloper;

    Education *education;

    GitDock *gitDock;

    void dragEnterEvent(QDragEnterEvent *drag_event) override;
    void dropEvent(QDropEvent *drop_event) override;


    void SetupTabWidget();
    void SetupMenuBar();
    void SetupToolBar();
    void SetupStatusBar();
    void SetupFileExplorer();
    void SetupFileDocker();
    void SetupCompileDock();
    void SetupCodeInfoDock();
    void SetupEducationDock();
    void SetupGitDock();

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

    // indicates wheather in file open actions ignore filepath ad insert own content, TODO: make not save able, editable
    bool SampleLoading = false;
    bool AssemblyLoading = false;

    QToolButton *editorViewButton;
    QToolButton *nodeViewButton;
    QToolButton *hexViewButton;
    QToolButton *debuggerViewButton;
    QToolButton *binaryViewButton;

    // when switching view, since its based on QToolButtuns, we cant uncheck the one we came from,
    // because it would be mess here.
    void uncheckAllVerticalTabButtons();

private slots:

    // PlainTextEdit text operations
    void slotToggleComment();

    void UpdateParameter();

    void CreateFile();

    void OpenFile();
    void OpenFile(const QString &filepath, const bool &readAndSetDocument = true);
    void OpenFile(const QModelIndex &);
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

    void showInvitationScreen();

    void ChangeTabIndexInList(int, int);
    void DeleteTabFromList(int);
    void UpdateCurrentIndex(int);
    void UpdateCurrentIndex(QListWidgetItem *);
    void UpdateCurrentIndexOnDelete(int);


    // commandline executor , for now ... later programmatically
    void slotBuild();
    void slotRun();
    void slotStopProcess();
    void slotClangFormat();
    void slotClangTidy();
    void slotClangCheck();
    void slotClangDocGenerate();
    void slotValgrind();

    void slotPullRequest();
    void slotCommitRequest();
    void slotPushRequest();


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

    void slotShowFindReplaceDock();
    void slotCursorPositionChanged();

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
    void slotCreateBreakPoint(const int &);
    void slotDeleteBreakPoint(const int &);

    void slotSetBreakpointAtLine();
    void slotShowBreakpointsList();
    void slotShowAttachToProcess();

    // Education dock --- doubleclick
    void slotOpenCppSample(QListWidgetItem *item);
    void slotOpenCppUserSample(QListWidgetItem *item);

    void slotOpenLinterFile(QListWidgetItem *item);
    void slotOpenReferenceFile(QTreeWidgetItem *item, int column);

    void slotUpdateDebuggerFilePath(const QString &, const int &, const int &);
    void slotOpenUrl(const QUrl &);

private:
    // files operation variables
    bool CHANGES_IN_PROJECT = false;
    bool ALWAYS_SAVE = false;
};





#endif // MAINWINDOW_H
