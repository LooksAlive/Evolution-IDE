#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

/*
 * consist of 2 parts:
 *  1. is GUI representation
 *  2. lldb functions which are used in GUI
*/

#include <QDockWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QLineEdit>
#include <QCompleter>
#include <QPlainTextEdit>
#include <QString>
#include <QSplitter>
#include <QTreeView>
#include <QListWidget>
#include <QComboBox>
#include <QPalette>
#include <QThread>
#include <QListWidget>
#include <QTreeView>
#include <QTabWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTreeWidget>

#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "lldb/API/LLDB.h"

#include "TaskWidget/taskwidget.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "filemanager.h"

using namespace lldb;


// sources:  https://stackoverflow.com/questions/8063434/qt-run-independent-thread-from-other-thread

/*
 * blank QObject to move thread here and run asynchronously, independent on the main thread
 * this is done in start() function
 * NOTE! i tried to create c++ standard thread but it is not possible... Qt raises many errors within
 * different widgets thread is pointing to
 * It should share memory, since all main objects are accessible and deleted with app itself
*/
class Runner : public QObject {
    Q_OBJECT
public:
    explicit Runner(QObject *parent = nullptr) : QObject(parent) {}
    ~Runner() = default;
};

class DebuggerWidget : public QWidget {
    Q_OBJECT
public:
    explicit DebuggerWidget(QWidget *parent = nullptr);
    ~DebuggerWidget();

    // outside function only for duplicating current opened file
    void setStartFilePosition(const QString &path, const int &line);
    void setExecutable(const std::string &exe_file_path);

    // small list widget with information
    void showBreakPointsList();
    // task view to attach running already process
    void showTaskManager();
    void showSetManualBreakPoint(const QString &filepath);
    // TODO: inherit QWidget and new class ... + add remove button in vertical left toolBar + connect outside
    // break points list
    QWidget *BreakPointListWindow;
    // consider view -> more breaks may be in the same file, but ....
    QTreeWidget *BreakPoint_List;
    QWidget *manual_window;
    QLineEdit *line_input;
    const char *file_path;

private:
    QVBoxLayout *MainWindowLayout; // SourceConsoleLayout + debug_variable_window
    QHBoxLayout *SourceWatchLayout;// source_view + Console
    PlainTextEdit *source_view;

    // ------------------------------------------------------------------

    QWidget *Console;
    QHBoxLayout *MainConsoleLayout;// ControlPanel + debug_output
    QLineEdit *DebuggerPrompt;
    QCompleter *completer;
    QPlainTextEdit *debug_output;
    QVBoxLayout *console_out_in;
    QSplitter *splitter;

    QToolButton *btn_StartDebug, *btn_StopDebug, *btn_RunToCursor, *btn_StepOver,
            *btn_StepInto, *btn_StepInstruction, *btn_Continue, *btn_StepOut;

    QToolBar *DebugToolBar;
    QVBoxLayout *ControlPanel;// DebugToolBar + all buttons

    void createToolBar();
    void createControlPanel();
    void createConsole();

    // ------------------------------------------------------------------

    void createDebugWatchWindow();
    QWidget *WatchWindow;
    QVBoxLayout *WatchLayout;
    QListWidget *WatchListView;

    QTabWidget *ConsoleTab;
    QWidget *DebuggerOutput;

    QTreeView *VariablesView;

    void createCallStackWindow();
    void fillCallStack();
    QVBoxLayout *CallStackLayout;
    // active threads
    QComboBox *ThreadBox;
    // absolute file paths
    QListWidget *CallStack;

    // this will be called when process stopped, insert threads into box
    void fillThreadBox();


    // managed by breakpoints, position will be gathered from debug symbols
    void setFilePosition();

    // while process pending, i should not touch for ex. step over or else
    void enableDebuggerButtons();
    void disableDebuggerButtons();

public slots:
    void slotStepOver();
    void slotStepInto();
    void slotStepOut();
    void slotStepInstruction();

    void slotContinue();

    void slotStopDebug();
    void slotRunToCursor();

private slots:
    void slotStartDebug();// private because i need to swap view
    void slotCmdlineExecute();
    void slotSetBreakPointByManualLine();

    void slotOpenCallStackFile(QListWidgetItem *item);
    /*
    void onActionAddWatch();
    void onActionModifyWatch();
    void onActionRemoveWatch();

    

    void createStackView();
    void createGDBConversation();
    void createWatchLayout();
    */


// lldb:

public:
    void pause();    // when breakpoint is hit
    void Continue();

    void createBreakpoint(const char *file_name, const int &line);
    void removeBreakpoint(const char *file_name, const int &line);
    void removeBreakpoint(const break_id_t &id);

    void storeFrameData(SBFrame frame);
    SBThread getCurrentThread();
    SBFrame getCurrentFrame();
    std::string frameGetLocation(const SBFrame& frame);
    SBValue findSymbol(const char *name);

    void stepOver();
    void stepInto();
    void stepOut();
    void stepInstruction();

    std::string executeDebuggerCommand(const std::string &args);

    // data storages
    struct framedata{
        std::string type;
        std::string name;
        std::string value;
        std::string description;
    };
    // get num of frames in project -> return some data struct vector, this would need to be updated or collected at
    // once
    std::vector<framedata> get_var_func_info();         // get names, values and insert into variable widget
    framedata get_var_func_info_update();         // step by step updating

    struct BreakPointData{
        break_id_t break_id;
        const char *filename;
        int line;
    };
    std::vector<BreakPointData> BreakPointList;


    void start();   // process initialization
    void stop();    // destructor called here

    void attachToRunningProcess(const int &proc_id);


    // The first argument is the file path we want to look something up in
    const char *executable = "/home/adam/Desktop/sources/Evolution-IDE/cmake-build-debug/editor";   // "/home/adam/Desktop/SKK/cmake-build/executable"
    const char *addr_cstr = "#address_to_lookup";
    const bool add_dependent_libs = false;
    const char *arch = nullptr;
    const char *platform = nullptr;

    bool isRunning = false;

private:
    void init();
    // main function
    // is breakpoint hit, waiting for such events
    // called when debug process starts
    void setProcessInterruptFeatures();
    bool HandleProcessEvent(SBEvent &event);
    bool HandleProcessStateChangeEvent(SBEvent &event);
    void HandleProcessStopped(SBEvent &event, SBProcess &process);

    const char *getAssembly(SBThread thread);

    QThread *worker;
    Runner *runner;

    SBError error;
    SBStream strm;
    SBFileSpec filespec;


    SBDebugger Debugger;
    SBTarget Target;
    SBProcess Process;
    SBListener listener;
};

#endif // DEBUGGERDOCK_H
