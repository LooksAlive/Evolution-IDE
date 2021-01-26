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

#include "parser.h"

ClangBridge::ClangBridge() {

}

void ClangBridge::addDocument(PathRef File, StringRef Contents, llvm::StringRef Version, WantDiagnostics WD,
                              bool ForceRebuild) {

}

void ClangBridge::removeDocument(PathRef File) {

}

void ClangBridge::codeComplete(PathRef File, Position Pos, const clangd::CodeCompleteOptions &Opts,
                               Callback <CodeCompleteResult> CB) {

}

void ClangBridge::signatureHelp(PathRef File, Position Pos, Callback <SignatureHelp> CB) {

}

void ClangBridge::locateSymbolAt(PathRef File, Position Pos, Callback <std::vector<LocatedSymbol>> CB) {

}

void ClangBridge::switchSourceHeader(PathRef Path, Callback <llvm::Optional<clangd::Path>> CB) {

}

void ClangBridge::findDocumentHighlights(PathRef File, Position Pos, Callback <std::vector<DocumentHighlight>> CB) {

}

void ClangBridge::findHover(PathRef File, Position Pos, Callback <llvm::Optional<HoverInfo>> CB) {

}

void ClangBridge::typeHierarchy(PathRef File, Position Pos, int Resolve, TypeHierarchyDirection Direction,
                                Callback <llvm::Optional<TypeHierarchyItem>> CB) {

}

void ClangBridge::resolveTypeHierarchy(TypeHierarchyItem Item, int Resolve, TypeHierarchyDirection Direction,
                                       Callback <llvm::Optional<TypeHierarchyItem>> CB) {

}

void ClangBridge::workspaceSymbols(StringRef Query, int Limit, Callback <std::vector<SymbolInformation>> CB) {

}

void ClangBridge::documentSymbols(StringRef File, Callback <std::vector<DocumentSymbol>> CB) {

}

void ClangBridge::foldingRanges(StringRef File, Callback <std::vector<FoldingRange>> CB) {

}

void ClangBridge::findReferences(PathRef File, Position Pos, uint32_t Limit, Callback <ReferencesResult> CB) {

}

void ClangBridge::formatRange(PathRef File, StringRef Code, Range Rng, Callback <tooling::Replacements> CB) {

}

void ClangBridge::formatFile(PathRef File, StringRef Code, Callback <tooling::Replacements> CB) {

}

void ClangBridge::formatOnType(PathRef File, StringRef Code, Position Pos, StringRef TriggerText,
                               Callback <std::vector<TextEdit>> CB) {

}

void ClangBridge::prepareRename(PathRef File, Position Pos, const RenameOptions &RenameOpts,
                                Callback <llvm::Optional<Range>> CB) {

}

void ClangBridge::rename(PathRef File, Position Pos, llvm::StringRef NewName, const RenameOptions &Opts,
                         Callback <FileEdits> CB) {

}

void ClangBridge::enumerateTweaks(PathRef File, Range Sel, Callback <std::vector<TweakRef>> CB) {

}

void ClangBridge::applyTweak(PathRef File, Range Sel, StringRef ID, Callback <Tweak::Effect> CB) {

}

void ClangBridge::onFileEvent(const DidChangeWatchedFilesParams &Params) {

}

void ClangBridge::symbolInfo(PathRef File, Position Pos, Callback <std::vector<SymbolDetails>> CB) {

}

void ClangBridge::semanticRanges(PathRef File, const std::vector<Position> &Pos,
                                 Callback <std::vector<SelectionRange>> CB) {

}

void ClangBridge::documentLinks(PathRef File, Callback <std::vector<DocumentLink>> CB) {

}

void ClangBridge::semanticHighlights(PathRef File, Callback <std::vector<HighlightingToken>>) {

}

void ClangBridge::customAction(PathRef File, llvm::StringRef Name, Callback <InputsAndAST> Action) {

}

llvm::StringMap<TUScheduler::FileStats> ClangBridge::fileStats() const {
    //return nullptr;
}

bool ClangBridge::blockUntilIdleForTest(llvm::Optional<double> TimeoutSeconds) {
    return false;
}

void ClangBridge::formatCode(PathRef File, llvm::StringRef Code, ArrayRef <tooling::Range> Ranges,
                             Callback <tooling::Replacements> CB) {

}

Context ClangBridge::createProcessingContext(PathRef) const {
    //return nullptr;
}


