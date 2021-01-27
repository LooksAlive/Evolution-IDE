#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

/*
 * console & source view on left, variable view on right
 *
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
#include <QPalette>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>


#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "filemanager.h"
#include "lldbbridge.h"

class DebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebuggerWidget(QWidget *parent = nullptr);
    ~DebuggerWidget();

    // outside function only for duplicating current opened file
    void setStartFilePosition(const QString &file_path, const int &line);
    void setExecutable(const std::string &exe_file_path);

    lldbBridge debugger;    // debugger instance for direct ...

    // small list widget with information
    void showBreakPointsList();
    void showSetManualBreakPoint(const QString &filepath);
    QWidget *window;
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
    void buildConsole();

    // ------------------------------------------------------------------

    QWidget *debug_variable_window;
    QVBoxLayout *var_layout;
    QListWidget *all_variables;
    QListWidget *variable_description;

    void buildDebugVariableWindow();

    // managed by breakpoints, position will be gathered from debug symbols
    void setFilePosition();

public slots:
    void slotStepOver();
    void slotStepInto();
    void slotStepOut();
    void slotStepInstruction();

    void slotContinue();

    void slotStartDebug();
    void slotStopDebug();
    void slotRunToCursor();

    void slotToggleBreakPoint();

private slots:

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


};




#endif // DEBUGGERDOCK_H
