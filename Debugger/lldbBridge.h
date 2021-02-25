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
#include <QMutex>
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
#include "Delegate.h"
#include "TaskWidget/taskwidget.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"
#include "filemanager.h"

using namespace lldb;


// sources:  https://stackoverflow.com/questions/8063434/qt-run-independent-thread-from-other-thread

class lldbBridge;

/*
 * blank QObject to move thread here and run asynchronously, independent on the main thread
 * NOTE! based on experience: <<<<< in Qt we only main thread can modify Gui(widgets) >>>>>
*/

class ProcessHandler : public QObject {
    Q_OBJECT
public:
    explicit ProcessHandler(lldbBridge *lldb_bridge = nullptr, QObject *parent = nullptr);
    ~ProcessHandler() = default;

public slots:
    void setProcessInterruptFeatures();

private:
    lldbBridge *DBridge;
    bool HandleProcessEvent(SBEvent &event);
    bool HandleProcessStateChangeEvent(SBEvent &event);
    void HandleProcessStopped(SBEvent &event, SBProcess &process);

signals:
    void addMessage(const QString &msg) const;
    void enableButtons() const;
    void disableButtons() const;

    void watchPointHit() const;
    void breakPointHit() const;
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
    void setStartPosition(const char *filepath, const int &line) const;
    void setFilePosition(const SBFrame &frame);
    void setExecutable(const std::string &exe_file_path);

    // task view to attach running already process
    void showTaskManager();
    void showSetManualBreakPoint(const QString &filepath);
    // small list widget with information
    void showBreakPointsList();
    // dialog based break point list like in view
    //BreakPointListWindow *DialogBreakPoint_List = nullptr;

    QWidget *manual_BpWindow;
    QLineEdit *line_input;
    const char *file_path;

    DebuggerDock *Dock;
    // this will be called when process stopped, insert threads into box
    void collectThreads();
    void fillCallStack();

private:
    DebugWatchDock *WatchDock;
    void connectDockWidgets();

    BreakPointListWindow *DialogBreakPoint_List;

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

    void slotOpenCallStackFile(QListWidgetItem *item) const;
    void slotGoToBreakPointFile(QListWidgetItem *item) const;

    void slotRemoveBreakPoint();
    void slotRemoveAllBreakPoints();

    void slotSetVariableDescription(const QModelIndex &index);

    void slotAddWatch();
    void slotRemoveWatch();
    void slotModifyWatch();

public:
    void pause();// when breakpoint is hit
    void Continue();

    void createBreakpoint(const char *filepath, const int &line);
    void removeBreakpoint(const char *filepath, const int &line);
    void removeBreakpoint(const break_id_t &id);

    void collectFrameData(SBFrame frame);
    // has children value
    void recursiveValueIterator(SBValue value, QStandardItem *parent_row);
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

    // handle events that happens on worker thread
private slots:
    // append message to window
    void setMessage(const QString &msg) const;
    // while process pending, i should not touch for ex. step over or else
    void enableDebuggerButtons() const;
    void disableDebuggerButtons() const;
    void handleBreakPointHit();
    void handleWatchPointHit();

private:
    void init();

    const char *getAssembly(SBThread thread);

    QThread *ProcessingThread;
    ProcessHandler *processHandler;

    SBError error;
    SBStream strm;
    SBFileSpec filespec;

    SBValueList FrameVariablesList;

    SBDebugger Debugger;
    SBTarget Target;
    SBProcess Process;

public:
    SBListener listener;
};

#endif // DEBUGGERDOCK_H
