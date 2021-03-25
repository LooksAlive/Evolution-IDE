#ifndef COMMANDLINEEXECUTOR_H
#define COMMANDLINEEXECUTOR_H

#include <iostream>
#include <string>
#include <vector>

#include <QDir>// only for build section to find out if cmake-build already exists...
#include <QProcess>
#include <QString>
#include <QThread>

#include "Widgets/ConsoleDock/consoledock.h"

class CommandLineExecutor : public QObject {
    Q_OBJECT
public:
    explicit CommandLineExecutor(QObject *parent = nullptr);

    ~CommandLineExecutor();

    enum BuildMode {
        Debug = 1,
        Release = 2,
    };
    static std::string ExecuteSimpleCommand(const std::string &cmd);

    void setSourceFiles(const std::vector<std::string> &sources);
    void setLibraryPaths(const std::vector<std::string> &library_paths);
    void Build(const bool &cmake, const std::string &ProjectRootDir, ConsoleDock *editor = nullptr);
    void Execute(const bool &cmake, const std::string &executable_path, ConsoleDock *editor = nullptr);
    // kill running process with pid
    // TODO: set argument executable name to avoid QSettings usage here
    static int getPid(const std::string &executable_path);
    static void killProcess(const int &proc_id);

    static std::string ClangFormat(const std::vector<std::string> &sources, const std::string &clang_format_file_path);

    static std::string ClangTidy(const std::vector<std::string> &sources);

    static std::string ClangCheck(const std::vector<std::string> &sources);

    static std::string Valgrind(const std::string &executable_path);

    static void OpenGdbGui(const std::string &executable_path);

    std::string compile_args = "/usr/bin/";

    // when some tool is not installed we have static ones in /tools
    bool useAppTools = false;

private:
    static void DetermineCompilerVersion(const std::string &tool);

    ConsoleDock *edit;

    QProcess *process;

private slots:

    void slotSetOutput();
};

#endif // COMMANDLINEEXECUTOR_H
