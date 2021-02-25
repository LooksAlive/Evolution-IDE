#ifndef EVOLUTION_IDE_DEBUGGERDOCK_H
#define EVOLUTION_IDE_DEBUGGERDOCK_H

#include <QComboBox>
#include <QCompleter>
#include <QDockWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

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

    void insertBreakPoint(const uint32_t &ID, const char *filename, const int &line) const;
    // remove not here, only clear whole list, bc. there can be situation:
    // ID:  1,2,3,4 --> remove 3  --> 1,2,4  -> will require more stuffs to do

private:
    void createWindow();
};

class DebuggerDock : public QDockWidget {
    Q_OBJECT
public:
    explicit DebuggerDock(QWidget *parent = nullptr);
    ~DebuggerDock() = default;

private:
    void createConsole();
    void createToolBar();
    void createControlTitleBar();
    void createCallStackWindow();
    void createBreakPointList();

public:
    BreakPointListWindow *BreakPoint_List;

    // ------------------------------------------------------------------

    QWidget *Console;
    QHBoxLayout *MainConsoleLayout;// ControlPanel + debug_output
    QLineEdit *DebuggerPrompt;
    QCompleter *completer;
    QPlainTextEdit *debug_output;
    QVBoxLayout *console_out_in;

    QToolButton *btn_StartDebug, *btn_StopDebug, *btn_RunToCursor, *btn_StepOver,
            *btn_StepInto, *btn_StepInstruction, *btn_Continue, *btn_StepOut;
    QToolButton *btn_addWatch;

    QToolBar *DebugToolBar;
    QToolBar *TitleControlBar;

    QTabWidget *ConsoleTab;
    QWidget *DebuggerOutput;

    QTreeView *VariablesView;

    QVBoxLayout *CallStackLayout;
    // active threads
    QComboBox *ThreadBox;
    // absolute file paths
    QListWidget *CallStack;
};


#endif//EVOLUTION_IDE_DEBUGGERDOCK_H
