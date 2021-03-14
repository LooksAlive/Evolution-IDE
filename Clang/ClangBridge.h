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

    /*
    // preamble:
    std::vector<Diag> Diags;
    IncludeStructure Includes;
    MainFileMacros Macros;
    std::unique_ptr<PreambleFileStatusCache> StatCache;
    CanonicalIncludes CanonIncludes;

    //
    CompilerInvocation Invocation;
    llvm::MemoryBuffer *MainFileBuffer; // not pointer !!

    const LangOptions LangOpts;
    const llvm::MemoryBuffer *Buffer;
    unsigned MaxLines;

    DiagnosticsEngine *Diagnostics;
    IntrusiveRefCntPtr<llvm::vfs::FileSystem> VFS;
    std::shared_ptr<PCHContainerOperations> PCHContainerOps;
    PreambleCallbacks Callbacks;

    // parsed AST:
    std::shared_ptr<const PreambleData> preambleData;
    llvm::Optional<ParsedAST> parsedAST;
    llvm::ArrayRef<Diag> CompilerInvocationDiags;

    ParseOptions parseOpts;
    ParseInputs Inputs;

    CompilerInvocation CI;

    RealThreadsafeFS TFS;
    */
    PreambleData *PData = nullptr;

    void setCompilationArguments(const std::string &args);

    void addDocument(PathRef File, StringRef Contents,
                     llvm::StringRef Version = "null",
                     WantDiagnostics WD = WantDiagnostics::Auto,
                     bool ForceRebuild = false);

    void removeDocument(PathRef File);

    void codeComplete(PathRef File, Position Pos, const clangd::CodeCompleteOptions &Opts);

    clangd::CodeCompleteOptions codeCompleteOptions;
    CodeCompleteResult codeCompleteResults;

    void signatureHelp(PathRef File, Position Pos);

    SignatureHelp signatureHelpResults;

    // Find declaration/definition locations of symbol at a specified position.
    void locateSymbolAt(PathRef File, Position Pos);

    std::vector<LocatedSymbol> locatedDefDecl;

    void rename(PathRef File, Position Pos, llvm::StringRef NewName,
                const RenameOptions &Opts);

    RenameOptions renameOptions;

    // format only partition of code or whole file(s)
    void formatCode(PathRef File, llvm::StringRef Code,
                    ArrayRef<tooling::Range> Ranges);

    // Get code hover for a given position.
    void findHover(PathRef File, Position Pos);

    llvm::Optional<HoverInfo> hoverInfo;

    // Resolve type hierarchy item in the given direction.
    void typeHierarchy(PathRef File, Position Pos, int Resolve,
                       TypeHierarchyDirection Direction);

    llvm::Optional<TypeHierarchyItem> TypeHierarchy;

    void resolveTypeHierarchy(TypeHierarchyItem Item, int Resolve,
                              TypeHierarchyDirection Direction);

    // Retrieve ranges that can be used to fold code within the specified file.
    void foldingRanges(StringRef File);

    std::vector<FoldingRange> foldRanges;

    // Retrieve locations for symbol references.
    void findReferences(PathRef File, Position Pos, uint32_t Limit);

    ReferencesResult refResults;

    // run results through switch and set text colors
    void semanticHighlights(PathRef File);

    std::vector<HighlightingToken> highlightingTokens;

private:

};


#endif
