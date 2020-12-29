#include "debuggerwidget.h"

DebuggerWidget::DebuggerWidget(QWidget *parent) : QWidget(parent)
{
    top = new QHBoxLayout();
    top_layout = new QVBoxLayout();
    setWindowTitle("debugger");
    buildDebugConsole();
    buildDebugVariableWindow();
    source_view = new PlainTextEdit(this);

    top_layout->addWidget(source_view);
    top_layout->addWidget(debug_console);

    top->addLayout(top_layout);
    top->addWidget(debug_variable_window);

    setLayout(top);

}

DebuggerWidget::~DebuggerWidget(){}


void DebuggerWidget::buildDebugConsole(){

    debug_console = new QWidget();
    console_layout = new QVBoxLayout();
    QHBoxLayout *lay = new QHBoxLayout();
    debug_console->setMaximumHeight(150);

    debug_output = new PlainTextEdit(debug_console);
    step = new QPushButton(debug_console);
    step->setText("Step");
    step->setFixedSize(70, 30);
    step_over = new QPushButton(debug_console);
    step_over->setText("Step Over");
    step_over->setFixedSize(70, 30);
    step_in = new QPushButton(debug_console);
    step_in->setText("step In");
    step_in->setFixedSize(70, 30);
    step_out = new QPushButton(debug_console);
    step_out->setText("step Out");
    step_out->setFixedSize(70, 30);

    console_layout->addWidget(step);
    console_layout->addWidget(step_over);
    console_layout->addWidget(step_in);
    console_layout->addWidget(step_out);

    lay->addLayout(console_layout);
    lay->addWidget(debug_output);

    debug_console->setLayout(lay);
}

void DebuggerWidget::buildDebugVariableWindow() {
    debug_variable_window = new QWidget();
    var_layout = new QVBoxLayout();
    debug_variable_window->setMaximumWidth(150); // for now ... later change responsibly
    //debug_variable_window->setMinimumSize(350, height());
    //debug_variable_window->setMaximumSize(350, height());
    var_layout->addWidget(new PlainTextEdit);
    debug_variable_window->setLayout(var_layout);
}
