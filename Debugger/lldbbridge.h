#ifndef LLDBBRIDGE_H
#define LLDBBRIDGE_H

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
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

    void setReport(const char *error);   // SBError handling function
    std::string report; // access outside TODO: how to track if error occurred or not ?

    std::string pause();    // when breakpoint is hit
    std::string Continue();

    std::string setBreakpoint();
    std::string removeBreakpoint();

    void setFrame(SBFrame frame);
    SBThread getCurrentThread();
    SBFrame getCurrentFrame();
    std::string frameDescribeLocation();
    SBValue findSymbol(const char *name);



    void stepOver();
    void stepInto();
    void stepOut();
    void stepInstruction();

    std::string executeDebuggerCommand(const std::string &args);


    struct framedata{
        std::string type;
        std::string name;
        std::vector<std::string> values;
    };
    // get num of frames in project -> return some data struct vector, this would need to be updated or collected at
    // once
    std::vector<framedata> get_var_func_info();         // get names, values and insert into variable widget

    framedata get_var_func_info_update();         // step by step updating


    bool isRunning();
    void recordError();

    void start();   // process initialization
    void stop();    // destructor called here


    // The first argument is the file path we want to look something up in
    const char *executable = "/home/adam/Desktop/SKUSKA/Parser";
    const char *addr_cstr = "#address_to_lookup";
    const bool add_dependent_libs = false;
    const char *arch = nullptr;
    const char *platform = nullptr;


private:
    void init();

    SBError error;
    SBStream strm;


    SBDebugger Debugger;
    SBTarget   Target;
    SBProcess  Process;


    struct breaks{
        int line;
        char compile_unit;
    };
    //std::unordered_map<breaks, lldb::SBBreakpoint> Breakpoints;

};

#endif // LLDBBRIDGE_H
