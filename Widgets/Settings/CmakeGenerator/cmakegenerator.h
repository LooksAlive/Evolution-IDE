#ifndef CMAKEGENERATOR_H
#define CMAKEGENERATOR_H

/*

  for now will generate only 1 CmakeLists.txt file for whole project, nat handling subdirectory cmake lists


  # TODO: user can add more executables, so make smaller string variables to hold whole parts and build at the END
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <QFile>
#include <QDir>

#include "cmakegenerator.h"

class CmakeGenerator
{
public:
    CmakeGenerator();

    void addSourceFile(const std::string &file); // generate list set(SOURCE_FILES ...)
    void setProjectName(const std::string &name);
    void setLibraryPath(const std::string &lpath);
    void setNotParsedCmakeCode(const std::string &code);    // inserted under project(m_name)
    void setCompiler(const std::string &compiler);
    void setCompileFlags(const std::string &flags);

    // how to manage files to fit in ???????
    //void createStaticLibrary(const std::string &llib_name);    // static library from sources
    //void createSharedLibrary(const std::string &slib_name);   // shared library from sources

    void createCmakeLists(const std::string &path = ".");

private:

    // gathered
    std::string cmakelists_path = "";
    std::string m_name = "";
    std::string m_compiler = "";
    std::string m_flags = "";
    std::string m_cmake_code = "";      // additional code added by user
    std::vector<std::string> m_sources;
    std::vector<std::string> m_lpaths;
    std::string m_minimum_cmake_version = "cmake_minimum_required(VERSION 3.16)";


    // generated
    std::string g_name = "";

    void generateSources();
    std::string g_sources = "set(SOURCE_FILES ";    // ) add after all is filled

    void generateExecutable();
    std::string g_executable = "";

    void generateLibraryPathsList();
    std::string g_lpaths = "set(ADDITIONAL_LIBRARY_PATHS ";      // ) add after all is filled

    void generateLinkLibraries();
    std::string g_link_libraries = "target_link_libraries ";



    std::string g_buffer = "";     // final buffer to write into file


};





#endif // CMAKEGENERATOR_H
