#include "debuggerwidget.h"

DebuggerWidget::DebuggerWidget(QWidget *parent) : QWidget(parent)
{
    MainWindowLayout = new QHBoxLayout();
    SourceConsoleLayout = new QVBoxLayout();
    setWindowTitle("debugger");
    buildConsole();
    buildDebugVariableWindow();
    source_view = new PlainTextEdit(this);

    SourceConsoleLayout->addWidget(source_view);
    SourceConsoleLayout->addWidget(Console);

    /*
    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(Console);
    splitter->addWidget(source_view);

    SourceConsoleLayout->addWidget(splitter);
    */
    MainWindowLayout->addLayout(SourceConsoleLayout);
    MainWindowLayout->addWidget(debug_variable_window);

    MainWindowLayout->setContentsMargins(0,0,0,0);

    setLayout(MainWindowLayout);
}

DebuggerWidget::~DebuggerWidget(){}


void DebuggerWidget::buildConsole(){

    Console = new QWidget(this);
    MainConsoleLayout = new QHBoxLayout(this);
    console_out_in = new QVBoxLayout();
    debug_output = new QPlainTextEdit(this);
    args_input = new QLineEdit(this);
    completer = new QCompleter(this);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    // TODO: set data to completer somehow
    args_input->setCompleter(completer);
    // enter
    connect(args_input, SIGNAL(returnPressed()), this, SLOT(slotCmdlineExecute()));

    console_out_in->addWidget(args_input);
    console_out_in->addWidget(debug_output);

    Console->setFixedHeight(200);
    /*
    Console->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::lightGray);
    Console->setPalette(pal);
    */
    //Console->setStyleSheet("border: 1px solid rgb(0, 255, 0);");

    createControlPanel();

    MainConsoleLayout->addLayout(ControlPanel);
    MainConsoleLayout->addLayout(console_out_in);

    Console->setLayout(MainConsoleLayout);
}

void DebuggerWidget::createToolBar()
{
    DebugToolBar=new QToolBar(this);
    DebugToolBar->setContentsMargins(0,0,0,0);

    btn_StartDebug=new QToolButton(this);
    btn_StartDebug->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StartDebug.png"));
    btn_StartDebug->setFixedSize(26, 26);
    connect(btn_StartDebug, SIGNAL(clicked()), this, SLOT(slotStartDebug()));
    DebugToolBar->addWidget(btn_StartDebug);

    btn_StopDebug=new QToolButton(this);
    btn_StopDebug->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StopDebug.png"));
    btn_StopDebug->setFixedSize(26, 26);
    connect(btn_StopDebug, SIGNAL(clicked()), this, SLOT(slotStopDebug()));
    DebugToolBar->addWidget(btn_StopDebug);
    DebugToolBar->addSeparator();

    btn_RunToCursor=new QToolButton(this);
    btn_RunToCursor->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/RunToCursor.png"));
    btn_RunToCursor->setFixedSize(26, 26);
    connect(btn_RunToCursor, SIGNAL(clicked()), this, SLOT(slotRunToCursor()));
    DebugToolBar->addWidget(btn_RunToCursor);

    ControlPanel->addWidget(DebugToolBar);
}

void DebuggerWidget::createControlPanel()
{

    ControlPanel=new QVBoxLayout;
    ControlPanel->setContentsMargins(0,0,0,0);

    createToolBar();

    btn_StepOver=new QToolButton(this);
    btn_StepOver->setText(" Step Over");
    btn_StepOver->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextStep.png"));
    btn_StepOver->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOver->setFixedSize(160, 30);
    connect(btn_StepOver, SIGNAL(clicked()), this, SLOT(slotStepOver()));
    ControlPanel->addWidget(btn_StepOver);

    btn_StepInto=new QToolButton(this);
    btn_StepInto->setText(" Step Into");
    btn_StepInto->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextLine.png"));
    btn_StepInto->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInto->setFixedSize(160, 30);
    connect(btn_StepInto, SIGNAL(clicked()), this, SLOT(slotStepInto()));
    ControlPanel->addWidget(btn_StepInto);

    btn_StepOut=new QToolButton(this);
    btn_StepOut->setText(" Step Out");
    btn_StepOut->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/GetOutOfFunction.png"));
    btn_StepOut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOut->setFixedSize(160, 30);
    connect(btn_StepOut, SIGNAL(clicked()), this, SLOT(slotStepOut()));
    ControlPanel->addWidget(btn_StepOut);

    btn_StepInstruction=new QToolButton(this);
    btn_StepInstruction->setText(" Next Instruction");
    btn_StepInstruction->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/JumpFunction.png"));
    btn_StepInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInstruction->setFixedSize(160, 30);
    connect(btn_StepInstruction, SIGNAL(clicked()), this, SLOT(slotStepInstruction()));
    ControlPanel->addWidget(btn_StepInstruction);

    btn_Continue=new QToolButton(this);
    btn_Continue->setText(" Continue");
    btn_Continue->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/Continue.png"));
    btn_Continue->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_Continue->setFixedSize(160, 30);
    connect(btn_Continue, SIGNAL(clicked()), this, SLOT(slotContinue()));
    ControlPanel->addWidget(btn_Continue);

    ControlPanel->addStretch();
}


void DebuggerWidget::buildDebugVariableWindow() {

    debug_variable_window = new QWidget(this);
    var_layout = new QVBoxLayout(this);
    debug_variable_window->setMaximumWidth(250); // for now ... later change responsibly

    all_variables = new QListWidget(this);
    variable_description = new QListWidget(this);

    /*
    auto data = debugger.get_var_func_info();

    for (int i=0; i< data.size(); i++){
        all_variables->addItem(new QListWidgetItem(data[i].name.c_str()));
        QString type = "type: " + data[i].type.c_str();
        variable_description->addItem(new QListWidgetItem(type));
        for (int z=0; z< data[i].values.size(); z++){
            variable_description->addItem(new QListWidgetItem(data[i].values[z].c_str()));
        }
    }

    // small update var func also implement later
    */

    // connect(OptionsList, &QListWidget::currentRowChanged, WidgetStack, &QStackedWidget::setCurrentIndex);

    var_layout->addWidget(all_variables);
    var_layout->addWidget(variable_description);

    //var_layout->addWidget(new PlainTextEdit);
    debug_variable_window->setLayout(var_layout);

    /*
    QSplitter *watchDockContainer=new QSplitter(this);
    watchDockContainer->setOrientation(Qt::Vertical);
    QToolBar *customWatchControl=new QToolBar(watchDockContainer);
    QToolButton *addwatch =new QToolButton(watchDockContainer);
    addwatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/AddWatch.png"));
    customWatchControl->addWidget(addwatch);
    QToolButton *modifywatch=new QToolButton(watchDockContainer);
    modifywatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/ModifyWatch.png"));
    customWatchControl->addWidget(modifywatch);
    customWatchControl->addSeparator();
    QToolButton *removewatch=new QToolButton(watchDockContainer);
    removewatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/RemoveWatch.png"));

    customWatchControl->addWidget(removewatch);
    */
}

// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotStartDebug() {
    debugger.start();
}

void DebuggerWidget::slotStopDebug() {
    debugger.stop();
}

void DebuggerWidget::slotRunToCursor() {
    // later
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotStepOver() {
    debugger.stepOver();
}

void DebuggerWidget::slotStepInto() {
    debugger.stepInto();
}

void DebuggerWidget::slotStepOut() {
    debugger.stepOut();
}

void DebuggerWidget::slotStepInstruction() {
    debugger.stepInstruction();
}

void DebuggerWidget::slotContinue() {
    if(debugger.isRunning()){
        debugger.Continue();
    }else{
        return;
    }
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotCmdlineExecute() {
    debugger.executeDebuggerCommand(args_input->text().toStdString());
}
// -----------------------------------------------------------------------------------------



void DebuggerWidget::setDebugPosition(const QString &file_path, const int &line) {
    FileManager fmanager;
    QString content = fmanager.simple_read(file_path);  // wants a QString !!!
    source_view->setPlainText(content);
    source_view->setCursorAtLine(line);   // later maybe some effect
}

void DebuggerWidget::setExecutable(const std::string &exe_file_path) {
    debugger.executable = exe_file_path.c_str();
}



