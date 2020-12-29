#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

#include <QDockWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>


#include "Widgets/PlainTextEdit/plaintextedit.h"

class DebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    DebuggerWidget(QWidget *parent = nullptr);
    ~DebuggerWidget();


private:

    QHBoxLayout *top;
    QVBoxLayout *top_layout;
    PlainTextEdit *source_view;

    QWidget *debug_console;
    QVBoxLayout *console_layout;
    PlainTextEdit *debug_output;

    QPushButton *step;
    QPushButton *step_over;

    QPushButton *step_in;
    QPushButton *step_out;

    QWidget *debug_variable_window;
    QVBoxLayout *var_layout;

    void buildDebugConsole();
    void buildDebugVariableWindow();





};




#endif // DEBUGGERDOCK_H
