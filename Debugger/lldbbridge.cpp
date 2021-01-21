#include "lldbbridge.h"

lldbBridge::lldbBridge(){}

lldbBridge::~lldbBridge() {
    lldb::pid_t pid = Process.GetProcessID();
    std::cout << std::string("Killing process ") + char(pid) << std::endl;  // not added pid, bad conversion
    setReport("Killing process ");
    Process.Kill();
    Debugger.Terminate();
}

void lldbBridge::init() {

    SBDebugger::Initialize();
    Debugger = SBDebugger::Create();
    Target = Debugger.CreateTarget(executable);

    // Create a debugger instance so we can create a target
    if (!Debugger.IsValid()){
        setReport("error: failed to create a debugger object\n");
    }

    strm.RedirectToFileHandle(stdout, false);


    if(addr_cstr == nullptr) {
        setReport("empty address");
        exit(1);
    }
// The second argument in the address that we want to lookup
//lldb::addr_t file_addr = strtoull(addr_cstr, nullptr, 0);
// Create a target using the executable.
//SBTarget target = debugger.CreateTarget(executable, arch, platform,
//                                       add_dependent_libs, error);

    if (!error.Success()) {
        std::cout <<"error: " << error.GetDescription(strm) << std::endl;
        exit(1);
    }

    SBModuleSpec module_spec;
    module_spec.SetFileSpec(SBFileSpec(executable));
    if(!module_spec.IsValid()){
        std::cout <<"Loaded not valid executable, Exiting " << std::endl;
        exit(1);
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

void lldbBridge::setFrame(SBFrame frame) {
    //clear();

    auto vars = frame.GetVariables(true,  // args
                                   true,  // locals
                                   true,  // statics
                                   true   // in scope only
    );

    for (uint32_t idx = 0; idx < vars.GetSize(); ++idx) {
        auto val = vars.GetValueAtIndex(idx);
        /*
        auto item = new QTreeWidgetItem(QStringList() << val.GetName() << val.GetDisplayTypeName()
                                                      << val.GetSummary() << val.GetValue()
                                                      << val.GetObjectDescription());

        insertTopLevelItem(0, item);
         */
    }
}

void lldbBridge::setBreakpoint() {

    std::string res;

    // create the breakpoint
    /*
    auto breakpoint = Target.BreakpointCreateByLocation(
            node.module().sourceFilePath().string().c_str(), linenum);

     	// make sure that it's good
	if (!breakpoint.IsValid()) {
		res.addEntry("EUKN", "Could not set breakpoint on node",
		             {{"nodeid", node.stringId()},
		              {"File Name", node.module().sourceFilePath().string()},
		              {"Line  Number", linenum}});

		return res;
	}

    breakpoint.SetEnabled(true);

    */
}

void lldbBridge::removeBreakpoint() {

    /*
    auto iter = mBreakpoints.find();
    if (iter == mBreakpoints.end()) { return false; }

    return Target.BreakpointDelete(iter->second.GetID());
    */
}

void lldbBridge::start() {
    init();

    if (!Target.IsValid()) {
        setReport("Cannot start a debugger process with an invalid target");
    }

    Process = Target.LaunchSimple(nullptr, nullptr, executable);
}

void lldbBridge::stop() {
    lldbBridge::~lldbBridge();
}


bool lldbBridge::isRunning() {
    if(Debugger.IsValid() && Process.IsValid()){
        return true;
    }
    else{
        return false;
    }
}

void lldbBridge::recordError() {
    // when process gen some stop signal --> get generated error, or just implement SBError
}


std::vector<lldbBridge::framedata> lldbBridge::get_var_func_info() {
    return std::vector<lldbBridge::framedata>();
}

lldbBridge::framedata lldbBridge::get_var_func_info_update() {
    return lldbBridge::framedata();
}

SBThread lldbBridge::getCurrentThread() {
    if(Process){
        return Process.GetSelectedThread();
    }
}

SBFrame lldbBridge::getCurrentFrame() {
    SBThread thread = getCurrentThread();
    if(thread){
        return thread.GetSelectedFrame();
    }
}

std::string lldbBridge::frameDescribeLocation() {
    SBFrame frame;
    const char *filename = frame.GetLineEntry().GetFileSpec().GetFilename();
    SBFunction function = frame.GetFunction();
    uint32_t line = frame.GetLineEntry().GetLine();
    std::string description = "location: ";
    description += std::string("file= ") + filename + " ";
    description += std::string("line= ") + char(line) + " ";
    description += std::string("address= ") + char(frame.GetPCAddress().GetOffset()) + " ";
    description += std::string("function= ") + function.GetDisplayName() + " ";

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
    SBCommandReturnObject result = SBCommandReturnObject();
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