#include "commandlineexecutor.h"

CommandLineExecutor::CommandLineExecutor()
{
    if(GenerateCmake){
        // generate cmake object in another class -> invoke
    }
}


void CommandLineExecutor::setCompiler(const std::string &compiler, const std::string &version, const BuildMode &mode, const std::string &addflags){

    switch (mode) {
        case Debug:
            flags += "-O0 -g";
            break;
        case Release:
            flags += "-O3";
            break;
        default: break;
    }
    args = compiler + version + " " + addflags;
}

void CommandLineExecutor::DetermineCompilerVersion(){
    // find version in linux
}

std::string CommandLineExecutor::Compile(){
    // string will be returned into console
}
