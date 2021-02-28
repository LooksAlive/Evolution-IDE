#include "commandlineexecutor.h"
#include <QSettings>
#include <QString>
#include <csignal>

ExecutionHandler::ExecutionHandler(QObject *parent) : QObject(parent) {}

void ExecutionHandler::ExecuteCommand() {
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(args.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                emit addMessage(buffer);
            }
        }
        pclose(stream);
    }
}


CommandLineExecutor::CommandLineExecutor(QObject *parent) : QObject(parent) {}

std::string CommandLineExecutor::ExecuteSimpleCommand(const std::string &cmd) {
    std::string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    // ????
    //cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                data.append(buffer);
            }
        }
        pclose(stream);
    }
    return data;
}

/* compiler
------------------------------------------------------------------------ */

void CommandLineExecutor::setSourceFiles(const std::vector<std::string> &sources) {

    for (const auto &source : sources) {
        compile_args += " " + source;
    }
}

void CommandLineExecutor::setLibraryPaths(const std::vector<std::string> &library_paths) {

    compile_args += "-L ";
    for (const auto &library_path : library_paths) {
        compile_args += " " + library_path;
    }
}

void CommandLineExecutor::DetermineCompilerVersion(const std::string &tool){
    // find version in linux ( ls /usr/bin | grep -i "some_tool[-]" ----->>> parse, get version )
    ExecuteSimpleCommand("ls /usr/bin | grep -i '" + tool + "-'");
    // version = temp[end???];
}

void CommandLineExecutor::Build(const bool &cmake, const std::string &ProjectRootDir, ConsoleDock *editor) {
    edit = editor;
    ExecutionThread = new QThread(this);
    executionHandler = new ExecutionHandler();

    connect(ExecutionThread, &QThread::started, executionHandler, &ExecutionHandler::ExecuteCommand);
    connect(ExecutionThread, &QThread::finished, this, &QObject::deleteLater);

    connect(executionHandler, SIGNAL(ExecutionHandler::addMessage(const QString &)), this, SLOT(setMessage(const QString &)));
    executionHandler->moveToThread(ExecutionThread);
    if (cmake) {// later -> cmake only generates file(cmake ..), so real build (make -j2) separate later,
        // also num of cpu cores as argument to build + add them to non cmake build
        std::string cmake_build;
        QDir dir;
        if (dir.exists(QString::fromStdString(ProjectRootDir) + "/cmake-build")) {
            cmake_build = "cd " + ProjectRootDir + "/cmake-build" + " && cmake .. && make -j2 ";
            if (edit) {
                edit->processText(cmake_build.c_str());
            }
        } else {
            cmake_build += "mkdir " + ProjectRootDir + "/cmake-build && cd " + ProjectRootDir + "/cmake-build " +
                           " && cmake .. && make -j2";
            if (edit) {
                edit->processText(cmake_build.c_str());
            }
        }
        executionHandler->args = cmake_build;
        ExecutionThread->start();

        //ExecuteCommand(cmake_build, edit);
    } else {
        std::string compile_args;
        executionHandler->args = compile_args;
        ExecutionThread->start();
        // string will be returned into console
        //ExecuteCommand(compile_args, edit);
    }
}
void CommandLineExecutor::Execute(const bool &cmake, const std::string &executable_path, ConsoleDock *editor) {
    edit = editor;
    std::string cmake_exec;
    ExecutionThread = new QThread(this);
    executionHandler = new ExecutionHandler();

    connect(ExecutionThread, &QThread::started, executionHandler, &ExecutionHandler::ExecuteCommand);
    connect(ExecutionThread, &QThread::finished, this, &QObject::deleteLater);

    connect(executionHandler, SIGNAL(ExecutionHandler::addMessage(const QString &)), this, SLOT(setMessage(const QString &)));
    executionHandler->moveToThread(ExecutionThread);

    if (cmake) {
        cmake_exec += executable_path;// ProjectRootDir;   Project_Dir + "/cmake-build/" + executable_name
        if (edit) {
            edit->processText(cmake_exec.c_str());
        }

        executionHandler->args = cmake_exec;
        ExecutionThread->start();

    } else {
        executionHandler->args = cmake_exec;
        ExecutionThread->start();
        //ExecuteCommand(executable_path, edit);
    }
}

/* clang tools
------------------------------------------------------------------------ */

std::string CommandLineExecutor::ClangFormat(const std::vector<std::string> &sources, const std::string &clang_format_file_path) {
    std::string clang_format_args = "/usr/bin/clang-format " + clang_format_file_path;

    for (const auto &source : sources) {
        clang_format_args += " " + source;
    }
    return ExecuteSimpleCommand(clang_format_args);
}

std::string CommandLineExecutor::ClangCheck(const std::vector<std::string> &sources) {
    std::string clang_check_args = "/usr/bin/clang-check ";
    std::string base_flags;

    for (const auto &source : sources) {
        clang_check_args += " " + source;
    }
    clang_check_args += base_flags;
    return ExecuteSimpleCommand(clang_check_args);
}

std::string CommandLineExecutor::ClangTidy(const std::vector<std::string> &sources) {
    std::string clang_tidy_args = "/usr/bin/clang-tidy ";
    std::string base_flags;

    for (const auto &source : sources) {
        clang_tidy_args += " " + source;
    }
    clang_tidy_args += base_flags;
    return ExecuteSimpleCommand(clang_tidy_args);
}

/* other tools
------------------------------------------------------------------------ */

std::string CommandLineExecutor::Valgrind(const std::string &executable_path) {
    std::string valgrind_args = "/usr/bin/valgrind --tool=memcheck ./" + executable_path;
    std::string base_flags;

    valgrind_args += base_flags;
    return ExecuteSimpleCommand(valgrind_args);
}

void CommandLineExecutor::OpenGdbGui(const std::string &executable_path) {
    ExecuteSimpleCommand("/usr/bin/gdbgui ./" + executable_path);
}

int CommandLineExecutor::getPid(const std::string &executable_path) {
    if (executable_path.empty()) {
        return 0;
    }
    const std::string my_pid = ExecuteSimpleCommand("pidof -s " + executable_path);

    return static_cast<int>(my_pid.c_str(), NULL, 10);// (int)
}

void CommandLineExecutor::killProcess(const int &proc_id) {
    kill(proc_id, SIGKILL);
}

void CommandLineExecutor::setMessage(const QString &msg) const {
    edit->processText(msg);
}
