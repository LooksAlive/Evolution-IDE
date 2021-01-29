#include <QSettings>

#include "icons/IconFactory.h"
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

    debug_output->setReadOnly(true);
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
    btn_StartDebug->setIcon(QIcon(IconFactory::StartDebug));
    btn_StartDebug->setFixedSize(26, 26);
    connect(btn_StartDebug, SIGNAL(clicked()), this, SLOT(slotStartDebug()));
    DebugToolBar->addWidget(btn_StartDebug);

    btn_StopDebug=new QToolButton(this);
    btn_StopDebug->setIcon(QIcon(IconFactory::StopDebug));
    btn_StopDebug->setFixedSize(26, 26);
    connect(btn_StopDebug, SIGNAL(clicked()), this, SLOT(slotStopDebug()));
    DebugToolBar->addWidget(btn_StopDebug);
    DebugToolBar->addSeparator();

    btn_RunToCursor=new QToolButton(this);
    btn_RunToCursor->setIcon(QIcon(IconFactory::RunToCursor));
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
    btn_StepOver->setIcon(QIcon(IconFactory::NextLine));
    btn_StepOver->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOver->setFixedSize(160, 30);
    connect(btn_StepOver, SIGNAL(clicked()), this, SLOT(slotStepOver()));
    ControlPanel->addWidget(btn_StepOver);

    btn_StepInto=new QToolButton(this);
    btn_StepInto->setText(" Step Into");
    btn_StepInto->setIcon(QIcon(IconFactory::StepInto));
    btn_StepInto->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInto->setFixedSize(160, 30);
    connect(btn_StepInto, SIGNAL(clicked()), this, SLOT(slotStepInto()));
    ControlPanel->addWidget(btn_StepInto);

    btn_StepOut=new QToolButton(this);
    btn_StepOut->setText(" Step Out");
    btn_StepOut->setIcon(QIcon(IconFactory::GetOutOfFunction));
    btn_StepOut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOut->setFixedSize(160, 30);
    connect(btn_StepOut, SIGNAL(clicked()), this, SLOT(slotStepOut()));
    ControlPanel->addWidget(btn_StepOut);

    btn_StepInstruction=new QToolButton(this);
    btn_StepInstruction->setText(" Next Instruction");
    btn_StepInstruction->setIcon(QIcon(IconFactory::NextInstruction));
    btn_StepInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInstruction->setFixedSize(160, 30);
    connect(btn_StepInstruction, SIGNAL(clicked()), this, SLOT(slotStepInstruction()));
    ControlPanel->addWidget(btn_StepInstruction);

    btn_Continue=new QToolButton(this);
    btn_Continue->setText(" Continue");
    btn_Continue->setIcon(QIcon(IconFactory::Resume));
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

    for (int i = 0; i <= 5; ++i) {
        all_variables->addItem("nieco nabuduce");
        variable_description->addItem("nieco nabuduce");
    }


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
    btn_StartDebug->setEnabled(false);

    btn_StepOver->setEnabled(false);
    btn_StepInto->setEnabled(false);
    btn_StepInstruction->setEnabled(false);
    btn_Continue->setEnabled(false);
    btn_StepOut->setEnabled(false);

    debugger.start();
    // running
    // also make sure this feature is done by program to exit normally, but remain in error states
    btn_StepOver->setEnabled(true);
    btn_StepInto->setEnabled(true);
    btn_StepInstruction->setEnabled(true);
    btn_Continue->setEnabled(true);
    btn_StepOut->setEnabled(true);

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
    btn_StartDebug->setEnabled(true);
}

void DebuggerWidget::slotStopDebug() {
    debugger.stop();
    // can run again
    btn_StartDebug->setEnabled(true);

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}

void DebuggerWidget::slotRunToCursor() {
    // later
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotStepOver() {
    debugger.stepOver();

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}

void DebuggerWidget::slotStepInto() {
    debugger.stepInto();

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}

void DebuggerWidget::slotStepOut() {
    debugger.stepOut();

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}

void DebuggerWidget::slotStepInstruction() {
    debugger.stepInstruction();

    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}

void DebuggerWidget::slotContinue() {
    if(debugger.isRunning){
        debugger.Continue();
    }else{
        return;
    }
    if(debugger.report != ""){
        debug_output->appendPlainText(debugger.report.c_str());
    }
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotCmdlineExecute() {
    std::string res = debugger.executeDebuggerCommand(args_input->text().toStdString());
    debug_output->appendPlainText(res.c_str()); // hope QString takes a C string and no implicit conversion is needed
    args_input->clear();
}
// -----------------------------------------------------------------------------------------



void DebuggerWidget::setStartFilePosition(const QString &file_path, const int &line) {
    FileDirManager fmanager;
    QString content = fmanager.simple_read(file_path);  // wants a QString !!!
    source_view->setPlainText(content);
    source_view->setCursorAtLine(line);   // later maybe some effect
}

void DebuggerWidget::setFilePosition(){

}

void DebuggerWidget::setExecutable(const std::string &exe_file_path) {
    debugger.executable = exe_file_path.c_str();
    qDebug() << exe_file_path.c_str();
    //QSettings settings("Evolution");
    //debugger.executable = settings.value("Evolution/executable_path").toString().toStdString().c_str();
    //qDebug() << debugger.executable;
}

void DebuggerWidget::slotToggleBreakPoint() {
    // debugger.setBreakpoint();
}

void DebuggerWidget::showBreakPointsList() {
    QWidget *window = new QWidget(this);
    QTreeWidget *tree = new QTreeWidget(this);
    //QListWidget *break_list = new QListWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();
    window->setFixedWidth(500);
    tree->setColumnCount(3);
    tree->setHeaderLabels(QStringList() << "ID" << "File" << "Line");
    //layout->addWidget(break_list);
    layout->addWidget(tree);
    window->setWindowFlags(Qt::Dialog);
    for (int i = 0; i < debugger.BreakPointList.size(); i++) {
        //QListWidgetItem *item = new QListWidgetItem(break_list, i);
        QTreeWidgetItem *item = new QTreeWidgetItem(i);
        item->setIcon(0, QIcon(IconFactory::BreakPoint));  // icon to begining
        //QString info = QString("ID: ") + QString::number(debugger.BreakPointList[i].break_id) + ", line: " +
        //        QString::number(debugger.BreakPointList[i].line) + " File: " + debugger.BreakPointList[i].filename;
        item->setText(0, QString::number(debugger.BreakPointList[i].break_id));
        item->setText(1, debugger.BreakPointList[i].filename);
        item->setText(2, QString::number(debugger.BreakPointList[i].line));
        //break_list->addItem(item);
        tree->addTopLevelItem(item);
    }

    window->setLayout(layout);
    window->show();
}

void DebuggerWidget::showSetManualBreakPoint(const QString &filepath) {
    file_path = filepath.toStdString().c_str();
    window = new QWidget(this);
    line_input = new QLineEdit(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QFormLayout *form = new QFormLayout();
    window->setWindowFlags(Qt::Dialog);
    form->addRow("line: ", line_input);
    layout->addLayout(form);
    connect(line_input, SIGNAL(returnPressed()), this, SLOT(slotSetBreakPointByManualLine()));

    window->setLayout(layout);
    window->show();
}

void DebuggerWidget::slotSetBreakPointByManualLine() {
    int line = line_input->text().toInt();
    if(line != 0){
        debugger.setBreakpoint(file_path, line);
    }
    window->close();
}

void DebuggerWidget::showTaskManager() {
    TaskWidget *task = new TaskWidget(this);
    task->setFilterVisable(true);
    task->show();
    // debugger.attachToRunningProcess(some id);    // add signal from task and slot from here
}



