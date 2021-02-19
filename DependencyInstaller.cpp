#include "DependencyInstaller.h"

DependencyInstaller::DependencyInstaller(){

}

void DependencyInstaller::install(const Package &package) {

    CommandLineExecutor executor;

    switch (package) {
        case clang:
            executor.ExecuteSimpleCommand("sudo apt-get install clang");
            break;
        case gcc:
            executor.ExecuteSimpleCommand("sudo apt-get install gcc");
            break;
        case llvm:
            executor.ExecuteSimpleCommand("sudo apt-get install llvm");
            break;
        case gdbgui:
            break;
        case gdb:
            executor.ExecuteSimpleCommand("sudo apt-get install gdb");
            break;
        case lldb:
            executor.ExecuteSimpleCommand("sudo apt-get install lldb");
            break;
        case Qt5:
            executor.ExecuteSimpleCommand("sudo apt-get install qt5-default");
            break;
        case python3:
            executor.ExecuteSimpleCommand("sudo apt-get install python3");
            break;

        default:
            break;
    }
}
