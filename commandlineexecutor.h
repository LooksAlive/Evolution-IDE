#ifndef COMMANDLINEEXECUTOR_H
#define COMMANDLINEEXECUTOR_H

#include <QPlainTextEdit>

#include <iostream>
#include <string>
#include <vector>

#include <QDir> // only for build section to find out if cmake-build already exists...
#include <QString>

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
    
    // second argument represents output widget to append data constantly
    std::string ExecuteCommand(const std::string& cmd, QPlainTextEdit *edit = nullptr);

    void setCompiler(const std::string &compiler, const BuildMode &mode, const std::string &addflags);
    void setExecutableName(const std::string &name, const std::string &path);
    void setSourceFiles(const std::vector<std::string> &sources);
    void setLibraryPaths(const std::vector<std::string> &library_paths);
    std::string Build(const bool &cmake, QPlainTextEdit *edit = nullptr);
    std::string Execute(const bool &cmake, QPlainTextEdit *edit = nullptr);
    // kill running process with pid
    // TODO: set argument executable name to avoid QSettings usage here
    int getPid();
    void killProcess();

    std::string ClangFormat(const std::vector<std::string> &sources);
    std::string ClangTidy(const std::vector<std::string> &sources);
    std::string ClangCheck(const std::vector<std::string> &sources);

    std::string Valgrind();

    void OpenGdbGui();

    std::string ProjectRootDir = "";
    std::string cmake_build = "";
    std::string cmake_exec = "";


    std::string compile_args = "/usr/bin/";
    std::string exec_args = "";

private:

    void DetermineCompilerVersion(const std::string &tool);


    std::string version = "";
    std::string flags = "";

    std::string executable_name = "a.out";   //    executable.elf
    int proc_id;
    std::string Project_Dir = "";        //    /home/adam/Desktop/SKUSKA/

};





#endif // COMMANDLINEEXECUTOR_H
