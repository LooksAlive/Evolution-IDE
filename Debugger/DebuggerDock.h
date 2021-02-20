#ifndef EVOLUTION_IDE_DEBUGGERDOCK_H
#define EVOLUTION_IDE_DEBUGGERDOCK_H


#include "debuggerwidget.h"
#include <QDockWidget>
#include <QListWidget>
#include <QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <Widgets/PlainTextEdit/plaintextedit.h>

#include "debuggerwidget.h"

class DebuggerDock : public QDockWidget {
    Q_OBJECT
public:
    explicit DebuggerDock(QWidget *parent = nullptr);
    ~DebuggerDock();

private:
    void createDock();
    void createConsole();
    void createToolBar();
    void createControlPanel();
    void createDebugWatchWindow();
    void createCallStackWindow();

    void fillCallStack();

    void createBreakPointList();
    BreakPointListWindow *BreakPoint_List;

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

    QWidget *WatchWindow;
    QVBoxLayout *WatchLayout;
    QListWidget *WatchListView;

    QTabWidget *ConsoleTab;
    QWidget *DebuggerOutput;

    QTreeView *VariablesView;

    QVBoxLayout *CallStackLayout;
    // active threads
    QComboBox *ThreadBox;
    // absolute file paths
    QListWidget *CallStack;

private slots:
    void slotOpenCallStackFile(QListWidgetItem *item);
    void slotGoToBreakPointFile(QListWidgetItem *item);

    void slotRemoveBreakPoint();
    void slotRemoveAllBreakPoint();
};


#endif//EVOLUTION_IDE_DEBUGGERDOCK_H
