#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

/*
 * lldb functions + connections with private slots are here
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

#include <QFormLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <QFileIconProvider>

#include "lldb/API/LLDB.h"

#include "DebugWatchDock.h"
#include "DebuggerDock.h"
#include "TaskWidget/taskwidget.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"
#include "filemanager.h"

using namespace lldb;


// sources:  https://stackoverflow.com/questions/8063434/qt-run-independent-thread-from-other-thread

class lldbBridge;

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
    explicit Runner(std::shared_ptr<lldbBridge> Deb = nullptr, QObject *parent = nullptr);
    ~Runner() = default;

    void runDebugSession();

private:
    std::shared_ptr<lldbBridge> debugger;
};

class lldbBridge : public QObject {
    Q_OBJECT
public:
    explicit lldbBridge(DebuggerDock *dock, DebugWatchDock *watchDock, QObject *parent = nullptr);
    ~lldbBridge();

    void setProjectFilePaths(const QStringList &paths) { sources = paths; }
    QStringList sources;
    void setEditors(Tab *tabs, PlainTextEdit *editor) {
        tab = tabs;
        currentEdit = editor;
    };
    Tab *tab;
    PlainTextEdit *currentEdit;
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
    //BreakPointListWindow *DialogBreakPoint_List = nullptr;

    QWidget *manual_window;
    QLineEdit *line_input;
    const char *file_path;

private:
    DebuggerDock *Dock;
    DebugWatchDock *WatchDock;

    void connectDockWidgets();

    // this will be called when process stopped, insert threads into box
    void collectThreads();
    void fillCallStack();

    BreakPointListWindow *DialogBreakPoint_List;


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

    void slotAddWatch();
    void slotRemoveWatch();
    void slotModifyWatch();

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
