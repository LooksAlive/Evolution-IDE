#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

/*
 * consist of 2 parts:
 *  1. is GUI representation
 *  2. lldb functions which are used in GUI
*/

#include <QComboBox>
#include <QCompleter>
#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPalette>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QString>
#include <QTabWidget>
#include <QThread>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTreeWidget>

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "lldb/API/LLDB.h"

#include "TaskWidget/taskwidget.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "filemanager.h"

using namespace lldb;


// sources:  https://stackoverflow.com/questions/8063434/qt-run-independent-thread-from-other-thread

class DebuggerWidget;

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
    explicit Runner(std::shared_ptr<DebuggerWidget> Deb = nullptr, QObject *parent = nullptr);
    ~Runner() = default;

    void runDebugSession();

private:
    std::shared_ptr<DebuggerWidget> debugger;
};

class BreakPointListWindow : public QWidget {
    Q_OBJECT
public:
    explicit BreakPointListWindow(QWidget *parent = nullptr);
    ~BreakPointListWindow() = default;

    QHBoxLayout *MainLayout;
    // consider view -> more breaks may be in the same file, but ....
    QTreeWidget *BpList;
    QToolBar *BpBar;

    QToolButton *remove;
    QToolButton *removeAll;
    QToolButton *mute;
    QToolButton *muteAll;

    void insertBreakPoint(const break_id_t &ID, const char *filename, const int &line) const;
    // remove not here, only clear whole list, bc. there can be situation:
    // ID:  1,2,3,4 --> remove 3  --> 1,2,4  -> will require more stuffs to do

private:
    void createWindow();
};

class DebuggerWidget : public QWidget {
    Q_OBJECT
public:
    explicit DebuggerWidget(QWidget *parent = nullptr);
    ~DebuggerWidget();

    // all file associated stuffs
    void setStartPosition(const char *filepath, const int &line);
    void setFilePosition(const SBFrame &frame);
    void setExecutable(const std::string &exe_file_path);

    // task view to attach running already process
    void showTaskManager();
    void showSetManualBreakPoint(const QString &filepath);

    // small list widget with information
    void showBreakPointsList();
    // dialog based break point list like in view
    BreakPointListWindow *DialogBreakPoint_List = nullptr;

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
    void collectThreads();

    void createBreakPointList();
    BreakPointListWindow *BreakPoint_List;


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
    void slotGoToBreakPointFile(QListWidgetItem *item);

    void slotRemoveBreakPoint();
    void slotRemoveAllBreakPoint();
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
    void pause();// when breakpoint is hit
    void Continue();

    void createBreakpoint(const char *filepath, const int &line);
    void removeBreakpoint(const char *filepath, const int &line);
    void removeBreakpoint(const break_id_t &id);

    void storeFrameData(SBFrame frame);
    SBThread getCurrentThread();
    SBFrame getCurrentFrame();
    std::string frameGetLocation(const SBFrame &frame);
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
        const char *filepath;
        int line;
    };
    std::vector<BreakPointData> BreakPointList;


    void start();   // process initialization
    void stop();    // destructor called here

    void attachToRunningProcess(const int &proc_id);


    // The first argument is the file path we want to look something up in
    const char *executable = "/home/adam/Desktop/sources/Evolution-IDE/cmake-build-debug/editor";// "/home/adam/Desktop/SKK/cmake-build/executable"
    const char *addr_cstr = "#address_to_lookup";
    const bool add_dependent_libs = false;
    const char *arch = nullptr;
    const char *platform = nullptr;

    // main function
    // is breakpoint hit, waiting for such events
    // called when debug process starts
public slots:
    void setProcessInterruptFeatures();

private:
    void init();
    bool HandleProcessEvent(SBEvent &event);
    bool HandleProcessStateChangeEvent(SBEvent &event);
    void HandleProcessStopped(SBEvent &event, SBProcess &process);

    const char *getAssembly(SBThread thread);

    QThread *worker = nullptr;
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
