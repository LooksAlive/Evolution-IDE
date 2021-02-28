#ifndef PARSER_H
#define PARSER_H

#include "../clang-tidy/ClangTidyOptions.h"
#include "CodeComplete.h"
#include "ConfigProvider.h"
#include "GlobalCompilationDatabase.h"
#include "Hover.h"
#include "Protocol.h"
#include "SemanticHighlighting.h"
#include "TUScheduler.h"
#include "XRefs.h"
#include "index/Background.h"
#include "index/FileIndex.h"
#include "index/Index.h"
#include "refactor/Rename.h"
#include "refactor/Tweak.h"
#include "support/Cancellation.h"
#include "support/Function.h"
#include "support/ThreadsafeFS.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Core/Replacement.h"
#include "llvm/ADT/FunctionExtras.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/StringRef.h"
#include <functional>
#include <future>
#include <string>
#include <type_traits>
#include <utility>

using namespace clang;
using namespace clang::clangd;
using StringRef = llvm::StringRef;

/// When set, used by ClangdServer to get clang-tidy options for each particular
/// file. Must be thread-safe. We use this instead of ClangTidyOptionsProvider
/// to allow reading tidy configs from the VFS used for parsing.
using ClangTidyOptionsBuilder = std::function<clang::tidy::ClangTidyOptions(
        llvm::vfs::FileSystem &, llvm::StringRef /*File*/)>;

class ClangBridge {
public:
    ClangBridge();
    ~ClangBridge() = default;

    //Options Opts;

    void setCompilationArguments(const std::string &args);

    void addDocument(PathRef File, StringRef Contents,
                     llvm::StringRef Version = "null",
                     WantDiagnostics WD = WantDiagnostics::Auto,
                     bool ForceRebuild = false);

    void removeDocument(PathRef File);

    void codeComplete(PathRef File, Position Pos, const clangd::CodeCompleteOptions &Opts);

    void signatureHelp(PathRef File, Position Pos);

    void locateSymbolAt(PathRef File, Position Pos);

private:

};


#endif
