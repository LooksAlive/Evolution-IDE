#ifndef COMMANDLINEEXECUTOR_H
#define COMMANDLINEEXECUTOR_H

#include <iostream>
#include <string>
#include <vector>

class CommandLineExecutor
{
public:
    CommandLineExecutor();
    ~CommandLineExecutor() = default;
    bool GenerateCmake = false;

    enum BuildMode {
        Debug = 1,
        Release = 2,
    };
    
    std::string ExecuteCommand(std::string cmd);

    void setCompiler(const std::string &compiler, const BuildMode &mode, const std::string &addflags);
    void setExecutableName(const std::string &name, const std::string &path);
    void setSourceFiles(const std::vector<std::string> &sources);
    void setLibraryPaths(const std::vector<std::string> &library_paths);
    std::string Build();
    std::string Execute();

    std::string ClangFormat(const std::vector<std::string> &sources);
    std::string ClangTidy(const std::vector<std::string> &sources);
    std::string ClangCheck(const std::vector<std::string> &sources);

    std::string Valgrind();

    void OpenGdbGui();

private:

    void DetermineCompilerVersion(const std::string &tool);

    std::string compile_args = "/usr/bin/";
    std::string version = "";
    std::string flags = "";

    std::string executable_name = "a.out";   //    /home/adam/Desktop/SKUSKA/
    std::string executable_path = "";        //    executable.elf

    std::string cmake_args = "mkdir build && cd build && cmake ..";
};





#endif // COMMANDLINEEXECUTOR_H
