#include "commandlineexecutor.h"
#include <QSettings>
#include <QString>
#include <csignal>

CommandLineExecutor::CommandLineExecutor(QObject *parent) : QObject(parent) {}


void CommandLineExecutor::ExecuteCommand(const std::string &cmd, QPlainTextEdit *edit) {
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    // ????
    //cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                if (edit) {
                    // constantly update cmd output
                    edit->appendPlainText(buffer);// const_cast<char *>(buffer)
                    // buffer = "";
                }
            }
        }
        pclose(stream);
    }
}

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

void CommandLineExecutor::Build(const bool &cmake, const std::string &ProjectRootDir, QPlainTextEdit *edit) {

    if (cmake) {// later -> cmake only generates file(cmake ..), so real build (make -j2) separate later,
        // also num of cpu cores as argument to build + add them to non cmake build
        std::string cmake_build;
        QDir dir;
        if (dir.exists(QString::fromStdString(ProjectRootDir) + "/cmake-build")) {
            cmake_build = "cd " + ProjectRootDir + "/cmake-build" + " && cmake .. && make -j2 ";
            if (edit) {
                edit->appendPlainText(cmake_build.c_str());
            }
        } else {
            cmake_build += "mkdir " + ProjectRootDir + "/cmake-build && cd " + ProjectRootDir + "/cmake-build " +
                           " && cmake .. && make -j2";
            if (edit) {
                edit->appendPlainText(cmake_build.c_str());
            }
        }

        // QObject::connect: Cannot queue arguments of type 'QTextCursor'
        // (Make sure 'QTextCursor' is registered using qRegisterMetaType().)
        //qRegisterMetaType<QTextCursor>("QTextCursor");
        /*
        worker = new QThread();
        runner = new ExecutionRunner();

        connect(worker, &QThread::started, this, &CommandLineExecutor::ExecuteCommand(cmake_build, edit));
        connect(worker, &QThread::finished, this, &QObject::deleteLater);
        ExecutionRunner->moveToThread(worker);
        worker->start();
        */
        //QThread *thread = QThread::create(&CommandLineExecutor::ExecuteCommand, cmake_build, edit);
        //thread->start();

        ExecuteCommand(cmake_build, edit);
    } else {
        std::string compile_args;
        if (edit) {
            edit->appendPlainText(compile_args.c_str());
        }
        // string will be returned into console
        ExecuteCommand(compile_args, edit);
    }
}
void CommandLineExecutor::Execute(const bool &cmake, const std::string &executable_path, QPlainTextEdit *edit) {

    std::string cmake_exec;
    if (cmake) {
        cmake_exec += executable_path;// ProjectRootDir;   Project_Dir + "/cmake-build/" + executable_name
        if (edit) {
            edit->appendPlainText(cmake_exec.c_str());
        }
        /*
        std::thread th(&CommandLineExecutor::ExecuteCommand, cmake_exec, edit);
        if(th.joinable()){
            th.join();
        }
        */
        ExecuteCommand(cmake_exec, edit);
    } else {
        if (edit) {
            edit->appendPlainText(executable_path.c_str());
        }
        ExecuteCommand(executable_path, edit);
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
