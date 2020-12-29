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





void CmakeGenerator::generateSources(){

    for (unsigned int i = 0; i < m_sources.size(); i++) {
        g_sources += m_sources[i] + "\n";
    }
    g_sources += ")";
}


void CmakeGenerator::generateExecutable(){
    g_executable = "add_executable(" +  m_name + "${SOURCE_FILES}" + ")";
}

void CmakeGenerator::generateLibraryPathsList(){

    for (unsigned int i = 0; i < m_lpaths.size(); i++) {
        g_lpaths += m_lpaths[i] + "\n";
    }
    g_lpaths += ")";
}

void CmakeGenerator::generateLinkLibraries(){
    g_link_libraries = m_name + " " + "${ADDITIONAL_LIBRARY_PATHS}";
}






void CmakeGenerator::createCmakeLists(const std::string &path){
    cmakelists_path = path; // get from filemanager -> Rootdir

    g_buffer += m_minimum_cmake_version + "\n";
    g_buffer += g_name + "\n\n";
    g_buffer += "set(CMAKE_CXX_COMPILER " + m_compiler + ")";
    g_buffer += "set(CXX_FLAGS " + m_flags + ")";
    g_buffer += m_cmake_code + "\n";    // additional code added by user
    g_buffer += g_sources + "\n\n";
    g_buffer += g_link_libraries + "\n\n";


    QFile file(QString::fromStdString(path));
    file.write(g_buffer.c_str());

}
