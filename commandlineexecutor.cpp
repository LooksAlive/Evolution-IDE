#include "commandlineexecutor.h"
#include <QDebug>
#include <QSettings>
#include <QString>
#include <csignal>

CommandLineExecutor::CommandLineExecutor(QObject *parent) : QObject(parent) {

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), process, SLOT(kill()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotSetOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(slotSetOutput()));
}

CommandLineExecutor::~CommandLineExecutor() {
    //delete buildHandler;
    //BuildThread->deleteLater();
    //delete executionHandler;
    //ExecutionThread->deleteLater();
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

void CommandLineExecutor::Build(const bool &cmake, const std::string &ProjectRootDir, ConsoleDock *editor) {
    edit = editor;

    if (cmake) {// later -> cmake only generates file(cmake ..), so real build (make -j2) separate later,
        // also num of cpu cores as argument to build + add them to non cmake build
        std::string cmake_build;
        QDir dir;
        if (dir.exists(QString::fromStdString(ProjectRootDir) + "/cmake-build")) {
            cmake_build = "cd " + ProjectRootDir + "/cmake-build" + " && /bin/cmake .. && /bin/make -j2 ";
            if (edit) {
                edit->ConsoleOutput->append(cmake_build.c_str());
            }

            process->setWorkingDirectory(QString::fromStdString(ProjectRootDir) + "/cmake-build");
            process->start("/bin/cmake", QStringList() << "..", QProcess::ReadWrite);
            if (!process->waitForFinished())
                qDebug() << "Process failed: " << process->errorString();
            process->start("/bin/make", QStringList() << "-j2", QProcess::ReadWrite);
            if (!process->waitForFinished())
                qDebug() << "Process failed: " << process->errorString();

        } else {
            cmake_build += "/bin/mkdir " + ProjectRootDir + "/cmake-build && cd " + ProjectRootDir + "/cmake-build " +
                           " && /bin/cmake .. && /bin/make -j2";
            if (edit) {
                edit->ConsoleOutput->append(cmake_build.c_str());
            }

            process->start("/bin/mkdir", QStringList() << QString::fromStdString(ProjectRootDir) + "/cmake-build", QProcess::ReadWrite);
            process->waitForFinished();
            process->setWorkingDirectory(QString::fromStdString(ProjectRootDir) + "/cmake-build");
            process->start("/bin/cmake", QStringList() << "..", QProcess::ReadWrite);
            if (!process->waitForFinished())
                qDebug() << "Process failed: " << process->errorString();
            process->start("/bin/make", QStringList() << "-j2", QProcess::ReadWrite);
            if (!process->waitForFinished())
                qDebug() << "Process failed: " << process->errorString();
            emit processFinished();
        }

    } else {
        std::string compile_args;
        // string will be returned into console
    }
}
void CommandLineExecutor::Execute(const bool &cmake, const std::string &executable_path, ConsoleDock *editor) {
    edit = editor;
    std::string cmake_exec;

    if (cmake) {
        cmake_exec += executable_path;// ProjectRootDir;   Project_Dir + "/cmake-build/" + executable_name
        if (edit) {
            edit->ConsoleOutput->append(cmake_exec.c_str());
        }

        process->start(QString::fromStdString(cmake_exec), QStringList(), QProcess::ReadWrite);
        if (!process->waitForFinished())
            qDebug() << "Process failed: " << process->errorString();
        emit processFinished();

    } else {
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

void CommandLineExecutor::slotSetOutput() {
    qDebug() << "in slot !!!";
    if (!process->waitForFinished())
        qDebug() << "Process failed: " << process->errorString();

    edit->processText(QString(process->readAllStandardOutput()));
    edit->processText(QString(process->readAllStandardError()));
}
