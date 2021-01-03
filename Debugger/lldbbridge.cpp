#include "lldbbridge.h"

lldbBridge::lldbBridge()
{
    SBDebugger::Initialize();
    Debugger = SBDebugger::Create();
    Target = Debugger.CreateTarget("filepath");
}

lldbBridge::~lldbBridge() {
    kill_process();
    Debugger.Terminate();
}

void lldbBridge::init() {
    // Create a debugger instance so we can create a target
    if (!Debugger.IsValid()){
        std::cout << "error: failed to create a debugger object\n" << std::endl;
    }


    strm.RedirectToFileHandle(stdout, false);


    if(addr_cstr == nullptr) {
        std::cout <<"empty address";
        exit(1);
    }
// The second argument in the address that we want to lookup
//lldb::addr_t file_addr = strtoull(addr_cstr, nullptr, 0);
// Create a target using the executable.
//SBTarget target = debugger.CreateTarget(exe_file_path, arch, platform,
//                                       add_dependent_libs, error);

    if (!error.Success()) {
        std::cout <<"error: " << error.GetDescription(strm) << std::endl;
        exit(1);
    }

    SBModuleSpec module_spec;
    module_spec.SetFileSpec(SBFileSpec(exe_file_path));
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


std::string lldbBridge::kill_process() {
    std::string res;
    if (Process.IsValid()) {
        auto err = Process.Kill();
        if (err.Fail()) {
            res = "Failed to terminate process"; // "Error Code " + err.GetError() + " Error String " +  err.GetCString();
            }
    }
    return res;
}

void lldbBridge::setFrame(lldb::SBFrame frame) {
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

std::string lldbBridge::pause() {
    std::string res;

    if (Process.IsValid()) {
        auto err = Process.Stop();
        if (err.Fail()) {
            res = "Failed to pause process"; // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
    return res;
}

std::string lldbBridge::Continue() {
    std::string res;

    if (Process.IsValid()) {
        auto err = Process.Continue();
        if (err.Fail()) {
            res = "Failed to pause process"; // {{"Error Code", err.GetError()}, {"Error String", err.GetCString()}};
        }
    }
    return res;
}

std::string lldbBridge::setBreakpoint() {

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
    return std::string();
}

std::string lldbBridge::removeBreakpoint() {

    /*
    auto iter = mBreakpoints.find();
    if (iter == mBreakpoints.end()) { return false; }

    return Target.BreakpointDelete(iter->second.GetID());
    */
    return std::string();
}

void lldbBridge::start() {
    std::string res;

    if (!Target.IsValid()) {
        res = "Cannot start a debugger process with an invalid target";
    }

    Process = Target.LaunchSimple(nullptr, nullptr, exe_file_path);


}
