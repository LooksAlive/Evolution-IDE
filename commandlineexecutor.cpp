#include "commandlineexecutor.h"
#include <signal.h>
#include <QString>
#include <QSettings>

CommandLineExecutor::CommandLineExecutor()
{
    if(GenerateCmake){
        // generate cmake object in another class -> invoke
    }
}



std::string CommandLineExecutor::ExecuteCommand(const std::string& cmd, QPlainTextEdit *edit) {

  std::string data;
  FILE* stream;
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
                  edit->appendPlainText(buffer);  // const_cast<char *>(buffer)
                  // buffer = "";
              }
              data.append(buffer);
          }
      }
      pclose(stream);
  }
  return data;
}

/* compiler
------------------------------------------------------------------------ */

void CommandLineExecutor::setCompiler(const std::string &compiler, const BuildMode &mode, const std::string &addflags){

    switch (mode) {
        case Debug:
            flags += "-O0 -g";
            break;
        case Release:
            flags += "-O3";
            break;
        default: break;
    }
    //DetermineCompilerVersion(compiler);
    compile_args += compiler + version + " " + flags + " " + addflags;
}

void CommandLineExecutor::setExecutableName(const std::string &name, const std::string &path){
    executable_name = name;
    Project_Dir = path;
    compile_args += " -o " + Project_Dir + "/" + name;
}

void CommandLineExecutor::setSourceFiles(const std::vector<std::string> &sources){

    for (unsigned int i = 0; i < sources.size(); i++) {
        compile_args += " " + sources[i];
    }
}

void CommandLineExecutor::setLibraryPaths(const std::vector<std::string> &library_paths){

    compile_args += "-L ";
    for (unsigned int i = 0; i < library_paths.size(); i++) {
        compile_args += " " + library_paths[i];
    }

}

void CommandLineExecutor::DetermineCompilerVersion(const std::string &tool){
    // find version in linux ( ls /usr/bin | grep -i "some_tool[-]" ----->>> parse, get version )
    std::string temp = ExecuteCommand("ls /usr/bin | grep -i '" + tool + "-'"  );
    // version = temp[end???];
}

std::string CommandLineExecutor::Build(const bool &cmake, QPlainTextEdit *edit){

    std::string output;
    if(cmake){ // later -> cmake only generates file(cmake ..), so real build (make -j2) separate later,
        // also num of cpu cores as argument to build + add them to non cmake build
        QDir dir;
        if(dir.exists(QString::fromStdString(ProjectRootDir) + "/cmake-build")) {
            cmake_build = "cd " + ProjectRootDir + "/cmake-build" + " && cmake .. && make -j2 ";
        }
        else {
            cmake_build += "mkdir " + ProjectRootDir + "/cmake-build && cd " + ProjectRootDir + "/cmake-build " +
                    " && cmake .. && make -j2";
        }
        output = ExecuteCommand(cmake_build, edit);
    }
    else{
        // string will be returned into console
        output = ExecuteCommand(compile_args, edit);
    }

    return output;
}
std::string CommandLineExecutor::Execute(const bool &cmake, QPlainTextEdit *edit){

    std::string output;
    if(cmake){
        cmake_exec += Project_Dir + "/cmake-build/" + executable_name; // ProjectRootDir
        output = ExecuteCommand(cmake_exec, edit);
    }
    else {
        // string will be returned into console
        exec_args = Project_Dir + "/" + executable_name;
        output = ExecuteCommand(exec_args, edit);
    }
    return output;
}

/* clang tools
------------------------------------------------------------------------ */

std::string CommandLineExecutor::ClangFormat(const std::vector<std::string> &sources){
    std::string clang_format_args = "/usr/bin/clang-format ";
    std::string base_flags;

    for (unsigned int i = 0; i < sources.size(); i++) {
        clang_format_args += " " + sources[i];
    }
    clang_format_args += base_flags;
    std::string output = ExecuteCommand(clang_format_args);

    return output;
}

std::string CommandLineExecutor::ClangCheck(const std::vector<std::string> &sources){
    std::string clang_check_args = "/usr/bin/clang-check ";
    std::string base_flags;

    for (unsigned int i = 0; i < sources.size(); i++) {
        clang_check_args += " " + sources[i];
    }
    clang_check_args += base_flags;
    std::string output = ExecuteCommand(clang_check_args);

    return output;
}

std::string CommandLineExecutor::ClangTidy(const std::vector<std::string> &sources){
    std::string clang_tidy_args = "/usr/bin/clang-tidy ";
    std::string base_flags;

    for (unsigned int i = 0; i < sources.size(); i++) {
        clang_tidy_args += " " + sources[i];
    }
    clang_tidy_args += base_flags;
    std::string output = ExecuteCommand(clang_tidy_args);

    return output;
}

/* other tools
------------------------------------------------------------------------ */

std::string CommandLineExecutor::Valgrind(){
    std::string valgrind_args = "/usr/bin/valgrind --tool=memcheck ./" + executable_name;
    std::string base_flags;

    valgrind_args += base_flags;
    std::string output = ExecuteCommand(valgrind_args);

    return output;
}

void CommandLineExecutor::OpenGdbGui(){
    ExecuteCommand("/usr/bin/gdbgui ./" + executable_name);
}

int CommandLineExecutor::getPid() {
    if(executable_name.empty()){
        QSettings settings("Evolution");
        executable_name = settings.value("Evolution/executable").toString().toStdString();
    }
    std::string my_pid = ExecuteCommand("pidof -s " + executable_name);
    proc_id = strtoul(my_pid.c_str(), NULL, 10);
    return proc_id;
}

void CommandLineExecutor::killProcess() {
    kill(proc_id, SIGKILL);
}
