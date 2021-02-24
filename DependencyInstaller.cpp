#include "DependencyInstaller.h"

DependencyInstaller::DependencyInstaller(){

}

void DependencyInstaller::install(const Package &package) {
    switch (package) {
        case clang:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install clang");
            break;
        case gcc:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install gcc");
            break;
        case llvm:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install llvm");
            break;
        case gdbgui:
            break;
        case gdb:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install gdb");
            break;
        case lldb:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install lldb");
            break;
        case Qt5:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install qt5-default");
            break;
        case python3:
            CommandLineExecutor::ExecuteSimpleCommand("sudo apt-get install python3");
            break;

        default:
            break;
    }
}
