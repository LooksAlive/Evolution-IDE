cmake_minimum_required(VERSION 3.5.1)
project(Evolution-IDE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++-10)
set(BASE_FLAGS "-O0 -g -Wall") # -O3

# required flags for clang, llvm, lldb
# -frtti for SMTP # -fno-rtti
set(CXX_FLAGS
        " ${BASE_FLAGS} -frtti -fno-exceptions -D_GNU_SOURCE -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_FLAGS ${CXX_FLAGS})

#set(CMAKE_CXX_FLAGS "-O0 -g -std=c++17 -fsanitize=address -fno-omit-frame-pointer")

#add_subdirectory(Widgets)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        /usr/local/include         # llvm, lldb, clang
        /usr/local/include/clang-tidy
        /usr/local/include/clangd  # clangd -> indexer, completer, parser etc.
        /usr/local/include/clangd/support
        Widgets/Notifier/simple-mail/src        # SMTP
        #clangd
        #clangd/support
)

set(CLANGD_SOURCES
        clangd/AST.cpp
        #clangd/ClangdLSPServer.cpp
        #clangd/ClangdServer.cpp
        clangd/CodeComplete.cpp
        clangd/CodeCompletionStrings.cpp
        clangd/CollectMacros.cpp
        clangd/CompileCommands.cpp
        clangd/Compiler.cpp
        clangd/Config.cpp
        clangd/ConfigCompile.cpp
        clangd/ConfigProvider.cpp
        clangd/ConfigYAML.cpp
        clangd/Diagnostics.cpp
        clangd/DraftStore.cpp
        clangd/ExpectedTypes.cpp
        clangd/FindSymbols.cpp
        clangd/FindTarget.cpp
        clangd/FileDistance.cpp
        clangd/Format.cpp
        clangd/FS.cpp
        clangd/FuzzyMatch.cpp
        clangd/GlobalCompilationDatabase.cpp
        clangd/Headers.cpp
        clangd/HeaderSourceSwitch.cpp
        clangd/Hover.cpp
        clangd/IncludeFixer.cpp
        clangd/JSONTransport.cpp
        clangd/PathMapping.cpp
        clangd/Protocol.cpp
        clangd/Quality.cpp
        clangd/ParsedAST.cpp
        clangd/Preamble.cpp
        clangd/RIFF.cpp
        clangd/Selection.cpp
        clangd/SemanticHighlighting.cpp
        clangd/SemanticSelection.cpp
        clangd/SourceCode.cpp
        clangd/QueryDriverDatabase.cpp
        clangd/TUScheduler.cpp
        clangd/URI.cpp
        clangd/XRefs.cpp

        clangd/CompletionModel.cpp

        clangd/index/Background.cpp
        clangd/index/BackgroundIndexLoader.cpp
        clangd/index/BackgroundIndexStorage.cpp
        clangd/index/BackgroundQueue.cpp
        clangd/index/BackgroundRebuild.cpp
        clangd/index/CanonicalIncludes.cpp
        clangd/index/FileIndex.cpp
        clangd/index/Index.cpp
        clangd/index/IndexAction.cpp
        clangd/index/MemIndex.cpp
        clangd/index/Merge.cpp
        clangd/index/Ref.cpp
        clangd/index/Relation.cpp
        clangd/index/Serialization.cpp
        clangd/index/Symbol.cpp
        clangd/index/SymbolCollector.cpp
        clangd/index/SymbolID.cpp
        clangd/index/SymbolLocation.cpp
        clangd/index/SymbolOrigin.cpp
        clangd/index/YAMLSerialization.cpp

        clangd/index/dex/Dex.cpp
        clangd/index/dex/Iterator.cpp
        clangd/index/dex/PostingList.cpp
        clangd/index/dex/Trigram.cpp

        clangd/refactor/Rename.cpp
        clangd/refactor/Tweak.cpp

        clangd/support/Cancellation.cpp
        clangd/support/Function.h
        clangd/support/Markup.h
        clangd/support/Threading.cpp
        clangd/support/Trace.cpp
        clangd/support/Cancellation.h
        clangd/support/Logger.cpp
        clangd/support/Path.h
        clangd/support/Threading.h
        clangd/support/Trace.h
        clangd/support/Context.cpp
        clangd/support/Logger.h
        clangd/support/Shutdown.cpp
        clangd/support/ThreadsafeFS.cpp
        clangd/support/Context.h
        clangd/support/Markup.cpp
        clangd/support/Shutdown.h
        clangd/support/ThreadsafeFS.h
        )

set(TASK_VIEW
        TaskWidget/processmodel.h
        TaskWidget/readprocessstrategy.h
        TaskWidget/processdescriptor.cpp
        TaskWidget/processproxymodel.cpp
        TaskWidget/taskwidget.cpp
        TaskWidget/processdescriptor.h
        TaskWidget/processproxymodel.h
        TaskWidget/taskwidget.h
        TaskWidget/processfilterwidget.cpp
        TaskWidget/readlinuxprocessesstrategy.cpp
        TaskWidget/taskwidgetimages.qrc
        TaskWidget/processfilterwidget.h
        TaskWidget/readlinuxprocessesstrategy.h
        TaskWidget/processmodel.cpp
        TaskWidget/readprocessstrategy.cpp
        )

set(SMTP_MAIL
        Widgets/Notifier/simple-mail/src/emailaddress.h
        Widgets/Notifier/simple-mail/src/emailaddress.cpp
        Widgets/Notifier/simple-mail/src/mimeattachment.cpp
        Widgets/Notifier/simple-mail/src/mimeattachment.h
        Widgets/Notifier/simple-mail/src/mimecontentformatter.cpp
        Widgets/Notifier/simple-mail/src/mimecontentformatter.h
        Widgets/Notifier/simple-mail/src/mimefile.cpp
        Widgets/Notifier/simple-mail/src/mimefile.h
        Widgets/Notifier/simple-mail/src/mimehtml.h
        Widgets/Notifier/simple-mail/src/mimehtml.cpp
        Widgets/Notifier/simple-mail/src/mimeinlinefile.cpp
        Widgets/Notifier/simple-mail/src/mimeinlinefile.h
        Widgets/Notifier/simple-mail/src/emailaddress.cpp
        Widgets/Notifier/simple-mail/src/emailaddress.h
        Widgets/Notifier/simple-mail/src/mimeattachment.h
        Widgets/Notifier/simple-mail/src/mimeattachment.cpp
        Widgets/Notifier/simple-mail/src/mimemessage.cpp
        Widgets/Notifier/simple-mail/src/mimemessage.h
        Widgets/Notifier/simple-mail/src/mimemultipart.cpp
        Widgets/Notifier/simple-mail/src/mimemultipart.h
        Widgets/Notifier/simple-mail/src/mimepart.cpp
        Widgets/Notifier/simple-mail/src/mimepart.h
        Widgets/Notifier/simple-mail/src/mimetext.cpp
        Widgets/Notifier/simple-mail/src/mimetext.h
        Widgets/Notifier/simple-mail/src/quotedprintable.cpp
        Widgets/Notifier/simple-mail/src/quotedprintable.h
        Widgets/Notifier/simple-mail/src/sender.cpp
        Widgets/Notifier/simple-mail/src/sender.h
        Widgets/Notifier/simple-mail/src/server.h
        Widgets/Notifier/simple-mail/src/server.cpp
        Widgets/Notifier/simple-mail/src/serverreply.cpp
        Widgets/Notifier/simple-mail/src/serverreply.h
        Widgets/Notifier/simple-mail/src/smtpexports.h
        #Widgets/Notifier/simple-mail/src/SimpleMail     # main file
        )

set(HEX_VIEW
        Widgets/HexView/QHexView/document/buffer/qfilebuffer.cpp
        Widgets/HexView/QHexView/document/buffer/qfilebuffer.h
        Widgets/HexView/QHexView/document/buffer/qhexbuffer.cpp
        Widgets/HexView/QHexView/document/buffer/qhexbuffer.h
        Widgets/HexView/QHexView/document/buffer/qmemorybuffer.cpp
        Widgets/HexView/QHexView/document/buffer/qmemorybuffer.h
        Widgets/HexView/QHexView/document/buffer/qmemoryrefbuffer.cpp
        Widgets/HexView/QHexView/document/buffer/qmemoryrefbuffer.h
        Widgets/HexView/QHexView/document/commands/hexcommand.cpp
        Widgets/HexView/QHexView/document/commands/hexcommand.h
        Widgets/HexView/QHexView/document/commands/insertcommand.cpp
        Widgets/HexView/QHexView/document/commands/insertcommand.h
        Widgets/HexView/QHexView/document/commands/removecommand.cpp
        Widgets/HexView/QHexView/document/commands/removecommand.h
        Widgets/HexView/QHexView/document/commands/replacecommand.cpp
        Widgets/HexView/QHexView/document/commands/replacecommand.h
        Widgets/HexView/QHexView/document/qhexcursor.cpp
        Widgets/HexView/QHexView/document/qhexcursor.h
        Widgets/HexView/QHexView/document/qhexdocument.cpp
        Widgets/HexView/QHexView/document/qhexdocument.h
        Widgets/HexView/QHexView/document/qhexmetadata.cpp
        Widgets/HexView/QHexView/document/qhexmetadata.h
        Widgets/HexView/QHexView/document/qhexrenderer.cpp
        Widgets/HexView/QHexView/document/qhexrenderer.h
        Widgets/HexView/QHexView/qhexview.cpp
        Widgets/HexView/QHexView/qhexview.h
    )

set(SOURCE_FILES
                main.cpp
                highlighter.cpp
                highlighter.h
                mainwindow.cpp
                mainwindow.h
                customtabstyle.h
                EnvironmentSettings.h
                DependencyInstaller.cpp
                DependencyInstaller.h
                mainwindow.ui

                Qt-Frameless-Window-DarkStyle/DarkStyle.h
                Qt-Frameless-Window-DarkStyle/DarkStyle.cpp

                Widgets/PlainTextEdit/plaintextedit.cpp
                Widgets/PlainTextEdit/plaintextedit.h
                Widgets/PlainTextEdit/completer.cpp
                Widgets/PlainTextEdit/completer.h
                Widgets/FileExplorer/fileexplorer.cpp
                Widgets/FileExplorer/fileexplorer.h
                Widgets/FileDock/filedock.cpp
                Widgets/FileDock/filedock.h
                Widgets/Tab/tab.cpp
                Widgets/Tab/tab.h
        Widgets/Settings/settingswindow.cpp
        Widgets/Settings/settingswindow.h
        Widgets/ConsoleDock/consoledock.cpp
        Widgets/ConsoleDock/consoledock.h
        Widgets/FindReplace/findreplace.cpp
        Widgets/FindReplace/findreplace.h
        Widgets/BinaryInfo/binaryview.cpp
        Widgets/BinaryInfo/binaryview.h
        qconsole.cpp
        qconsole.h
        ${HEX_VIEW}
        #${CLANGD_SOURCES} # in the future will be more accurate, now only takes much storage and time
        ${SMTP_MAIL}
        ${TASK_VIEW}
        Widgets/HexView/hexview.cpp
        Widgets/HexView/hexview.h
        Widgets/Converter/converter.cpp
        Widgets/Converter/converter.h

        #Debugger/lldbbridge.cpp
        #Debugger/lldbbridge.h
        Debugger/lldbBridge.cpp
        Debugger/lldbBridge.h
        Clang/indexer.cpp
        Clang/indexer.h
        Clang/ClangBridge.cpp
        Clang/ClangBridge.h
        llvm/llvmbridge.cpp
        llvm/llvmbridge.h

        commandlineexecutor.cpp
        commandlineexecutor.h
        filemanager.cpp
        filemanager.h
        Widgets/Settings/CmakeGenerator/cmakegenerator.cpp
        Widgets/Settings/CmakeGenerator/cmakegenerator.h
        Widgets/Settings/CmakeGenerator/cmakegeneratorwidget.cpp
        Widgets/Settings/CmakeGenerator/cmakegeneratorwidget.h
        Widgets/Git/gitbridge.cpp
        Widgets/Git/gitbridge.h
        Widgets/Settings/git/gitwidget.cpp
        Widgets/Settings/git/gitwidget.h
        icons/IconFactory.h
        Widgets/Settings/Appearence/AppearenceWidget.cpp
        Widgets/Settings/Appearence/AppearenceWidget.h
        Widgets/CodeInfoDock/CodeInfoDock.cpp
        Widgets/CodeInfoDock/CodeInfoDock.h
        Widgets/FileExplorer/NewWindow.cpp
        Widgets/FileExplorer/NewWindow.h
        Widgets/Settings/Debugger/DebuggerSettings.cpp
        Widgets/Settings/Debugger/DebuggerSettings.h
        Widgets/FileExplorer/SourceTemplates.h
        Widgets/NodeView/NodeView.cpp
        Widgets/NodeView/NodeView.h
        Widgets/PlainTextEdit/GoToLineColumn.cpp
        Widgets/PlainTextEdit/GoToLineColumn.h
        Widgets/ProgressBar/ProgressBar.cpp
        Widgets/ProgressBar/ProgressBar.h
        Widgets/Education/Education.cpp
        Widgets/Education/Education.h
        Widgets/Education/CodeSamples.h
        Widgets/Settings/ClangFormat/ClangFormat.cpp
        Widgets/Settings/ClangFormat/ClangFormat.h
        Widgets/Education/NewSampleWindow.cpp
        Widgets/Education/NewSampleWindow.h
        Widgets/Settings/Installer/Installer.cpp
        Widgets/Settings/Installer/Installer.h
        Debugger/DebuggerDock.cpp
        Debugger/DebuggerDock.h
        Debugger/DebugWatchDock.cpp
        Debugger/DebugWatchDock.h
        Widgets/SearchBox/SearchBox.cpp
        Widgets/SearchBox/SearchBox.h
        Delegate.cpp Delegate.h
        Widgets/CodeInfoDock/RefactoringDock.cpp
        Widgets/CodeInfoDock/RefactoringDock.h
        Widgets/CodeInfoDock/LinterDock.cpp
        Widgets/CodeInfoDock/LinterDock.h
        Widgets/PlainTextEdit/InvitationScreen.cpp
        Widgets/PlainTextEdit/InvitationScreen.h
        Widgets/Git/GitDock.cpp
        Widgets/Git/GitDock.h
        Widgets/Education/CodeSamples.cpp
        Widgets/Notifier/SmtpMailNotifier.cpp
        Widgets/Notifier/SmtpMailNotifier.h
        Widgets/Notifier/ContactDeveloper.cpp
        Widgets/Notifier/ContactDeveloper.h Widgets/PlainTextEdit/HoverInfo.cpp Widgets/PlainTextEdit/HoverInfo.h)




#set(Qt5Core_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Core/")
#set(Qt5Widgets_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Widgets/")
#set(Qt5Console_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Console/")


SET(CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# required libs by clang ...
set(SYSTEM_LIBS rt dl tinfo pthread m z)

set(LLDB_LIBS_SHARED lldb) # one shared lib

# clang libraries; also added manually:
# clangd has no shared libs, so if i want to use clang shared lib in the future and recompile clang by in this manner
# i would need to go deep into clangd and extract, what i need -> so yes later

set(CLANG_LIBS_SHARED clang)

set(CLANG_LIBS
        clangAnalysis clangApplyReplacements clangARCMigrate
        clangAST clangASTMatchers clangBasic
        clangCodeGen clangTidyLLVMLibcModule clangQuery
        clangToolingRefactoring clangToolingSyntax
        clangTransformer clangToolingInclusions clangTidy
        clangToolingCore clangToolingASTDiff clangTooling
        clangTidyZirconModule clangTidyUtils clangTidyMain
        clangTidyReadabilityModule clangTidyPortabilityModule
        clangTidyOpenMPModule clangTidyPerformanceModule
        clangTidyPlugin clangTidyObjCModule clangTidyMPIModule
        clangTidyModernizeModule clangTidyMiscModule
        clangStaticAnalyzerFrontend clangStaticAnalyzerCheckers
        clangTesting clangTidyLLVMModule clangParse
        clangTidyLinuxKernelModule clangTidyHICPPModule
        clangTidyGoogleModule clangTidyFuchsiaModule
        clangTidyDarwinModule clangTidyCppCoreGuidelinesModule
        clangTidyAbseilModule clangTidyAndroidModule
        clangTidyBoostModule clangTidyBugproneModule
        clangTidyCERTModule clangStaticAnalyzerCore
        clangReorderFields clangRewrite clangRewriteFrontend
        clangSema clangSerialization clangLex clangMove
        clangHandleCXX clangHandleLLVM clangChangeNamespace
        clangIncludeFixer clangIncludeFixerPlugin clangIndex
        clangFormat clangFrontend clangFrontendTool
        clangdSupport clangDynamicASTMatchers clangEdit
        clangDoc clangdRemoteIndex clangDriver
        clangDaemonTweaks clangDependencyScanning
        clangDirectoryWatcher clangCrossTU clangDaemon
        )

# LLVM libraries: `llvm-config -ibs $(LLVM_MODULES)`;LLVM_MODULES = all engine
set(LLVM_LIBS_SHARED    Polly
                        LTO
                        LLVM #llvm -> not working
                        )


set(LLVM_LIBS
        LLVMXRay LLVMWindowsManifest LLVMTableGen LLVMSymbolize
        LLVMDebugInfoPDB LLVMOrcJIT LLVMOrcError LLVMJITLink
        LLVMObjectYAML LLVMMCA LLVMLTO LLVMPasses LLVMCoroutines
        LLVMObjCARCOpts LLVMExtensions LLVMLineEditor LLVMLibDriver
        LLVMInterpreter LLVMInterfaceStub LLVMFuzzMutate LLVMMCJIT
        LLVMExecutionEngine LLVMRuntimeDyld LLVMDWARFLinker
        LLVMDlltoolDriver LLVMOption LLVMDebugInfoGSYM LLVMCoverage
        LLVMXCoreDisassembler LLVMXCoreCodeGen LLVMXCoreDesc LLVMXCoreInfo
        LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc
        LLVMX86Info LLVMWebAssemblyDisassembler LLVMWebAssemblyCodeGen
        LLVMWebAssemblyDesc LLVMWebAssemblyAsmParser LLVMWebAssemblyInfo
        LLVMSystemZDisassembler LLVMSystemZCodeGen LLVMSystemZAsmParser
        LLVMSystemZDesc LLVMSystemZInfo LLVMSparcDisassembler LLVMSparcCodeGen
        LLVMSparcAsmParser LLVMSparcDesc LLVMSparcInfo LLVMRISCVDisassembler
        LLVMRISCVCodeGen LLVMRISCVAsmParser LLVMRISCVDesc LLVMRISCVUtils
        LLVMRISCVInfo LLVMPowerPCDisassembler LLVMPowerPCCodeGen
        LLVMPowerPCAsmParser LLVMPowerPCDesc LLVMPowerPCInfo LLVMNVPTXCodeGen
        LLVMNVPTXDesc LLVMNVPTXInfo LLVMMSP430Disassembler LLVMMSP430CodeGen
        LLVMMSP430AsmParser LLVMMSP430Desc LLVMMSP430Info LLVMMipsDisassembler
        LLVMMipsCodeGen LLVMMipsAsmParser LLVMMipsDesc LLVMMipsInfo
        LLVMLanaiDisassembler LLVMLanaiCodeGen LLVMLanaiAsmParser LLVMLanaiDesc
        LLVMLanaiInfo LLVMHexagonDisassembler LLVMHexagonCodeGen
        LLVMHexagonAsmParser LLVMHexagonDesc LLVMHexagonInfo LLVMBPFDisassembler
        LLVMBPFCodeGen LLVMBPFAsmParser LLVMBPFDesc LLVMBPFInfo
        LLVMAVRDisassembler LLVMAVRCodeGen LLVMAVRAsmParser LLVMAVRDesc
        LLVMAVRInfo LLVMARMDisassembler LLVMARMCodeGen LLVMARMAsmParser
        LLVMARMDesc LLVMARMUtils LLVMARMInfo LLVMAMDGPUDisassembler
        LLVMAMDGPUCodeGen LLVMMIRParser LLVMipo LLVMInstrumentation
        LLVMVectorize LLVMLinker LLVMIRReader LLVMAsmParser LLVMFrontendOpenMP
        LLVMAMDGPUAsmParser LLVMAMDGPUDesc LLVMAMDGPUUtils LLVMAMDGPUInfo
        LLVMAArch64Disassembler LLVMMCDisassembler LLVMAArch64CodeGen
        LLVMCFGuard LLVMGlobalISel LLVMSelectionDAG LLVMAsmPrinter
        LLVMDebugInfoDWARF LLVMCodeGen LLVMTarget LLVMScalarOpts
        LLVMInstCombine LLVMAggressiveInstCombine LLVMTransformUtils
        LLVMBitWriter LLVMAnalysis LLVMProfileData LLVMObject LLVMTextAPI
        LLVMBitReader LLVMCore LLVMRemarks LLVMBitstreamReader
        LLVMAArch64AsmParser LLVMMCParser LLVMAArch64Desc LLVMMC
        LLVMDebugInfoCodeView LLVMDebugInfoMSF LLVMBinaryFormat
        LLVMAArch64Utils LLVMAArch64Info LLVMSupport LLVMDemangle

        )

set(GIT_LIBS git2)

link_directories(/usr/local/lib)

add_executable(editor ${SOURCE_FILES} resources.qrc)

# marked as comment -> not needed ...
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package( Qt5 REQUIRED
        Core
        Widgets
        Gui
        Xml
        PrintSupport
        Network
        )
#find_package( Qt5Widgets REQUIRED )
#find_package( Qt5Gui REQUIRED )
#find_package( Qt5Xml REQUIRED )
#find_package( Qt5PrintSupport REQUIRED )

#qt5_use_modules( editor Core Widgets Gui Xml PrintSupport)

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

target_link_libraries(editor -Wl,--start-group Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Network Qt5::Xml Qt5::PrintSupport
        ${SYSTEM_LIBS} ${LLVM_LIBS} ${CLANG_LIBS} ${LLDB_LIBS_SHARED} ${GIT_LIBS} -Wl,--end-group)
