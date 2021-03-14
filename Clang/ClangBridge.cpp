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
#include <iostream>
#include <thread>

#include "ClangBridge.h"


ClangBridge::ClangBridge() {

    //addDocument("", "", "");
    //const PreambleBounds Bounds = ComputePreambleBounds(LangOpts, MainFileBuffer, MaxLines);
    //llvm::ErrorOr<PrecompiledPreamble> precompiledPreamble = PrecompiledPreamble::Build(Invocation, MainFileBuffer, Bounds, *Diagnostics, VFS, PCHContainerOps, true, Callbacks);
    //PreambleData preambleData {Inputs, precompiledPreamble, Diags, Includes, Macros, StatCache, CanonIncludes};
    /*
    //PreambleData *preambleData;
    PreambleParsedCallback PreambleCallback;    // not setting up no idea how exactly :)
    preambleData = buildPreamble("FileName", CI,
                           Inputs, true,
                           PreambleCallback);
    //const PreambleData *preamble = preambleData.get();
    CompilerInvocation::CreateFromArgs(CI, "jkkasfh", *Diagnostics);
    parsedAST = ParsedAST::build("FileName", Inputs, std::make_unique<CompilerInvocation>(CI), llvm::ArrayRef<Diag>(), preambleData);
    */
}
/*
void ClangBridge::setCompilationArguments(const std::string &args) {
    // CDBDatabase CDB;
    // auto cmd = CDB.getCompileCommand(args);

    DirectoryBasedGlobalCompilationDatabase CDB(std::string("jskdfh"));
    Inputs.CompileCommand = CDB.getCompileCommand(args).getValue(); // fileName
    //cmd.getValue().CommandLine;

}

void ClangBridge::addDocument(PathRef File, StringRef Contents, llvm::StringRef Version, WantDiagnostics WD,
                              bool ForceRebuild) {
    parseOpts.ClangTidyOpts = tidy::ClangTidyOptions::getDefaults();

    Inputs.TFS = &TFS;
    Inputs.Contents = std::string(Contents);
    Inputs.Version = Version.str();
    Inputs.ForceRebuild = ForceRebuild;
    Inputs.Opts = std::move(parseOpts);
    //Inputs.Index = Index;
    // 2 more


}

void ClangBridge::removeDocument(PathRef File) {

}

void ClangBridge::signatureHelp(PathRef File, Position Pos) {
    signatureHelpResults = clangd::signatureHelp(File, Pos, *preambleData.get(), Inputs);
}

void ClangBridge::codeComplete(PathRef File, Position Pos, const clangd::CodeCompleteOptions &Opts) {
    codeCompleteResults = clangd::codeComplete(File, Pos, PData, Inputs, Opts);
}

void ClangBridge::locateSymbolAt(PathRef File, Position Pos) {
    locatedDefDecl = clangd::locateSymbolAt(parsedAST.getValue(), Pos, Inputs.Index); // add index
}

void ClangBridge::formatCode(PathRef File, llvm::StringRef Code, ArrayRef<tooling::Range> Ranges) {

    format::FormatStyle Style = getFormatStyleForFile(File, Code, TFS);
    // for whole file
    // {tooling::Range(0, Code.size())}
    tooling::Replacements IncludeReplaces =
        format::sortIncludes(Style, Code, Ranges, File);
    auto Changed = tooling::applyAllReplacements(Code, IncludeReplaces);
    if (!Changed)
        std::cout << "formation of file failed";
      //std::cout << Changed.takeError();

    format::reformat(
            Style, *Changed,
            tooling::calculateRangesAfterReplacements(IncludeReplaces, Ranges),
                File);

}

void ClangBridge::findHover(PathRef File, Position Pos) {
    format::FormatStyle Style = getFormatStyleForFile(
        File, Inputs.Contents, *Inputs.TFS);
    hoverInfo = clangd::getHover(parsedAST.getValue(), Pos, std::move(Style), Inputs.Index);
}

void ClangBridge::typeHierarchy(PathRef File, Position Pos, int Resolve, TypeHierarchyDirection Direction) {
    // TypeHierarchyDirection::Parents;
    TypeHierarchy = clangd::getTypeHierarchy(parsedAST.getValue(), Pos, Resolve, Direction, Inputs.Index,
                                             File);
}

void ClangBridge::resolveTypeHierarchy(TypeHierarchyItem Item, int Resolve, TypeHierarchyDirection Direction) {
    clangd::resolveTypeHierarchy(Item, Resolve, Direction, Inputs.Index);
}

void ClangBridge::foldingRanges(StringRef File) {
    foldRanges = clangd::getFoldingRanges(parsedAST.getValue()).get();
}

void ClangBridge::findReferences(PathRef File, Position Pos, uint32_t Limit) {
    refResults = clangd::findReferences(parsedAST.getValue(), Pos, Limit, Inputs.Index);
}

void ClangBridge::semanticHighlights(PathRef File) {
    highlightingTokens = clangd::getSemanticHighlightings(parsedAST.getValue());
}

void ClangBridge::rename(PathRef File, Position Pos, llvm::StringRef NewName, const RenameOptions &Opts) {
    auto Edits = clangd::rename(
        {Pos, NewName, parsedAST.getValue(), File, Inputs.Index, Opts, nullptr});
    if (!Edits)
      std::cout << "cannot rename !";

    if (Opts.WantFormat) {
      auto Style = getFormatStyleForFile(File, Inputs.Contents,
                                         *Inputs.TFS);
      llvm::Error Err = llvm::Error::success();
      for (auto &E : *Edits)
        Err =
            llvm::joinErrors(reformatEdit(E.getValue(), Style), std::move(Err));

    //if (Err)
    //    return CB(std::move(Err));
    }
    //RenameFiles.record(Edits->size());

}

*/
