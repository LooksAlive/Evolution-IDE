#ifndef LLDBBRIDGE_H
#define LLDBBRIDGE_H

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "lldb/API/LLDB.h"

using namespace lldb;

class lldbBridge
{
public:
    lldbBridge();
    ~lldbBridge();

    // no copy or move, doesn't really make sense
    lldbBridge(const lldbBridge& other) = delete;
    lldbBridge(lldbBridge&& other)      = delete;
    lldbBridge& operator=(const lldbBridge&) = delete;
    lldbBridge& operator=(lldbBridge&&) = delete;

    std::string kill_process();

    std::string pause();
    std::string Continue();

    std::string setBreakpoint();
    std::string removeBreakpoint();

    void setFrame(lldb::SBFrame frame);

    void start();


private:
    void init();

    // The first argument is the file path we want to look something up in
    const char *exe_file_path = "/home/adam/Desktop/SKUSKA/Parser";
    const char *addr_cstr = "#address_to_lookup";
    const bool add_dependent_libs = false;
    const char *arch = nullptr;
    const char *platform = nullptr;
    SBError error;
    SBStream strm;


    SBDebugger Debugger;
    SBTarget   Target;
    SBProcess  Process;

    //std::unordered_map<const NodeInstance*, lldb::SBBreakpoint> mBreakpoints;

};

#endif // LLDBBRIDGE_H
