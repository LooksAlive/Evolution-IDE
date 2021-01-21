#ifndef EVOLUTION_IDE_DEPENDENCYINSTALLER_H
#define EVOLUTION_IDE_DEPENDENCYINSTALLER_H

#include <ostream>
#include "commandlineexecutor.h"

class DependencyInstaller {

public:
    DependencyInstaller();
    ~DependencyInstaller() = default;

    enum Package{
        clang = 1,
        gcc,
        llvm,
        gdbgui, // does not have any linux build package to install by terminal, need to download(bin or github)
        gdb,
        lldb,
        Qt5,
        python3,

    };

    // for now only on linux
    void install(const Package &package);


};


#endif //EVOLUTION_IDE_DEPENDENCYINSTALLER_H
