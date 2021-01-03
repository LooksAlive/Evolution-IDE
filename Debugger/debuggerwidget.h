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
#include <QString>
#include <QSplitter>
#include <QTreeView>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>


#include "Widgets/PlainTextEdit/plaintextedit.h"

class DebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebuggerWidget(QWidget *parent = nullptr);
    ~DebuggerWidget();


private:

    QHBoxLayout *MainWindowLayout;  // SourceConsoleLayout + debug_variable_window
    QVBoxLayout *SourceConsoleLayout;        // source_view + Console
    PlainTextEdit *source_view;

    // ------------------------------------------------------------------

    QWidget *Console;
    QHBoxLayout *MainConsoleLayout;     // ControlPanel + debug_output
    PlainTextEdit *debug_output;
    QSplitter *splitter;

    QToolButton *tblStartDebug, *tblStopDebug, *tblRunToCursor,
            *tblNextLine, *tblIntoLine, *tblNextInstruction,
            *tblIntoInstruction, *tblContinue, *tblSkipFunction;

    QToolBar *DebugToolBar;
    QVBoxLayout *ControlPanel;      // DebugToolBar + all buttons

    void createToolBar();
    void createControlPanel();
    void buildConsole();

    // ------------------------------------------------------------------

    QWidget *debug_variable_window;
    QVBoxLayout *var_layout;

    void buildDebugVariableWindow();

private slots:
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
