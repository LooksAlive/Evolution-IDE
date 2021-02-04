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

/// Manages a collection of source files and derived data (ASTs, indexes),
/// and provides language-aware features such as code completion.
///
/// The primary client is ClangdLSPServer which exposes these features via
/// the Language Server protocol. ClangdServer may also be embedded directly,
/// though its API is not stable over time.
///
/// ClangdServer should be used from a single thread. Many potentially-slow
/// operations have asynchronous APIs and deliver their results on another
/// thread.
/// Such operations support cancellation: if the caller sets up a cancelable
/// context, many operations will notice cancellation and fail early.
/// (ClangdLSPServer uses this to implement $/cancelRequest).
class ClangBridge {
public:
    struct Options {
        /// To process requests asynchronously, ClangdServer spawns worker threads.
        /// If this is zero, no threads are spawned. All work is done on the calling
        /// thread, and callbacks are invoked before "async" functions return.
        unsigned AsyncThreadsCount = getDefaultAsyncThreadsCount();

        /// AST caching policy. The default is to keep up to 3 ASTs in memory.
        ASTRetentionPolicy RetentionPolicy;

        /// Cached preambles are potentially large. If false, store them on disk.
        bool StorePreamblesInMemory = true;
        /// Reuse even stale preambles, and rebuild them in the background.
        /// This improves latency at the cost of accuracy.
        bool AsyncPreambleBuilds = true;

        /// If true, ClangdServer builds a dynamic in-memory index for symbols in
        /// opened files and uses the index to augment code completion results.
        bool BuildDynamicSymbolIndex = false;
        /// Use a heavier and faster in-memory index implementation.
        bool HeavyweightDynamicSymbolIndex = true;
        /// If true, ClangdServer automatically indexes files in the current project
        /// on background threads. The index is stored in the project root.
        bool BackgroundIndex = false;

        /// Store refs to main-file symbols in the index.
        bool CollectMainFileRefs = false;

        /// If set, use this index to augment code completion results.
        SymbolIndex *StaticIndex = nullptr;

        /// If set, queried to obtain the configuration to handle each request.
        config::Provider *ConfigProvider = nullptr;

        /// If set, enable clang-tidy in clangd and use to it get clang-tidy
        /// configurations for a particular file.
        /// Clangd supports only a small subset of ClangTidyOptions, these options
        /// (Checks, CheckOptions) are about which clang-tidy checks will be
        /// enabled.
        ClangTidyOptionsBuilder GetClangTidyOptions;

        /// If true, turn on the `-frecovery-ast` clang flag.
        bool BuildRecoveryAST = true;

        /// If true, turn on the `-frecovery-ast-type` clang flag.
        bool PreserveRecoveryASTType = true;

        /// Clangd's workspace root. Relevant for "workspace" operations not bound
        /// to a particular file.
        /// FIXME: If not set, should use the current working directory.
        llvm::Optional<std::string> WorkspaceRoot;

        /// The resource directory is used to find internal headers, overriding
        /// defaults and -resource-dir compiler flag).
        /// If None, ClangdServer calls CompilerInvocation::GetResourcePath() to
        /// obtain the standard resource directory.
        llvm::Optional<std::string> ResourceDir = llvm::None;

        /// Time to wait after a new file version before computing diagnostics.
        DebouncePolicy UpdateDebounce = DebouncePolicy{
                /*Min=*/std::chrono::milliseconds(50),
                /*Max=*/std::chrono::milliseconds(500),
                /*RebuildRatio=*/1,
        };

        bool SuggestMissingIncludes = false;

        /// Clangd will execute compiler drivers matching one of these globs to
        /// fetch system include path.
        std::vector<std::string> QueryDriverGlobs;

        /// Enable notification-based semantic highlighting.
        bool TheiaSemanticHighlighting = false;

        /// Enable preview of FoldingRanges feature.
        bool FoldingRanges = false;

        /// Returns true if the tweak should be enabled.
        std::function<bool(const Tweak &)> TweakFilter = [](const Tweak &T) {
            return !T.hidden(); // only enable non-hidden tweaks.
        };

        explicit operator TUScheduler::Options() const;
    };

    // Sensible default options for use in tests.
    // Features like indexing must be enabled if desired.
    static Options optsForTest();

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
    config::Provider *ConfigProvider = nullptr;

    // const ThreadsafeFS &TFS;

    Path ResourceDir;
    // The index used to look up symbols. This could be:
    //   - null (all index functionality is optional)
    //   - the dynamic index owned by ClangdServer (DynamicIdx)
    //   - the static index passed to the constructor
    //   - a merged view of a static and dynamic index (MergedIndex)
    const SymbolIndex *Index = nullptr;
    // If present, an index of symbols in open files. Read via *Index.
    std::unique_ptr<FileIndex> DynamicIdx;
    // If present, the new "auto-index" maintained in background threads.
    std::unique_ptr<BackgroundIndex> BackgroundIdx;
    // Storage for merged views of the various indexes.
    std::vector<std::unique_ptr<SymbolIndex>> MergedIdx;

    // When set, provides clang-tidy options for a specific file.
    ClangTidyOptionsBuilder GetClangTidyOptions;

    // If this is true, suggest include insertion fixes for diagnostic errors that
    // can be caused by missing includes (e.g. member access in incomplete type).
    bool SuggestMissingIncludes = false;

    // If true, preserve expressions in AST for broken code.
    bool BuildRecoveryAST = true;
    // If true, preserve the type for recovery AST.
    bool PreserveRecoveryASTType = false;

    std::function<bool(const Tweak &)> TweakFilter;

    // GUARDED_BY(CachedCompletionFuzzyFindRequestMutex)
    llvm::StringMap<llvm::Optional<FuzzyFindRequest>>
            CachedCompletionFuzzyFindRequestByFile;
    mutable std::mutex CachedCompletionFuzzyFindRequestMutex;

    llvm::Optional<std::string> WorkspaceRoot;
    // WorkScheduler has to be the last member, because its destructor has to be
    // called before all other members to stop the worker thread that references
    // ClangdServer.
    // TUScheduler WorkScheduler;
};


#endif