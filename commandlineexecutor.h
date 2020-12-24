#ifndef COMMANDLINEEXECUTOR_H
#define COMMANDLINEEXECUTOR_H

#include <iostream>
#include <string>

class CommandLineExecutor
{
public:
    CommandLineExecutor();
    bool GenerateCmake = false;

    enum BuildMode {
        Debug = 1,
        Release = 2,
    };

    void setCompiler(const std::string &compiler, const std::string &version, const BuildMode &mode, const std::string &addflags);
    std::string Compile();

private:

    void DetermineCompilerVersion();

    std::string version = "";
    std::string args = "";
    std::string flags = "";

    std::string cmake_args = "mkdir build && cd build && cmake ..";
};





#endif // COMMANDLINEEXECUTOR_H
