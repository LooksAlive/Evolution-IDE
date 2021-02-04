#include <QSettings>
#include "icons/IconFactory.h"
#include "debuggerwidget.h"

DebuggerWidget::DebuggerWidget(QWidget *parent) : QWidget(parent)
{
    MainWindowLayout = new QHBoxLayout();
    SourceConsoleLayout = new QVBoxLayout();
    setWindowTitle("Debugger View");
    createConsole();
    createDebugVariableWindow();
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

void DebuggerWidget::createConsole(){

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

    console_out_in->addWidget(debug_output);
    console_out_in->addWidget(args_input);

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
    DebugToolBar = new QToolBar(this);
    DebugToolBar->setContentsMargins(0,0,0,0);

    btn_StartDebug = new QToolButton(this);
    btn_StartDebug->setIcon(QIcon(IconFactory::StartDebug));
    btn_StartDebug->setFixedSize(26, 26);
    connect(btn_StartDebug, SIGNAL(clicked()), this, SLOT(slotStartDebug()));
    DebugToolBar->addWidget(btn_StartDebug);

    btn_StopDebug = new QToolButton(this);
    btn_StopDebug->setIcon(QIcon(IconFactory::StopDebug));
    btn_StopDebug->setFixedSize(26, 26);
    connect(btn_StopDebug, SIGNAL(clicked()), this, SLOT(slotStopDebug()));
    DebugToolBar->addWidget(btn_StopDebug);
    DebugToolBar->addSeparator();

    btn_RunToCursor = new QToolButton(this);
    btn_RunToCursor->setIcon(QIcon(IconFactory::RunToCursor));
    btn_RunToCursor->setFixedSize(26, 26);
    connect(btn_RunToCursor, SIGNAL(clicked()), this, SLOT(slotRunToCursor()));
    DebugToolBar->addWidget(btn_RunToCursor);

    btn_StartDebug->setEnabled(true);  // at start i can run a process, but not stop it
    btn_StopDebug->setEnabled(false);
    btn_RunToCursor->setEnabled(false);

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
    btn_StepInstruction->setText(" Step Instruction");
    btn_StepInstruction->setIcon(QIcon(IconFactory::StepInstruction));
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

    // at start i can not do anything, only when the process is in progress and breakpoint is hit or else
    btn_StepOver->setEnabled(false);
    btn_StepInto->setEnabled(false);
    btn_StepInstruction->setEnabled(false);
    btn_Continue->setEnabled(false);
    btn_StepOut->setEnabled(false);

    ControlPanel->addStretch();
}

void DebuggerWidget::createDebugVariableWindow() {

    debug_variable_window = new QWidget(this);
    var_layout = new QVBoxLayout(this);
    thread_box = new QComboBox(this);
    debug_variable_window->setMaximumWidth(250); // for now ... later change responsibly

    all_variables = new QListWidget(this);
    variable_description = new QListWidget(this);
    /*
    for (int i = 0; i <= 5; i++) {
        all_variables->addItem("nieco nabuduce");
        variable_description->addItem("nieco nabuduce");
    }
    */

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

    var_layout->addWidget(thread_box);
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
    start();
    // running
    // also make sure this feature is done by program to exit normally, but remain in error states
}

void DebuggerWidget::slotStopDebug() {
    stop();
    // can run again
    btn_StartDebug->setEnabled(true);
}

void DebuggerWidget::slotRunToCursor() {
    // later
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotStepOver() {
    stepOver();
}

void DebuggerWidget::slotStepInto() {
    stepInto();
}

void DebuggerWidget::slotStepOut() {
    stepOut();
}

void DebuggerWidget::slotStepInstruction() {
    stepInstruction();
}

void DebuggerWidget::slotContinue() {
    Continue();
}
// -----------------------------------------------------------------------------------------

void DebuggerWidget::slotCmdlineExecute() {
    std::string res = executeDebuggerCommand(args_input->text().toStdString());
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
    executable = exe_file_path.c_str();
    qDebug() << exe_file_path.c_str();
    // debugger instance must be initialized before -->  void init()
    Target = Debugger.CreateTarget(executable);
    //QSettings settings("Evolution");
    //debugger.executable = settings.value("Evolution/executable_path").toString().toStdString().c_str();
    //qDebug() << debugger.executable;
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
    for (int i = 0; i < BreakPointList.size(); i++) {
        //QListWidgetItem *item = new QListWidgetItem(break_list, i);
        QTreeWidgetItem *item = new QTreeWidgetItem(i);
        item->setIcon(0, QIcon(IconFactory::BreakPoint));  // icon to begining
        //QString info = QString("ID: ") + QString::number(debugger.BreakPointList[i].break_id) + ", line: " +
        //        QString::number(debugger.BreakPointList[i].line) + " File: " + debugger.BreakPointList[i].filename;
        item->setText(0, QString::number(BreakPointList[i].break_id));
        item->setText(1, BreakPointList[i].filename);
        item->setText(2, QString::number(BreakPointList[i].line));
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
        setBreakpoint(file_path, line);
    }
    window->close();
}

void DebuggerWidget::showTaskManager() {
    TaskWidget *task = new TaskWidget(this);
    task->setFilterVisable(true);
    task->show();
    // debugger.attachToRunningProcess(some id);    // add signal from task and slot from here
}

void DebuggerWidget::addThread() {
    //int num = Process.GetNumThreads();
    //std::cout << num;
    for (int i = 0; i < Process.GetNumThreads(); i++) {
        auto thread = Process.GetThreadAtIndex(i);
        thread_box->insertItem(thread.GetThreadID(), QString::fromStdString(thread.GetName()));
    }


    //auto t = Process.GetSelectedThread();
    //thread_box->insertItem(t.GetThreadID(), QString::fromStdString(t.GetName()));
}

void DebuggerWidget::enableDebuggerButtons(){
    btn_StartDebug->setEnabled(false);
    btn_StopDebug->setEnabled(true);
    btn_RunToCursor->setEnabled(true);

    btn_StepOver->setEnabled(true);
    btn_StepInto->setEnabled(true);
    btn_StepInstruction->setEnabled(true);
    btn_Continue->setEnabled(true);
    btn_StepOut->setEnabled(true);
}

void DebuggerWidget::disableDebuggerButtons(){
    btn_StartDebug->setEnabled(true);
    btn_StopDebug->setEnabled(false);
    btn_RunToCursor->setEnabled(false);

    btn_StepOver->setEnabled(false);
    btn_StepInto->setEnabled(false);
    btn_StepInstruction->setEnabled(false);
    btn_Continue->setEnabled(false);
    btn_StepOut->setEnabled(false);
}










// lldb
// ****************************************************************************************

// have to ensure some things are done, when closed
DebuggerWidget::~DebuggerWidget() {
    //lldb::pid_t pid = Process.GetProcessID();
    debug_output->appendPlainText("Killing process ");
    Process.Kill();
    SBDebugger::Terminate();
}

void DebuggerWidget::init() {

    error = SBDebugger::InitializeWithErrorHandling();
    Debugger = SBDebugger::Create();
    // Create a debugger instance so we can create a target
    if (!Debugger.IsValid()){
        debug_output->appendPlainText("Error: failed to create a debugger object\n");
        SBStream str;
        Debugger.GetDescription(str);
        debug_output->appendPlainText(str.GetData());
        std::cout << str.GetData();
        //return;
    }
    // this is called when executable is set from outside, for know, only testing functionality
    Target = Debugger.CreateTarget(executable);

    listener = Debugger.GetListener();
    //SBLaunchInfo launch_info(nullptr);
    //launch_info.SetExecutableFile(filespec, true);
    //launch_info.SetListener(listener);

    strm.RedirectToFileHandle(stdout, false);


    if(addr_cstr == nullptr) {
        debug_output->appendPlainText("empty address");
        //return;
    }
// The second argument in the address that we want to lookup
//lldb::addr_t file_addr = strtoull(addr_cstr, nullptr, 0);
// Create a target using the executable.
//SBTarget target = debugger.CreateTarget(executable, arch, platform,
//                                       add_dependent_libs, error);

    if (!error.Success()) {
        std::cout <<"error: " << error.GetDescription(strm) << std::endl;
        //return;
    }

    SBModuleSpec module_spec;
    module_spec.SetFileSpec(SBFileSpec(executable));
    if(!module_spec.IsValid()){
        std::cout <<"Loaded not valid executable, Exiting " << std::endl;
        return;
    }
    SBModule module(module_spec);

    const size_t num_sections = module.GetNumSections();
    for (size_t sect_idx = 0; sect_idx < num_sections; ++sect_idx) {
        SBSection section = module.GetSectionAtIndex(sect_idx);
        std::cout <<"section " << sect_idx << ": " << section.GetName() << std::endl;
        if(section.GetNumSubSections() > 0){
            for (size_t sect_idxs = 0; sect_idxs < section.GetNumSubSections(); ++sect_idxs)
                std::cout <<"subsection of " << sect_idxs << ": " << section.GetSubSectionAtIndex(sect_idxs).GetName() << std::endl;
        }
    }
}

void DebuggerWidget::start() {
    // clear all first
    debug_output->clear();
    thread_box->clear();
    variable_description->clear();
    all_variables->clear();

    init();

    if (!Target.IsValid()) {
        debug_output->appendPlainText("Cannot start a debugger process with an invalid target: ");
        std::cout << "Cannot start a debugger process with an invalid target: \n";
        std::cout << executable;
        debug_output->appendPlainText(executable);
        //return;
    }

    setBreakpoint("/home/adam/Desktop/sources/Evolution-IDE/main.cpp", 23);
    //setBreakpoint("/home/adam/Desktop/sources/Evolution-IDE/main.cpp", 27);

    //Process = Target.LaunchSimple(nullptr, nullptr, nullptr);
    //Process = Target.Launch(launch_info, error);
    Process = Target.Launch(listener, nullptr, nullptr, nullptr, nullptr,
                            nullptr, nullptr, 0, false, error);

    if(!Process.IsValid()){
        debug_output->appendPlainText("Process is invalid \n");
        std::cout << "Process is invalid \n";
        SBStream str;
        Process.GetDescription(str);
        std::cout << str.GetData();
        //return;
    }


    //worker = new QThread(this);
    //worker->create(&DebuggerWidget::setProcessInterruptFeatures, this);
    //moveToThread(worker);

    //connect(worker, &QThread::started, this, &DebuggerWidget::setProcessInterruptFeatures);
    //connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    //worker->start();
    //setProcessInterruptFeatures();
    std::thread debug_thread(&DebuggerWidget::setProcessInterruptFeatures, this); // , "Debug_session"
    debug_thread.detach(); // join
    // do not join, since i do not want to want for thread to end
}

void DebuggerWidget::stop() {
    Process.Stop();
}

void DebuggerWidget::setProcessInterruptFeatures() {
    const uint32_t event_timeout_secs = 10; // Wait for 10 seconds for an event. You can set this to be longer if you want like UINT32_MAX to wait forever.
    bool done = false;
    while (!done) {
        SBEvent event;
        if (listener.WaitForEvent(event_timeout_secs, event)) {
            if (SBProcess::EventIsProcessEvent(event)) {
                // Handle process event
                done = HandleProcessEvent(event);
            }
        }
    }
    std::cout << "process ended";
    //uint32_t lldb::SBEvent::GetType() const;
}

bool DebuggerWidget::HandleProcessEvent(SBEvent &event) {

    // auto thread = getCurrentThread();
    switch (event.GetType()) {
        case lldb::SBProcess::eBroadcastBitStateChanged:
            // add all threads into threads view
            addThread();
            storeFrameData(getCurrentFrame());

            return HandleProcessStateChangeEvent(event);

        case lldb::SBProcess::eBroadcastBitSTDOUT:
            //return HandleProcessSTDOUTEvent(event);
            break;

        case lldb::SBProcess::eBroadcastBitSTDERR:
            //return HandleProcessSTDERREvent(event);
            break;
    }
    return false; // Not done, don't exit main loop
}

bool DebuggerWidget::HandleProcessStateChangeEvent(SBEvent &event) {
    SBProcess process = SBProcess::GetProcessFromEvent(event);
    StateType state = SBProcess::GetStateFromEvent(event);


    switch (state) {
        case eStateAttaching: ///< Process is currently trying to attach
            // Maybe you put up a progress dialog in case attach takes a while?
            std::cout << "attaching to process";
            debug_output->appendPlainText("attaching to process");
            break;
        case eStateLaunching: ///< Process is in the process of launching
            // Maybe you put up a progress dialog in case launch takes a while?
            std::cout << "launching";
            debug_output->appendPlainText("Launching");
            break;
        case eStateStopped:   ///< Process is stopped and can be examined
            debug_output->appendPlainText("Process has stopped and can be examined");
            enableDebuggerButtons();
            
            // retrieve data first
            //storeFrameData(getCurrentFrame());
            //setThreads();

            HandleProcessStopped(event, process);
            // since from this point the process will stop i can only stepping or continue
            //process.Stop();
            //return true;
            break;
        case eStateRunning:   ///< Process is now running and can't be examined
            // Update your UI maybe and disable the play and step buttons so the user
            // can't try to run the program while it is already running
            isRunning = true;
            btn_StartDebug->setEnabled(false);
            isRunning = true;
            std::cout << "running";
            debug_output->appendPlainText("Process is running");
            break;
        case eStateDetached:  ///< Process has been detached and can't be examined.
            // Update your GUI top indicate you are no longer debugging since LLDB
            // has detached from your process.
            std::cout << "process has been detached";
            debug_output->appendPlainText("Process has been detached");

            // If you return true, then this will cause the event loop the exit.
            // This will work well if you only ever debug one process at a time.
            // If you are debugging multiple, you can return false.
            return true;
        case eStateExited:    ///< Process has exited and can't be examined.
            // Update your GUI top indicate you are no longer debugging since your
            // process has run to completion and has exited
            isRunning = false;
            std::cout << "Process exited normally";
            debug_output->appendPlainText("Process exited normally");
            disableDebuggerButtons();

            // library loaded into memory no needed anymore
            //Process.Kill();
            //SBDebugger::Terminate();
            // If you return true, then this will cause the event loop the exit.
            // This will work well if you only ever debug one process at a time.
            // If you are debugging multiple, you can return false.
            return true;
        default:
            break;
    }
    return false; // Not done, don't exit main loop
}

void DebuggerWidget::HandleProcessStopped(SBEvent &event, SBProcess &process) {
    if (SBProcess::GetRestartedFromEvent(event)) {
        // Process is automatically restarted due to script or breakpoint action.
        // Don't update the GUI because we will soon receive a eStateRunning state...
        return;
    }

    std::string position = frameGetLocation(getCurrentFrame());

    const uint32_t num_threads = process.GetNumThreads();
    for (uint32_t thread_idx=0; thread_idx<num_threads; ++thread_idx) {
        SBThread thread = process.GetThreadAtIndex(thread_idx);
        // Get the thread stop description by using a stream
        SBStream description_stream;
        thread.GetDescription(description_stream, true);
        const char *description = description_stream.GetData();
        // You can look at description and use this string in your GUI if it has the contents you want.
        // This will be formatted using the "thread-format" which can be changed with "settings set":
        //   settings set thread-format ....

        // You can also get an enumeration for why the thread has stopped using:
        const StopReason thread_stop_reason = thread.GetStopReason();
        // Each stop reason might have additional data associated with it. See SBThread.h line 54 for details.
        // You can use this following SBThread functions to extract the data:
        //    size_t SBThread::GetStopReasonDataCount();
        //    uint64_t SBThread::GetStopReasonDataAtIndex(uint32_t idx);

        const size_t stop_reason_data_count = thread.GetStopReasonDataCount();
        switch (thread_stop_reason) {
            case eStopReasonInvalid:
                break;
            case eStopReasonNone:
                break;
            case eStopReasonTrace:
                break;
            case eStopReasonBreakpoint:
                // The stop reason data contains the breakpoint and breakpoint location
                // IDs that were hit. There might be more than one breakpoint that was
                // hit by the same thread, so we will want to report all breakpoints that were hit
                std::cout << "breakpoint was hit";
                debug_output->appendPlainText("breakpoint was hit: ");
                if(position != ""){
                    debug_output->appendPlainText(QString::fromStdString(position));
                }
                for (size_t i=0; i<stop_reason_data_count; i+=2) {
                    break_id_t bp_id = thread.GetStopReasonDataAtIndex(i);
                    break_id_t bp_loc_id = thread.GetStopReasonDataAtIndex(i+1);
                    SBBreakpoint bp = process.GetTarget().FindBreakpointByID(bp_id);
                    SBBreakpointLocation bp_loc = bp.FindLocationByID(bp_loc_id);
                    //bp_loc.GetAddress();
                    //bp_loc.GetThreadID();
                }
                break;
            case eStopReasonWatchpoint:
                break;
            case eStopReasonSignal:
                break;
            case eStopReasonException:
                break;
            case eStopReasonExec:
                break;
            case eStopReasonPlanComplete:
                break;
            case eStopReasonThreadExiting:
                break;
            case eStopReasonInstrumentation:
                break;
        }
    }
}

const char *DebuggerWidget::getAssembly(SBThread &thread) {
    lldb::SBFrame frame = thread.GetFrameAtIndex(0);
    lldb::SBFunction function = frame.GetFunction();
    lldb::SBInstructionList instructions;
    if (function.IsValid()) {
        // We have debug info that describes the frame's address
        instructions = function.GetInstructions(thread.GetProcess().GetTarget());
    } else {
        // We don't have debug info that describes the frame's address, maybe a symbol from the symbol table
        lldb::SBSymbol symbol = frame.GetSymbol();
        if (symbol.IsValid())
            instructions = symbol.GetInstructions(thread.GetProcess().GetTarget());
    }

    lldb::SBStream instruction_stream;
    instructions.GetDescription(instruction_stream);
    const char *disassembly = instruction_stream.GetData();

    return disassembly;
}

void DebuggerWidget::attachToRunningProcess(const int &proc_id){
    SBAttachInfo attach_info;
    attach_info.SetProcessID(proc_id);
    if(attach_info.ParentProcessIDIsValid()){
        Process = Target.Attach(attach_info, error);
    }
    if(error.Success()){
        debug_output->appendPlainText("something went wrong when attaching to process");
        //return;
    }
}


void DebuggerWidget::storeFrameData(SBFrame frame) {
    //clear();

    auto FrameList = frame.GetVariables(true,  // args
                                        true,  // locals
                                        true,  // statics
                                        true   // in scope only
    );

    for (uint32_t idx = 0; idx <= FrameList.GetSize(); idx++) {
        auto value = FrameList.GetValueAtIndex(idx);
        /*
        framedata data;
        data.name = value.GetDisplayTypeName();
        data.type = value.GetType().GetName();
        data.value = value.GetValue();
        data.description = value.GetObjectDescription();
        //value.GetSummary();
        //value.GetThread();
        */
        variable_description->insertItem(idx, value.GetValue());
        all_variables->insertItem(idx, value.GetName());

    }
}

std::string DebuggerWidget::frameGetLocation(SBFrame frame) {
    // set view to file and line
    const char *filename = frame.GetLineEntry().GetFileSpec().GetFilename();
    SBFunction function = frame.GetFunction();
    uint32_t line = frame.GetLineEntry().GetLine();
    std::string description = "location: ";
    description += std::string("file= ") + filename + " ";
    description += std::string("line= ") + char(line) + " ";
    description += std::string("address= ") + char(frame.GetPCAddress().GetOffset()) + " ";
    description += std::string("function= ") + function.GetDisplayName() + " ";

    std::cout << std::string("file= ") + filename + " ";
    std::cout << std::string("line= ") + char(line) + " ";
    std::cout << std::string("address= ") + char(frame.GetPCAddress().GetOffset()) + " ";
    std::cout << std::string("function= ") + function.GetDisplayName() + " ";

    return description;
}

void DebuggerWidget::setBreakpoint(const char *file_name, const int &line) {

    SBBreakpoint breakpoint = Target.BreakpointCreateByLocation(file_name, line);
    // make sure that it's good
    if (!breakpoint.IsValid()) {
        debug_output->appendPlainText("Breakpoint is not valid:  filename: ");
        debug_output->appendPlainText(file_name);
        //debug_output->appendPlainText(line);
        //return;
    }

    breakpoint.SetEnabled(true);


    BreakPointData data{breakpoint.GetID(), file_name, line};
    BreakPointList.push_back(data);
}

void DebuggerWidget::removeBreakpoint(const break_id_t &id) {
    if(!Target.BreakpointDelete(id)){
        debug_output->appendPlainText("Breakpoint was not deleted !");
        //return;
    }
}

void DebuggerWidget::removeBreakpoint(const char *file_name, const int &line) {

    break_id_t ID;
    for (int i = 0; i < BreakPointList.size(); i++) {
        if(BreakPointList[i].filename == file_name && BreakPointList[i].line == line){
            ID = BreakPointList[i].break_id;
        }
    }
    if(!Target.BreakpointDelete(ID)){
        debug_output->appendPlainText("Breakpoint was not deleted !");
        //return;
    }

}

std::vector<DebuggerWidget::framedata> get_var_func_info() {
    return std::vector<DebuggerWidget::framedata>();
}

DebuggerWidget::framedata get_var_func_info_update() {
    return DebuggerWidget::framedata();
}

SBThread DebuggerWidget::getCurrentThread() {
    // no needed
    if(!Process.IsValid()){
        std::cout << "process is not valid";
        debug_output->appendPlainText("not valid");
    }
    return Process.GetSelectedThread();
}

SBFrame DebuggerWidget::getCurrentFrame() {
    return getCurrentThread().GetSelectedFrame();
}

SBValue DebuggerWidget::findSymbol(const char *name){
    return Target.FindFirstGlobalVariable(name);
    // return Target.FindFunctions(name);  .....
}

void DebuggerWidget::pause() {
    if (Process.IsValid()) {
        auto err = Process.Stop();
        if (err.Fail()) {
            debug_output->appendPlainText("Failed to pause process"); // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
}

void DebuggerWidget::Continue() {
    if (Process.IsValid()) {
        auto err = Process.Continue();
        if (err.Fail()) {
            SBStream str;
            err.GetDescription(str);
            std::cout << "Failed to continue process" + std::string(str.GetData());
            debug_output->appendPlainText("Failed to continue process" + QString(str.GetData()));
            // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
    //disableDebuggerButtons();
}

void DebuggerWidget::stepOver() {
    Process.GetSelectedThread().StepOver();
    // plus later use other definition maybe, providing SBError
}

void DebuggerWidget::stepInto() {
    Process.GetSelectedThread().StepInto();
}

void DebuggerWidget::stepOut() {
    Process.GetSelectedThread().StepOut();
}

void DebuggerWidget::stepInstruction() {
    Process.GetSelectedThread().StepInstruction(true); // step_over     ; false -> into call instr.
}

std::string DebuggerWidget::executeDebuggerCommand(const std::string &args) {
    // tutorial site:
    // https://lldb.llvm.org/use/tutorial.html

    SBCommandReturnObject result;
    Debugger.GetCommandInterpreter().HandleCommand(args.c_str(), result);

    std::string output;
    if(result.Succeeded()){
        output = result.GetOutput();
    }else{
        output += result.GetError() + std::string(" ");
    }

    return output;
}
