#include "lldbbridge.h"

lldbBridge::lldbBridge(){}

lldbBridge::~lldbBridge() {
    lldb::pid_t pid = Process.GetProcessID();
    setReport("Killing process ");
    Process.Kill();
    SBDebugger::Terminate();
}

void lldbBridge::init() {

    error = SBDebugger::InitializeWithErrorHandling();
    Debugger = SBDebugger::Create();
    // Create a debugger instance so we can create a target
    if (!Debugger.IsValid()){
        setReport("Error: failed to create a debugger object\n");
        SBStream str;
        Debugger.GetDescription(str);
        setReport(str.GetData());
        std::cout << str.GetData();
        return;
    }
    // this should be set on the begining
    Target = Debugger.CreateTarget(executable);

    strm.RedirectToFileHandle(stdout, false);


    if(addr_cstr == nullptr) {
        setReport("empty address");
        return;
    }
// The second argument in the address that we want to lookup
//lldb::addr_t file_addr = strtoull(addr_cstr, nullptr, 0);
// Create a target using the executable.
//SBTarget target = debugger.CreateTarget(executable, arch, platform,
//                                       add_dependent_libs, error);

    if (!error.Success()) {
        std::cout <<"error: " << error.GetDescription(strm) << std::endl;
        return;
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
    init();

    if (!Target.IsValid()) {
        setReport("Cannot start a debugger process with an invalid target: ");
        std::cout << "Cannot start a debugger process with an invalid target: \n";
        std::cout << executable;
        setReport(executable);
        return;
    }

    setBreakpoint("main.cpp", 5);
    setBreakpoint("main.cpp", 6);
    setBreakpoint("main.cpp", 7);

    listener = Debugger.GetListener();
    SBLaunchInfo launch_info(nullptr);
    launch_info.SetExecutableFile(filespec, true);
    launch_info.SetListener(listener);
    //Process = Target.LaunchSimple(nullptr, nullptr, nullptr);
    Process = Target.Launch(launch_info, error);

    if(!Process.IsValid()){
        setReport("Process is invalid \n");
        std::cout << "Process is invalid \n";
        SBStream str;
        Process.GetDescription(str);
        std::cout << str.GetData();
        //return;
    }


    std::thread debug_thread(&lldbBridge::setProcessInterruptFeatures, this); // , "Debug_session"
    debug_thread.detach(); // join
    // do not join, since i do not want to want for thread to end
}

void lldbBridge::stop() {
    Process.Stop();
}

void lldbBridge::setProcessInterruptFeatures() {
    const uint32_t event_timeout_secs = 10; // Wait for 10 seconds for an event. You can set this to be longer if you want like UINT32_MAX to wait forever.
    bool done = false;
    std::cout << "Im finally in !!!! \n";
    while (!done) {
        SBEvent event;
        // frameDescribeLocation();


        if (listener.WaitForEvent(event_timeout_secs, event)) {
            if (SBProcess::EventIsProcessEvent(event)) {
                // Handle process event
                done = HandleProcessEvent(event);
            }
            else if (SBTarget::EventIsTargetEvent(event)) {
                // Handle target event
                //done = HandleTargetEvent(event);
            }
        }
    }
    //uint32_t lldb::SBEvent::GetType() const;
}

bool lldbBridge::HandleProcessEvent(SBEvent &event) {
    switch (event.GetType()) {
        case lldb::SBProcess::eBroadcastBitStateChanged:
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

bool lldbBridge::HandleProcessStateChangeEvent(SBEvent &event) {
    SBProcess process = SBProcess::GetProcessFromEvent(event);
    StateType state = SBProcess::GetStateFromEvent(event);

    switch (state) {
        case eStateAttaching: ///< Process is currently trying to attach
            // Maybe you put up a progress dialog in case attach takes a while?
            std::cout << "attaching to process";
            break;
        case eStateLaunching: ///< Process is in the process of launching
            // Maybe you put up a progress dialog in case launch takes a while?
            std::cout << "launching to process";
            break;
        case eStateStopped:   ///< Process is stopped and can be examined
            HandleProcessStopped(event, process);
            break;
        case eStateRunning:   ///< Process is now running and can't be examined
            // Update your UI maybe and disable the play and step buttons so the user
            // can't try to run the program while it is already running
            isRunning = true;
            std::cout << "running";
            break;
        case eStateDetached:  ///< Process has been detached and can't be examined.
            // Update your GUI top indicate you are no longer debugging since LLDB
            // has detached from your process.
            std::cout << "process has been detached";

            // If you return true, then this will cause the event loop the exit.
            // This will work well if you only ever debug one process at a time.
            // If you are debugging multiple, you can return false.
            return true;
        case eStateExited:    ///< Process has exited and can't be examined.
            // Update your GUI top indicate you are no longer debugging since your
            // process has run to completion and has exited
            isRunning = false;
            std::cout << "Process exited normally";

            // If you return true, then this will cause the event loop the exit.
            // This will work well if you only ever debug one process at a time.
            // If you are debugging multiple, you can return false.
            return true;
        default:
            break;
    }
    return false; // Not done, don't exit main loop
}

void lldbBridge::HandleProcessStopped(SBEvent &event, SBProcess &process) {
    if (SBProcess::GetRestartedFromEvent(event)) {
        // Process is automatically restarted due to script or breakpoint action.
        // Don't update the GUI because we will soon receive a eStateRunning state...
        return;
    }
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

const char * lldbBridge::getAssembly(SBThread &thread) {
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

void lldbBridge::attachToRunningProcess(const int &proc_id){
     SBAttachInfo attach_info;
    attach_info.SetProcessID(proc_id);
    if(attach_info.ParentProcessIDIsValid()){
        Process = Target.Attach(attach_info, error);
    }
    if(error.Success()){
        setReport("something went wrong shen attaching to process");
        //return;
    }
}


void lldbBridge::storeFrameData(SBFrame frame) {
    //clear();

    auto FrameList = frame.GetVariables(true,  // args
                                   true,  // locals
                                   true,  // statics
                                   true   // in scope only
    );

    for (uint32_t idx = 0; idx < FrameList.GetSize(); ++idx) {
        auto value = FrameList.GetValueAtIndex(idx);
        framedata data;
        data.name = value.GetDisplayTypeName();
        data.type = value.GetType().GetName();
        data.value = value.GetValue();
        data.description = value.GetObjectDescription();
        //value.GetSummary();
        //value.GetThread();
    }
}

void lldbBridge::setBreakpoint(const char *file_name, const int &line) {
    // create the breakpoint
    filespec.SetFilename(file_name);
    if(!filespec.IsValid()){
        setReport("invalid filename, breakpoint not set");
        return;
    }

    SBBreakpoint breakpoint = Target.BreakpointCreateByLocation(filespec, line);

     	// make sure that it's good
	if (!breakpoint.IsValid()) {
	    setReport("Breakpoint is not valid:  filename: ");
	    setReport(file_name);
        //setReport(line);
        return;
	}

    breakpoint.SetEnabled(true);


    BreakPointData data{breakpoint.GetID(), file_name, line};
    BreakPointList.push_back(data);
}

void lldbBridge::removeBreakpoint(const break_id_t &id) {
    if(!Target.BreakpointDelete(id)){
        setReport("Breakpoint was not deleted !");
        return;
    }
}

void lldbBridge::removeBreakpoint(const char *file_name, const int &line) {

    break_id_t ID;
    for (int i = 0; i < BreakPointList.size(); i++) {
        if(BreakPointList[i].filename == file_name && BreakPointList[i].line == line){
            ID = BreakPointList[i].break_id;
        }
    }
    if(!Target.BreakpointDelete(ID)){
        setReport("Breakpoint was not deleted !");
        return;
    }

}

std::vector<lldbBridge::framedata> get_var_func_info() {
    return std::vector<lldbBridge::framedata>();
}

lldbBridge::framedata get_var_func_info_update() {
    return lldbBridge::framedata();
}

SBThread lldbBridge::getCurrentThread() {
    return Process.GetSelectedThread();
}

SBFrame lldbBridge::getCurrentFrame() {
    return getCurrentThread().GetSelectedFrame();
}

std::string lldbBridge::frameDescribeLocation(SBFrame &frame) {
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

SBValue lldbBridge::findSymbol(const char *name){
    return Target.FindFirstGlobalVariable(name);
    // return Target.FindFunctions(name);  .....
}

void lldbBridge::pause() {

    if (Process.IsValid()) {
        auto err = Process.Stop();
        if (err.Fail()) {
            setReport("Failed to pause process"); // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
}

void lldbBridge::Continue() {

    if (Process.IsValid()) {
        auto err = Process.Continue();
        if (err.Fail()) {
            setReport("Failed to pause process"); // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
}

void lldbBridge::stepOver() {
    getCurrentThread().StepOver();
    // plus later use other definition maybe, providing SBError
}

void lldbBridge::stepInto() {
    getCurrentThread().StepInto();
}

void lldbBridge::stepOut() {
    getCurrentThread().StepOut();
}

void lldbBridge::stepInstruction() {
    getCurrentThread().StepInstruction(true); // step_over     ; false -> into call instr.
}

std::string lldbBridge::executeDebuggerCommand(const std::string &args) {
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

void lldbBridge::setReport(const char *msg) {
    report += msg + std::string("\n");
}

void lldbBridge::recordRunningError(const char *msg) {
    report += msg + std::string("\n");
}
/*
void lldbBridge::setObject(DebuggerWidget *widget) {
    widgets = widget;
}
*/







/*
const size_t num_symbols = module.GetNumSymbols();
std::cout <<"num " << num_symbols << std::endl;
SBSymbol symbol;
for (size_t symbol_idx=0; symbol_idx<num_symbols; ++symbol_idx) {
    symbol = module.GetSymbolAtIndex(symbol_idx);
    std::cout <<"section " << symbol_idx << ": " << symbol.GetStartAddress().GetSection().GetName() << std::endl;
    std::cout <<"section " << symbol_idx << ": " << symbol.GetStartAddress().GetFileAddress() << std::endl;
}
*/

//SBSymbol symbol = module.FindSymbol("main");

//std::cout <<"section: " << symbol.GetStartAddress().GetFileAddress() << std::endl;
//std::cout <<"section: " << symbol.GetDescription(strm) << std::endl;

/*
const size_t num_cus = module.GetNumCompileUnits();
std::cout <<"num: " << num_cus << std::endl;
for (size_t cu_idx=0; cu_idx<num_cus; ++cu_idx) {
    SBCompileUnit cu = module.GetCompileUnitAtIndex(cu_idx);
    std::cout <<"cu: " << cu.GetDescription(strm) << std::endl;
    std::cout <<"file: " << cu.GetFileSpec().GetFilename() << std::endl;
    std::cout <<"line: " << cu.GetNumLineEntries() << std::endl;
}
*/