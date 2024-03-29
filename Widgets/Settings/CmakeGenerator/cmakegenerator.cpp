#include "cmakegenerator.h"

CmakeGenerator::CmakeGenerator()
{

}

void CmakeGenerator::addSourceFile(const std::string &file){

    m_sources.push_back(file);
}

void CmakeGenerator::setProjectName(const std::string &project_name){
    m_name = project_name;
    g_name = "project(" + project_name + ")";
}

void CmakeGenerator::setLibraryPath(const std::string &lpath){
    m_lpaths.push_back(lpath);
}

void CmakeGenerator::setCompiler(const std::string &compiler){
    m_compiler = compiler;
}

void CmakeGenerator::setNotParsedCmakeCode(const std::string &code){
    m_cmake_code = code;
}

void CmakeGenerator::setCompileFlags(const std::string &flags){
    m_flags = flags;
}





void CmakeGenerator::generateSources() {

    for (auto &m_source : m_sources) {
        g_sources += m_source + "\n";
    }
    g_sources += ")";
}


void CmakeGenerator::generateExecutable(){
    g_executable = "add_executable(" +  m_name + " ${SOURCE_FILES}" + ")";
}

void CmakeGenerator::generateLibraryPathsList() {

    for (auto &m_lpath : m_lpaths) {
        g_lpaths += m_lpath + "\n";
    }
    g_lpaths += ")";
}

void CmakeGenerator::generateLinkLibraries(){
    g_link_libraries = m_name + " " + "${ADDITIONAL_LIBRARY_PATHS}";
}






void CmakeGenerator::createCmakeLists(const std::string &path){
    cmakelists_path = path; // get from filemanager -> Rootdir ||||  CMakeLists.txt not contained !

    g_buffer += m_minimum_cmake_version + "\n";
    g_buffer += g_name + "\n\n";
    g_buffer += "set(CMAKE_CXX_COMPILER " + m_compiler + ") \n\n";
    g_buffer += "set(CXX_FLAGS " + m_flags + ") \n\n";
    g_buffer += m_cmake_code + "\n";    // additional code added by user
    generateSources();
    g_buffer += g_sources + "\n\n";
    generateExecutable();
    g_buffer += g_executable + "\n\n";
    //g_buffer += g_link_libraries + "\n\n";


    QString cmake_path = QString::fromStdString(path) + "/CMakeLists.txt";

    QFile file(cmake_path);
    if(file.open(QFile::ReadWrite)) {
        file.write(g_buffer.c_str());
    }
}
