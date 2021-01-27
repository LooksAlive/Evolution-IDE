#include "ClangdServer.h"
#include "CodeComplete.h"
#include "Config.h"
#include "FindSymbols.h"
#include "Format.h"
#include "HeaderSourceSwitch.h"
#include "Headers.h"
#include "ParsedAST.h"
#include "Preamble.h"
#include "Protocol.h"
#include "SemanticHighlighting.h"
#include "SemanticSelection.h"
#include "SourceCode.h"
#include "TUScheduler.h"
#include "XRefs.h"
#include "index/CanonicalIncludes.h"
#include "index/FileIndex.h"
#include "index/Merge.h"
#include "refactor/Rename.h"
#include "refactor/Tweak.h"
#include "support/Logger.h"
#include "support/Markup.h"
#include "support/ThreadsafeFS.h"
#include "support/Trace.h"
#include "clang/Format/Format.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Core/Replacement.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/ScopeExit.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Errc.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/ScopedPrinter.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>

#include "ClangBridge.h"

ClangBridge::ClangBridge(const GlobalCompilationDatabase &CDB, const Options &Opts) {
    //GetClangTidyOptions(Opts.GetClangTidyOptions);
    SuggestMissingIncludes = Opts.SuggestMissingIncludes;
    BuildRecoveryAST = Opts.BuildRecoveryAST;
    PreserveRecoveryASTType = Opts.PreserveRecoveryASTType;
}

void ClangBridge::addDocument(PathRef File, StringRef Contents, llvm::StringRef Version, WantDiagnostics WD,
                              bool ForceRebuild) {

}

void ClangBridge::removeDocument(PathRef File) {

}

void ClangBridge::signatureHelp(PathRef File, Position Pos) {

}

void ClangBridge::locateSymbolAt(PathRef File, Position Pos) {

}

void ClangBridge::codeComplete(PathRef File, Position Pos, const clangd::CodeCompleteOptions &Opts) {

}
