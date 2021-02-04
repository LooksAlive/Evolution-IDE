#ifndef LLDBBRIDGE_H
#define LLDBBRIDGE_H

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

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

    // SBError handling function
    void setReport(const char *msg);
    // when program is running, this set if some unexpected occurred (segmentation fault, bad pointers, ...)
    void recordRunningError(const char *msg);
    std::string report;

    void pause();    // when breakpoint is hit
    void Continue();

    void setBreakpoint(const char *file_name, const int &line);
    void removeBreakpoint(const char *file_name, const int &line);
    void removeBreakpoint(const break_id_t &id);

    void storeFrameData(SBFrame frame);
    SBThread getCurrentThread();
    SBFrame getCurrentFrame();
    std::string frameDescribeLocation(SBFrame &frame);
    SBValue findSymbol(const char *name);

    void stepOver();
    void stepInto();
    void stepOut();
    void stepInstruction();

    std::string executeDebuggerCommand(const std::string &args);

    // data storages
    struct framedata{
        std::string type;
        std::string name;
        std::string value;
        std::string description;
    };
    // get num of frames in project -> return some data struct vector, this would need to be updated or collected at
    // once
    std::vector<framedata> get_var_func_info();         // get names, values and insert into variable widget
    framedata get_var_func_info_update();         // step by step updating

    struct BreakPointData{
        break_id_t break_id;
        const char *filename;
        int line;
    };
    std::vector<BreakPointData> BreakPointList;


    //void setObject(DebuggerWidget *widget);
    void start();   // process initialization
    void stop();    // destructor called here

    void attachToRunningProcess(const int &proc_id);


    // The first argument is the file path we want to look something up in
    const char *executable = "/home/adam/Desktop/sources/Evolution-IDE/cmake-build-debug/editor";   // "/home/adam/Desktop/SKK/cmake-build/executable"
    const char *addr_cstr = "#address_to_lookup";
    const bool add_dependent_libs = false;
    const char *arch = nullptr;
    const char *platform = nullptr;

    bool isRunning = false;

private:
    void init();
    // main function
    // is breakpoint hit, waiting for such events
    // called when debug process starts
    void setProcessInterruptFeatures();
    bool HandleProcessEvent(SBEvent &event);
    bool HandleProcessStateChangeEvent(SBEvent &event);
    void HandleProcessStopped(SBEvent &event, SBProcess &process);

    const char *getAssembly(SBThread &thread);

    //DebuggerWidget *widgets;

    SBError error;
    SBStream strm;
    SBFileSpec filespec;


    SBDebugger Debugger;
    SBTarget   Target;
    SBProcess  Process;
    SBListener listener;
};

#endif // LLDBBRIDGE_H
