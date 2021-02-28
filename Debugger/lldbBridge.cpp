#include "lldbBridge.h"
#include "icons/IconFactory.h"
#include <QSettings>
#include <QSharedPointer>

ProcessHandler::ProcessHandler(lldbBridge *lldb_bridge, QObject *parent) : QObject(parent), DBridge(lldb_bridge) {}


void ProcessHandler::setProcessInterruptFeatures() {
    const uint32_t event_timeout_secs = 10;// Wait for 10 seconds for an event. You can set this to be longer if you want like UINT32_MAX to wait forever.
    bool done = false;
    while (!done) {
        SBEvent event;// const
        if (DBridge->listener.WaitForEvent(event_timeout_secs, event)) {
            if (SBProcess::EventIsProcessEvent(event)) {
                // Handle process event
                done = HandleProcessEvent(event);
            }
        }
    }
    //uint32_t lldb::SBEvent::GetType() const;
}

bool ProcessHandler::HandleProcessEvent(SBEvent &event) {
    switch (event.GetType()) {
        case lldb::SBProcess::eBroadcastBitStateChanged:
            return HandleProcessStateChangeEvent(event);
            /*
            case lldb::SBProcess::eBroadcastBitSTDOUT:
                //return HandleProcessSTDOUTEvent(event);
                break;
            case lldb::SBProcess::eBroadcastBitSTDERR:
                //return HandleProcessSTDERREvent(event);
                break;
            */
        default:
            break;
    }
    return false;// Not done, don't exit main loop
}

bool ProcessHandler::HandleProcessStateChangeEvent(SBEvent &event) {
    SBProcess process = SBProcess::GetProcessFromEvent(event);
    StateType state = SBProcess::GetStateFromEvent(event);


    switch (state) {
        case eStateAttaching:///< Process is currently trying to attach
            // Maybe you put up a progress dialog in case attach takes a while?
            //DBridge->Dock->debug_output->appendPlainText("attaching to process");
            break;
        case eStateLaunching:///< Process is in the process of launching
            // Maybe you put up a progress dialog in case launch takes a while?
            //DBridge->Dock->debug_output->appendPlainText("Launching");
            break;
        case eStateStopped:///< Process is stopped and can be examined
            //DBridge->Dock->debug_output->appendPlainText("Process has stopped and can be examined");
            emit enableButtons();
            HandleProcessStopped(event, process);
            // since from this point the process will stop i can only stepping or continue
            return true;
            //break;
        case eStateRunning:///< Process is now running and can't be examined
            // Update your UI maybe and disable the play and step buttons so the user
            // can't try to run the program while it is already running
            //DBridge->Dock->btn_StartDebug->setEnabled(false);
            //DBridge->Dock->debug_output->appendPlainText("Process is running");
            break;
        case eStateDetached:///< Process has been detached and can't be examined.
            // Update your GUI top indicate you are no longer debugging since LLDB
            // has detached from your process.
            //DBridge->Dock->debug_output->appendPlainText("Process has been detached");

            // If you return true, then this will cause the event loop the exit.
            // This will work well if you only ever debug one process at a time.
            // If you are debugging multiple, you can return false.
            return true;
        case eStateExited:///< Process has exited and can't be examined.
            // Update your GUI top indicate you are no longer debugging since your
            // process has run to completion and has exited
            //DBridge->Dock->debug_output->appendPlainText("Process exited normally");
            emit addMessage("exited normally");
            emit disableButtons();

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
    return false;// Not done, don't exit main loop
}

void ProcessHandler::HandleProcessStopped(SBEvent &event, SBProcess &process) {
    if (SBProcess::GetRestartedFromEvent(event)) {
        // Process is automatically restarted due to script or breakpoint action.
        // Don't update the GUI because we will soon receive a eStateRunning state...
        return;
    }

    const uint32_t num_threads = process.GetNumThreads();
    for (uint32_t thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
        SBThread thread = process.GetThreadAtIndex(thread_idx);
        // Get the thread stop description by using a stream
        //SBStream description_stream;
        //thread.GetDescription(description_stream, true);
        //const char *description = description_stream.GetData();
        //debug_output->appendPlainText(description);
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
            case eStopReasonNone:
            case eStopReasonTrace:
                break;
            case eStopReasonBreakpoint:
                // The stop reason data contains the breakpoint and breakpoint location
                // IDs that were hit. There might be more than one breakpoint that was
                // hit by the same thread, so we will want to report all breakpoints that were hit
                //DBridge->Dock->debug_output->appendPlainText("breakpoint was hit: ");
                // DBridge->setFilePosition(thread.GetSelectedFrame());
                emit breakPointHit();
                for (size_t i = 0; i < stop_reason_data_count; i += 2) {
                    break_id_t bp_id = thread.GetStopReasonDataAtIndex(i);
                    break_id_t bp_loc_id = thread.GetStopReasonDataAtIndex(i + 1);
                    SBBreakpoint bp = process.GetTarget().FindBreakpointByID(bp_id);
                    SBBreakpointLocation bp_loc = bp.FindLocationByID(bp_loc_id);
                    //bp_loc.GetAddress();
                    //bp_loc.GetThreadID();
                }
                break;
            case eStopReasonWatchpoint:
            case eStopReasonSignal:
            case eStopReasonException:
            case eStopReasonExec:
            case eStopReasonPlanComplete:
            case eStopReasonThreadExiting:
            case eStopReasonInstrumentation:
                break;
        }
    }
}


lldbBridge::lldbBridge(DebuggerDock *dock, DebugWatchDock *watchDock, QObject *parent) : QObject(parent), Dock(dock), WatchDock(watchDock) {
    connectDockWidgets();
}

void lldbBridge::fillCallStack() {
    for (int i = 0; i <= 5; i++) {
        Dock->CallStack->insertItem(i, "some call");
    }
    // is there another wyy to do that ... extract positions, contents and files ????
    // const std::string backTrace = executeDebuggerCommand("thread backtrace all");
}

void lldbBridge::slotGoToBreakPointFile(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);
    const char *filepath = Dock->BreakPoint_List->BpList->currentItem()->text(1).toLatin1().data();
    const int line = Dock->BreakPoint_List->BpList->currentItem()->text(2).toInt();
    emit filePathUpdate(filepath, line, 1);
}

void lldbBridge::slotRemoveBreakPoint() {
    if (!Dock->BreakPoint_List->BpList->selectedItems().isEmpty()) {
        const int ID = Dock->BreakPoint_List->BpList->currentItem()->text(0).toInt();// ID
        removeBreakpoint(ID);
    }
    // otherwise raises error, of course no selected item cannot be removed or ... selected at 0
    else {
        // no idea this will works :)
        Dock->BreakPoint_List->BpList->setCurrentIndex(Dock->BreakPoint_List->BpList->rootIndex());
    }
}

void lldbBridge::slotRemoveAllBreakPoints() {
    for (unsigned int i = 0; i < BreakPointList.size(); i++) {
        //const int ID = BreakPoint_List->BpList->currentItem()->text(0).toInt(); // ID
        // assume that break IDs are increasing constantly
        if (!Target.BreakpointDelete(i)) {
            Dock->debug_output->appendPlainText("Breakpoint was not removed !");
        }
        //BreakPointList.erase(BreakPointList.begin() + i);
    }
    // TODO: figure out how to get specific item in list and remove them 1 by 1
    // clear whole list and insert remaining break points
    Dock->BreakPoint_List->BpList->clear();
    BreakPointList.clear();
}

void lldbBridge::slotEnableAllBreakPoints() {
    Target.DisableAllBreakpoints();
    for (int i = 0; i < Dock->BreakPoint_List->BpList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = Dock->BreakPoint_List->BpList->topLevelItem(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
}

void lldbBridge::slotDisableAllBreakPoints() {
    Target.DisableAllBreakpoints();
    for (int i = 0; i < Dock->BreakPoint_List->BpList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = Dock->BreakPoint_List->BpList->topLevelItem(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable /* | Qt::ItemIsEnabled */);
    }
}

void lldbBridge::slotEnableAllWatchPoints() {
    Target.EnableAllWatchpoints();
    for (int i = 0; i < WatchDock->VariableTreeValues->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = WatchDock->VariableTreeValues->topLevelItem(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
}

void lldbBridge::slotDisableAllWatchPoints() {
    Target.EnableAllWatchpoints();
    for (int i = 0; i < WatchDock->VariableTreeValues->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = WatchDock->VariableTreeValues->topLevelItem(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable /* | Qt::ItemIsEnabled */);
    }
}


// -----------------------------------------------------------------------------------------

void lldbBridge::slotStartDebug() {
    start();
    // running
    // also make sure this feature is done by program to exit normally, but remain in error states
}

void lldbBridge::slotStopDebug() {
    stop();
    // can run again
    Dock->btn_StartDebug->setEnabled(true);
}

void lldbBridge::slotRunToCursor() {
    // later
}
// -----------------------------------------------------------------------------------------

void lldbBridge::slotStepOver() {
    stepOver();
}

void lldbBridge::slotStepInto() {
    stepInto();
}

void lldbBridge::slotStepOut() {
    stepOut();
}

void lldbBridge::slotStepInstruction() {
    stepInstruction();
}

void lldbBridge::slotContinue() {
    Continue();
}
// -----------------------------------------------------------------------------------------

void lldbBridge::slotCmdlineExecute() {
    const std::string res = executeDebuggerCommand(Dock->DebuggerPrompt->text().toStdString());
    Dock->debug_output->appendPlainText(res.c_str());// hope QString takes a C string and no implicit conversion is needed
    Dock->DebuggerPrompt->clear();
}
// -----------------------------------------------------------------------------------------


void lldbBridge::setExecutable(const std::string &exe_file_path) {
    executable = exe_file_path.c_str();
    qDebug() << exe_file_path.c_str();
    // debugger instance must be initialized before -->  void init()
    Target = Debugger.CreateTarget(executable);
    //QSettings settings("Evolution");
    //debugger.executable = settings.value("Evolution/executable_path").toString().toStdString().c_str();
    //qDebug() << debugger.executable;
}

void lldbBridge::showBreakPointsList() {
    DialogBreakPoint_List = new BreakPointListWindow();
    DialogBreakPoint_List->setWindowFlags(Qt::Dialog);
    DialogBreakPoint_List->setMinimumWidth(400);
    for (const auto &i : BreakPointList) {
        DialogBreakPoint_List->insertBreakPoint(i.break_id, i.filepath, i.line);
    }
    // connect tool buttons + some new slots
    connect(DialogBreakPoint_List->BpList, SIGNAL(itemDoubleClicked(QListWidgetItem *, int column)), this, SLOT(slotGoToBreakPointFile(QListWidgetItem *, int column)));
    connect(DialogBreakPoint_List->remove, SIGNAL(clicked()), this, SLOT(slotRemoveBreakPoint()));
    connect(DialogBreakPoint_List->removeAll, SIGNAL(clicked()), this, SLOT(slotRemoveAllBreakPoints()));

    DialogBreakPoint_List->show();
}

void lldbBridge::slotOpenCallStackFile(QListWidgetItem *item) {
    QString filepath = item->text();
    //item->listWidget()->currentItem()->text();
    // TODO: get line associated with stack + might wanna get assembly in function
    emit filePathUpdate(filepath, 1, 1);
}

void lldbBridge::showSetManualBreakPoint(const QString &filepath) {
    file_path = filepath.toLatin1().data();
    manual_BpWindow = new QWidget(Dock);// cannot pass this, -> is only object inherited class
    line_input = new QLineEdit(Dock);   // let ownership to dock
    auto *layout = new QVBoxLayout();
    auto *form = new QFormLayout();
    manual_BpWindow->setWindowFlags(Qt::Dialog);
    line_input->setPlaceholderText("Line");
    form->addRow(line_input);
    layout->addLayout(form);
    connect(line_input, SIGNAL(returnPressed()), this, SLOT(slotSetBreakPointByManualLine()));

    manual_BpWindow->setLayout(layout);
    manual_BpWindow->show();
}

void lldbBridge::slotSetBreakPointByManualLine() {
    int line = line_input->text().toInt();
    manual_BpWindow->close();
    if (line != 0) {
        createBreakpoint(file_path, line);
    }
    manual_BpWindow->deleteLater();
}

void lldbBridge::showTaskManager() {
    auto *task = new TaskWidget();
    task->setFilterVisable(true);
    task->show();
    // debugger.attachToRunningProcess(some id);    // add signal from task and slot from here
}

void lldbBridge::collectThreads() {
    //int num = Process.GetNumThreads();
    //std::cout << num;
    for (int i = 0; i < Process.GetNumThreads(); i++) {
        auto thread = Process.GetThreadAtIndex(i);
        Dock->ThreadBox->insertItem(thread.GetThreadID(), QString::fromStdString(thread.GetName()));
    }
}

void lldbBridge::slotAddWatchPoint() {
    QModelIndex index = Dock->VariablesView->currentIndex();
    if (!index.isValid()) {
        return;
        // Dock->VariablesView->setCurrentIndex();
    }
    int row = index.row();
    const char *var = FrameVariablesList.GetValueAtIndex(row).GetName();

    Target.WatchAddress(FrameVariablesList.GetValueAtIndex(row).GetAddress().GetOffset(),
                        FrameVariablesList.GetValueAtIndex(row).GetByteSize(), true, false, error);


    //Target.GetNumWatchpoints();
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, var);
    QTreeWidgetItem *value_item = new QTreeWidgetItem();
    value_item->setText(0, FrameVariablesList.GetValueAtIndex(row).GetValue());
    item->addChild(value_item);
    WatchDock->VariableTreeValues->addTopLevelItem(item);
    WatchDock->setVisible(true);

    // enable all action if we have some elements
    WatchDock->removeWatch->setEnabled(true);
    WatchDock->modifyWatch->setEnabled(true);
}

void lldbBridge::slotRemoveWatchPoint() {
    QModelIndex index = WatchDock->VariableTreeValues->currentIndex();
    if (!index.isValid()) {
        return;
    }
    delete WatchDock->VariableTreeValues->takeTopLevelItem(index.row());
    // index -> watch_id; only 1 col, hope IDs are changing according to they sum :)
    Target.DeleteWatchpoint(index.row());

    if (WatchDock->VariableTreeValues->topLevelItemCount() != 0) {
        WatchDock->removeWatch->setEnabled(false);
        WatchDock->modifyWatch->setEnabled(false);
    }
}

void lldbBridge::slotRemoveAllWatchPoint() {
    WatchDock->VariableTreeValues->clear();// why this not work ?
    //for (int i = 0; i< WatchDock->VariableTreeValues->topLevelItemCount(); i++) {
    //    delete WatchDock->VariableTreeValues->topLevelItem(i);
    //}
    Target.DeleteAllWatchpoints();

    // disable all buttons, no elements left
    WatchDock->removeWatch->setEnabled(false);
    WatchDock->modifyWatch->setEnabled(false);
}

void lldbBridge::slotModifyWatchPoint() {
    QModelIndex index = WatchDock->VariableTreeValues->currentIndex();
    if (!index.isValid()) {
        return;
    }
    // this is inner item, find out
    WatchDock->VariableTreeValues->topLevelItem(index.row())->child(0)->setFlags(Qt::ItemIsEditable);
}

void lldbBridge::connectDockWidgets() {
    // console;    enter
    connect(Dock->DebuggerPrompt, SIGNAL(returnPressed()), this, SLOT(slotCmdlineExecute()));

    // BP List view
    connect(Dock->BreakPoint_List->BpList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotGoToBreakPointFile(QTreeWidgetItem *, int)));
    connect(Dock->BreakPoint_List->remove, SIGNAL(clicked()), this, SLOT(slotRemoveBreakPoint()));
    connect(Dock->BreakPoint_List->removeAll, SIGNAL(clicked()), this, SLOT(slotRemoveAllBreakPoints()));
    //connect(BreakPoint_List->mute, SIGNAL(clicked()), this, SLOT(slotMuteBreakPoint()));
    connect(Dock->BreakPoint_List->muteAll, SIGNAL(clicked()), this, SLOT(slotDisableAllBreakPoints()));

    connect(Dock->btn_StartDebug, SIGNAL(clicked()), this, SLOT(slotStartDebug()));
    connect(Dock->btn_StopDebug, SIGNAL(clicked()), this, SLOT(slotStopDebug()));
    connect(Dock->btn_RunToCursor, SIGNAL(clicked()), this, SLOT(slotRunToCursor()));

    connect(Dock->btn_StepOver, SIGNAL(clicked()), this, SLOT(slotStepOver()));
    connect(Dock->btn_StepInto, SIGNAL(clicked()), this, SLOT(slotStepInto()));
    connect(Dock->btn_StepOut, SIGNAL(clicked()), this, SLOT(slotStepOut()));
    connect(Dock->btn_StepInstruction, SIGNAL(clicked()), this, SLOT(slotStepInstruction()));
    connect(Dock->btn_Continue, SIGNAL(clicked()), this, SLOT(slotContinue()));


    // call stack, backtrace
    connect(Dock->CallStack, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenCallStackFile(QListWidgetItem *)));

    // VariablesView
    connect(Dock->btn_addWatch, SIGNAL(clicked()), this, SLOT(slotAddWatchPoint()));

    // WatchDock
    // addWatch is in dock title bar
    connect(WatchDock->removeWatch, SIGNAL(clicked()), this, SLOT(slotRemoveWatchPoint()));
    connect(WatchDock->modifyWatch, SIGNAL(clicked()), this, SLOT(slotModifyWatchPoint()));
    connect(WatchDock->removeAll, SIGNAL(clicked()), this, SLOT(slotRemoveAllWatchPoint()));
    connect(WatchDock->enableAll, SIGNAL(clicked()), this, SLOT(slotEnableAllWatchPoints()));
    connect(WatchDock->disableAll, SIGNAL(clicked()), this, SLOT(slotDisableAllWatchPoints()));
}

void lldbBridge::setMessage(const QString &msg) const {
    Dock->debug_output->appendPlainText(msg);
}

void lldbBridge::enableDebuggerButtons() const {
    Dock->btn_StartDebug->setEnabled(false);
    Dock->btn_StopDebug->setEnabled(true);
    Dock->btn_RunToCursor->setEnabled(true);

    Dock->btn_StepOver->setEnabled(true);
    Dock->btn_StepInto->setEnabled(true);
    Dock->btn_StepInstruction->setEnabled(true);
    Dock->btn_Continue->setEnabled(true);
    Dock->btn_StepOut->setEnabled(true);
}

void lldbBridge::disableDebuggerButtons() const {
    Dock->btn_StartDebug->setEnabled(true);
    Dock->btn_StopDebug->setEnabled(false);
    Dock->btn_RunToCursor->setEnabled(false);

    Dock->btn_StepOver->setEnabled(false);
    Dock->btn_StepInto->setEnabled(false);
    Dock->btn_StepInstruction->setEnabled(false);
    Dock->btn_Continue->setEnabled(false);
    Dock->btn_StepOut->setEnabled(false);
}

void lldbBridge::handleBreakPointHit() {
    Dock->debug_output->appendPlainText("BreakPoint was hit");
    setFilePosition(getCurrentFrame());
    collectThreads();
    collectFrameData(getCurrentFrame());
    fillCallStack();
}

void lldbBridge::handleWatchPointHit() {
    Dock->debug_output->appendPlainText("WatchPoint was hit");
    setFilePosition(getCurrentThread().GetSelectedFrame());
    collectThreads();
    collectFrameData(getCurrentFrame());
    fillCallStack();
}

// lldb
// ****************************************************************************************

// have to ensure some things are done, when closed
lldbBridge::~lldbBridge() {
    //lldb::pid_t pid = Process.GetProcessID();
    Process.Kill();
    SBDebugger::Destroy(Debugger);
    SBDebugger::Terminate();
}

void lldbBridge::init() {

    error = SBDebugger::InitializeWithErrorHandling();
    Debugger = SBDebugger::Create();

    if (!Debugger.IsValid()) {
        Dock->debug_output->appendPlainText("Error: failed to create a debugger object\n");
        SBStream str;
        Debugger.GetDescription(str);
        Dock->debug_output->appendPlainText(str.GetData());
        std::cout << str.GetData();
        //return;
    }
    // this is called when executable is set from outside, for now, only testing functionality
    Target = Debugger.CreateTarget(executable);

    listener = Debugger.GetListener();
    //SBLaunchInfo launch_info(nullptr);
    //launch_info.SetExecutableFile(filespec, true);
    //launch_info.SetListener(listener);

    strm.RedirectToFileHandle(stdout, false);


    if(addr_cstr == nullptr) {
        Dock->debug_output->appendPlainText("empty address");
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

void lldbBridge::start() {
    // clear all first
    Dock->debug_output->clear();
    Dock->ThreadBox->clear();
    slotRemoveAllBreakPoints();
    //Dock->BreakPoint_List->remove all ??
    //WatchListView->clear();

    init();

    if (!Target.IsValid()) {
        Dock->debug_output->appendPlainText("Cannot start a debugger process with an invalid target: ");
        std::cout << "Cannot start a debugger process with an invalid target: \n";
        std::cout << executable;
        Dock->debug_output->appendPlainText(executable);
        //return;
    }

    // createBreakpoint("/home/adam/Desktop/sources/Evolution-IDE/main.cpp", 23);
    createBreakpoint("/home/adam/Desktop/sources/Evolution-IDE/main.cpp", 27);

    //Process = Target.LaunchSimple(nullptr, nullptr, nullptr);
    //Process = Target.Launch(launch_info, error);
    Process = Target.Launch(listener, nullptr, nullptr, nullptr, nullptr,
                            nullptr, nullptr, 0, false, error);

    if (!Process.IsValid()) {
        Dock->debug_output->appendPlainText("Process is invalid \n");
        std::cout << "Process is invalid \n";
        Process.GetDescription(strm);
        std::cout << strm.GetData();
        return;
    }

    ProcessingThread = new QThread(this);
    //auto ptr = std::make_shared<DebuggerSession>(this);
    processHandler = new ProcessHandler(/*ptr*/ this);// QObject::moveToThread: Cannot move objects with a parent
    //&lldbBridge::setProcessInterruptFeatures
    connect(ProcessingThread, &QThread::started, processHandler, &ProcessHandler::setProcessInterruptFeatures);
    connect(ProcessingThread, &QThread::finished, ProcessingThread, &QObject::deleteLater);

    connect(processHandler, SIGNAL(addMessage(const QString &)), this, SLOT(setMessage(const QString &)));
    connect(processHandler, SIGNAL(enableButtons()), this, SLOT(enableDebuggerButtons()));
    connect(processHandler, SIGNAL(disableButtons()), this, SLOT(disableDebuggerButtons()));
    connect(processHandler, SIGNAL(breakPointHit()), this, SLOT(handleBreakPointHit()));
    connect(processHandler, SIGNAL(watchPointHit()), this, SLOT(handleWatchPointHit()));
    ProcessingThread->setObjectName("DebuggerThread");
    processHandler->moveToThread(ProcessingThread);
    //qRegisterMetaType<QSharedPointer<lldbBridge>>();
    ProcessingThread->start();
    // when process stopped, return true, pause process
    //setProcessInterruptFeatures();
}

void lldbBridge::stop() {
    // Process.Stop();
    Process.Kill();
}

const char *lldbBridge::getAssembly(SBThread thread) {
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

    return instruction_stream.GetData();
}

void lldbBridge::attachToRunningProcess(const int &proc_id) {
    SBAttachInfo attach_info;
    attach_info.SetProcessID(proc_id);
    if (attach_info.ParentProcessIDIsValid()) {
        Process = Target.Attach(attach_info, error);
    }
    if (error.Success()) {
        Dock->debug_output->appendPlainText("something went wrong when attaching to process");
        //return;
    }
}


void lldbBridge::collectFrameData(SBFrame frame) {
    // clear
    Dock->VariablesView->clear();
    const QString TypeStyle = "style=color:yellow;";
    const QString ValueStyle = "style=color:red;";
    const QString VariableNameStyle = "style=color:green;";

    // !!!! SPENT 4 HOURS WITH ELEMENT <p> (display:inline-block  css property stuffs)---
    // <p> appends \n like in html !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const QString Stag = "<span ";// between them goes style
    const QString Mtag = " >";
    const QString Etag = " </span>";


    QTreeWidgetItem *rootNode = Dock->VariablesView->invisibleRootItem();

    FrameVariablesList = frame.GetVariables(true,// args
                                            true,// locals
                                            true,// statics
                                            true // in scope only
    );

    for (uint32_t i = 0; i < FrameVariablesList.GetSize(); i++) {
        auto value = FrameVariablesList.GetValueAtIndex(i);
        // value.GetDeclaration()
        auto *row = new QTreeWidgetItem();
        // first iteration
        QString desc;
        //  myVal (type) value
        if (!QString::fromLatin1(value.GetName()).isEmpty())
            desc += Stag + VariableNameStyle + Mtag + QString::fromLatin1(value.GetName()) + Etag;
        if (!QString::fromLatin1(value.GetTypeName()).isEmpty())
            desc += "(" + Stag + TypeStyle + Mtag + QString::fromLatin1(value.GetTypeName()) + Etag + ")  ";
        if (QString::fromLatin1(value.GetValue()).isEmpty())
            // TODO: how tot lookup here if NULL or nullptr or some hex values
            // TODO: also how to view war characters, numbers, booleans instead of hex, ...
            desc += Stag + ValueStyle + Mtag + "{}" + Etag;
        else
            desc += Stag + ValueStyle + Mtag + QString::fromLatin1(value.GetValue()) + Etag;

        row->setText(0, desc);

        rootNode->addChild(row);

        if (value.MightHaveChildren()) {
            recursiveValueIterator(value, row);
        }
    }
    Dock->VariablesView->collapseAll();

    // update watched variables values
    // TODO: how to manage containers, strings, etc.
    // TODO: if var goes out of scope row will contain "OUT OF SCOPE"
    compareWatchedValues();
}

void lldbBridge::compareWatchedValues() {
    QList<int> allVariablesIndices;
    for (uint32_t i = 0; i < FrameVariablesList.GetSize(); i++) {
        for (int z = 0; z < WatchDock->VariableTreeValues->topLevelItemCount(); z++) {
            if (FrameVariablesList.GetValueAtIndex(i).GetName() == WatchDock->VariableTreeValues->topLevelItem(z)->text(0).toLatin1().data()) {
                // matched variable names, update variables values
                WatchDock->VariableTreeValues->topLevelItem(z)->child(0)->setText(0, FrameVariablesList.GetValueAtIndex(i).GetValue());
                allVariablesIndices.append(z);
            } else {
                // no match ... out of scope
            }
        }
    }
    // deal with out of scope variables, not exists anymore ...
    if (allVariablesIndices.size() != WatchDock->VariableTreeValues->topLevelItemCount()) {
        for (int i = 0; i < WatchDock->VariableTreeValues->topLevelItemCount(); i++) {
            if (!allVariablesIndices.contains(i)) {
                WatchDock->VariableTreeValues->topLevelItem(i)->child(0)->setText(0, "OUT OF SCOPE");
            }
        }
    }
}

void lldbBridge::recursiveValueIterator(SBValue value, QTreeWidgetItem *parent_row) {
    const QString TypeStyle = "style=color:yellow;";
    const QString ValueStyle = "style=color:red;";
    const QString VariableNameStyle = "style=color:green;";

    const QString Stag = "<span ";// between them goes style
    const QString Mtag = " >";
    const QString Etag = " </span>";

    for (uint32_t i = 0; i < value.GetNumChildren(); i++) {
        auto *child_row = new QTreeWidgetItem();

        QString desc;
        //  myVal (type) value
        if (!QString::fromLatin1(value.GetChildAtIndex(i).GetName()).isEmpty()) {
            desc += Stag + VariableNameStyle + Mtag + QString::fromLatin1(value.GetChildAtIndex(i).GetName()) + Etag;
        }
        if (!QString::fromLatin1(value.GetChildAtIndex(i).GetTypeName()).isEmpty()) {
            desc += "(" + Stag + TypeStyle + Mtag + QString::fromLatin1(value.GetChildAtIndex(i).GetTypeName()) + Etag + ") ";
        }
        if (QString::fromLatin1(value.GetChildAtIndex(i).GetValue()).isEmpty()) {
            desc += Stag + ValueStyle + Mtag + "{}" + Etag;
        } else {
            desc += Stag + ValueStyle + Mtag + QString::fromLatin1(value.GetChildAtIndex(i).GetValue()) + Etag;
        }

        child_row->setText(0, desc);
        parent_row->addChild(child_row);
        qDebug() << desc;
        qDebug() << "\n";
        /*
        if (value.MightHaveChildren()) {
            // recursion proceed here
            recursiveValueIterator(value.GetChildAtIndex(i), child_row);
        }
        */
    }
}

std::string lldbBridge::frameGetLocation(const SBFrame &frame) {
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

    // set source, where are we in debugging process + highlight line
    //source_view->setCursorAtLine(line);

    return description;
}

void lldbBridge::setFilePosition(const SBFrame &frame) {
    const char *filename = frame.GetLineEntry().GetFileSpec().GetFilename();
    SBFunction function = frame.GetFunction();
    uint32_t line = frame.GetLineEntry().GetLine();
    //const char* func_name = function.GetDisplayName();

    std::cout << filename;
    std::cout << "\n";
    std::cout << function.GetName();
    std::cout << "\n";
    std::cout << line;

    // TODO: if file is not contained, get assembly code displayed, jump over read, but remain selection
    // TODO: tab icon .... isn't it better to just emit signal ??

    // frame contains only file names : main.cpp, file.h, ... so i need to find path
    QString filepath;
    for (const auto &path : sources) {
        // we found the exact file path
        if (QFileInfo(QString::fromLatin1(filename)).fileName() == filename) {
            filepath = path;
            break;
        }
    }
    // assembly
    WorkingContent = "some assembly";

    emit filePathUpdate(filepath, line, 1);
}

void lldbBridge::createBreakpoint(const char *filepath, const int &line) {

    SBBreakpoint breakpoint = Target.BreakpointCreateByLocation(filepath, line);
    // make sure that it's good
    if (!breakpoint.IsValid()) {
        Dock->debug_output->appendPlainText("Breakpoint is not valid:  file: ");
        Dock->debug_output->appendPlainText(filepath);
        //debug_output->appendPlainText(line);
        return;
    }

    breakpoint.SetEnabled(true);

    BreakPointData data{breakpoint.GetID(), filepath, line};
    BreakPointList.push_back(data);

    Dock->BreakPoint_List->insertBreakPoint(breakpoint.GetID(), filepath, line);
    // also take case of this instance to ensure both of them gets created, removed
    /*
    if(DialogBreakPoint_List != nullptr){
        DialogBreakPoint_List->insertBreakPoint(breakpoint.GetID(), file_name, line);
    }
    */
}

void lldbBridge::removeBreakpoint(const break_id_t &id) {
    int pos;
    for (int i = 0; i < BreakPointList.size(); i++) {
        if (BreakPointList[i].break_id == id) {
            // remove from help list
            pos = i;
        }
    }
    if (!Target.BreakpointDelete(id)) {
        Dock->debug_output->appendPlainText("Breakpoint was not removed !");
    }
    BreakPointList.erase(BreakPointList.begin() + pos);
    // clear whole list and insert remaining break points
    Dock->BreakPoint_List->BpList->clear();
    for (const auto &b_point : BreakPointList) {
        Dock->BreakPoint_List->insertBreakPoint(b_point.break_id, b_point.filepath, b_point.line);
    }
    /*
    if(DialogBreakPoint_List != nullptr){
        DialogBreakPoint_List->BpList->clear();
        for (const auto& b_point : BreakPointList) {
            DialogBreakPoint_List->insertBreakPoint(b_point.break_id, b_point.filename, b_point.line);
        }
    }
    */
}

void lldbBridge::removeBreakpoint(const char *filepath, const int &line) {
    // deletion by ID --> find it out and then delete
    break_id_t ID;
    // after we find that exist and was removed
    int pos;
    for (int i = 0; i < BreakPointList.size(); i++) {
        if (BreakPointList[i].filepath == filepath && BreakPointList[i].line == line) {
            // store ID for deletion
            ID = BreakPointList[i].break_id;
            // remove from help list
            pos = i;
        }
    }
    if (!Target.BreakpointDelete(ID)) {
        Dock->debug_output->appendPlainText("Breakpoint was not removed !");
        return;
    }
    BreakPointList.erase(BreakPointList.begin() + pos);
    // clear whole list and insert remaining break points
    Dock->BreakPoint_List->BpList->clear();
    for (const auto &b_point : BreakPointList) {
        Dock->BreakPoint_List->insertBreakPoint(b_point.break_id, b_point.filepath, b_point.line);
    }
    /*
    if(DialogBreakPoint_List != nullptr){
        DialogBreakPoint_List->BpList->clear();
        for (const auto& b_point : BreakPointList) {
            DialogBreakPoint_List->insertBreakPoint(b_point.break_id, b_point.filename, b_point.line);
        }
    }
    */
}

SBThread lldbBridge::getCurrentThread() {
    // no needed
    /*
    if (!Process.IsValid()) {
        std::cout << "process is not valid";
        Dock->debug_output->appendPlainText("process is not valid");
        exit(1);
    }
    */
    return Process.GetSelectedThread();
}

SBFrame lldbBridge::getCurrentFrame() {
    return getCurrentThread().GetSelectedFrame();
}

SBValue lldbBridge::findSymbol(const char *name) {
    return Target.FindFirstGlobalVariable(name);
    // return Target.FindFunctions(name);  .....
}

void lldbBridge::pause() {
    if (Process.IsValid()) {
        auto err = Process.Stop();
        if (err.Fail()) {
            Dock->debug_output->appendPlainText("Failed to pause process");// {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
}

void lldbBridge::Continue() {
    if (Process.IsValid()) {
        auto err = Process.Continue();
        if (err.Fail()) {
            Dock->debug_output->appendPlainText("Process Ended");
            //setProcessInterruptFeatures();
            disableDebuggerButtons();
        } else {
            SBStream str;
            err.GetDescription(str);
            std::cout << "Failed to continue process" + std::string(str.GetData());
            Dock->debug_output->appendPlainText("Failed to continue process" + QString(str.GetData()));
            // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
            disableDebuggerButtons();
        }
    }
}

void lldbBridge::stepOver() {
    getCurrentThread().StepOver();
    setFilePosition(getCurrentFrame());
}

void lldbBridge::stepInto() {
    getCurrentThread().StepInto();
    setFilePosition(getCurrentFrame());
}

void lldbBridge::stepOut() {
    getCurrentThread().StepOut();
    setFilePosition(getCurrentFrame());
}

void lldbBridge::stepInstruction() {
    getCurrentThread().StepInstruction(true);// step_over     ; false -> into call instr.
    setFilePosition(getCurrentFrame());
}

std::string lldbBridge::executeDebuggerCommand(const std::string &args) {
    // tutorial site:
    // https://lldb.llvm.org/use/tutorial.html

    SBCommandReturnObject result;
    Debugger.GetCommandInterpreter().HandleCommand(args.c_str(), result);
    std::string output;
    const char *cstr;
    cstr = result.GetOutput();
    if (cstr && cstr[0])
        output += cstr;
    cstr = result.GetOutput();
    if (cstr && cstr[0])
        output += cstr;
    return output;
}

