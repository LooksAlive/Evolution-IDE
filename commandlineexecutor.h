#ifndef COMMANDLINEEXECUTOR_H
#define COMMANDLINEEXECUTOR_H

#include <QPlainTextEdit>

#include <iostream>
#include <string>
#include <vector>

#include <QDir>// only for build section to find out if cmake-build already exists...
#include <QString>
#include <QThread>

class ExecutionHandler : public QObject {
    Q_OBJECT
public:
    explicit ExecutionHandler(QObject *parent = nullptr);
    ~ExecutionHandler() = default;

    // actual command to run
    std::string args;

public slots:
    // second argument represents output widget to append data constantly
    // ????????? will there be any conflict with edit class ?? since it is Qt
    void ExecuteCommand();

signals:
    void addMessage(const QString &msg);
};


class CommandLineExecutor : public QObject {
    Q_OBJECT
public:
    explicit CommandLineExecutor(QObject *parent = nullptr);

    ~CommandLineExecutor() = default;

    enum BuildMode {
        Debug = 1,
        Release = 2,
    };
    // basic command on the main thread to take no longer than second
    static std::string ExecuteSimpleCommand(const std::string &cmd);

    void setSourceFiles(const std::vector<std::string> &sources);
    void setLibraryPaths(const std::vector<std::string> &library_paths);
    void Build(const bool &cmake, const std::string &ProjectRootDir, QPlainTextEdit *editor = nullptr);
    void Execute(const bool &cmake, const std::string &executable_path, QPlainTextEdit *editor = nullptr);
    // kill running process with pid
    // TODO: set argument executable name to avoid QSettings usage here
    static int getPid(const std::string &executable_path);
    static void killProcess(const int &proc_id);

    // clang_format_file_path -> .clang-format file with conf-s
    static std::string ClangFormat(const std::vector<std::string> &sources, const std::string &clang_format_file_path);
    static std::string ClangTidy(const std::vector<std::string> &sources);
    static std::string ClangCheck(const std::vector<std::string> &sources);

    static std::string Valgrind(const std::string &executable_path);

    static void OpenGdbGui(const std::string &executable_path);

    std::string compile_args = "/usr/bin/";

private:
    static void DetermineCompilerVersion(const std::string &tool);

    // consider only stack allocation
    QThread *ExecutionThread;
    ExecutionHandler *executionHandler;

    QPlainTextEdit *edit;

private slots:
    void setMessage(const QString &msg) const;
};





#endif // COMMANDLINEEXECUTOR_H
