#ifndef PARSER_H
#define PARSER_H

#include "clangd/ClangdServer.h"
//#include "clangd/ClangdLSPServer.h"

using namespace clang::clangd;
using namespace clang;

class database : public GlobalCompilationDatabase
{

public:
    database();
    ~database();
    llvm::Optional<tooling::CompileCommand>
    getCompileCommand(PathRef File) const override;
};



class Parser
{
public:
    Parser();
};


#endif // PARSER_H
