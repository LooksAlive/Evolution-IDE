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
#include <vector>
#include <thread>

#include "lldb/API/LLDB.h"

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "filemanager.h"
#include "TaskWidget/taskwidget.h"

using namespace lldb;

class DebuggerWidget : public QWidget
{
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
    QWidget *show_window;
    QWidget *manual_window;
    QLineEdit *line_input;
    const char *file_path;

private:

    QHBoxLayout *MainWindowLayout;  // SourceConsoleLayout + debug_variable_window
    QVBoxLayout *SourceConsoleLayout;        // source_view + Console
    PlainTextEdit *source_view;

    // ------------------------------------------------------------------

    QWidget *Console;
    QHBoxLayout *MainConsoleLayout;     // ControlPanel + debug_output
    QLineEdit *args_input;
    QCompleter *completer;
    QPlainTextEdit *debug_output;
    QVBoxLayout *console_out_in;
    QSplitter *splitter;

    QToolButton *btn_StartDebug, *btn_StopDebug, *btn_RunToCursor, *btn_StepOver,
            *btn_StepInto, *btn_StepInstruction, *btn_Continue, *btn_StepOut;

    QToolBar *DebugToolBar;
    QVBoxLayout *ControlPanel;      // DebugToolBar + all buttons

    void createToolBar();
    void createControlPanel();
    void createConsole();

    // ------------------------------------------------------------------

    QWidget *debug_variable_window;
    QVBoxLayout *var_layout;
    QListWidget *all_variables;
    QListWidget *variable_description;

    QTabWidget *tab;
    QTreeView *view;

    void createDebugVariableWindow();

    QComboBox *thread_box;
    // this will be called when process stopped
    void addThread();


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
    void slotStartDebug();      // private because i need to swap view
    void slotCmdlineExecute();
    void slotSetBreakPointByManualLine();
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

    QThread *worker = nullptr;

    SBError error;
    SBStream strm;
    SBFileSpec filespec;


    SBDebugger Debugger;
    SBTarget   Target;
    SBProcess  Process;
    SBListener listener;

};

#endif // DEBUGGERDOCK_H
