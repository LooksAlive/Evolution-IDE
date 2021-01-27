cmake_minimum_required(VERSION 3.5.1)
project(Evolution-IDE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++-10)
set(BASE_FLAGS "-O0 -g -Wall") # -O3

# required flags for clang, llvm, lldb
set(CXX_FLAGS
        " ${BASE_FLAGS} -fno-exceptions -fno-rtti -D_GNU_SOURCE -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS")
set(CMAKE_CXX_FLAGS ${CXX_FLAGS})

#set(CMAKE_CXX_FLAGS "-O0 -g -std=c++17 -fsanitize=address -fno-omit-frame-pointer")

#add_subdirectory(Widgets)

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

set(SOURCE_FILES main.cpp
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
                ${HEX_VIEW}
                Widgets/HexView/hexview.cpp
                Widgets/HexView/hexview.h
                Widgets/Converter/converter.cpp
                Widgets/Converter/converter.h

                Debugger/lldbbridge.cpp
                Debugger/lldbbridge.h
                Debugger/debuggerwidget.cpp
                Debugger/debuggerwidget.h
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
                Widgets/Settings/git/gitbridge.cpp
                Widgets/Settings/git/gitbridge.h
                Widgets/Settings/git/gitwidget.cpp
                Widgets/Settings/git/gitwidget.h
        icons/IconFactory.h
                Widgets/Settings/Appearence/AppearenceWidget.cpp
                Widgets/Settings/Appearence/AppearenceWidget.h
                Widgets/CodeInfoDock/CodeInfoDock.cpp
                Widgets/CodeInfoDock/CodeInfoDock.h
                Widgets/FileExplorer/NewWindow.cpp
                Widgets/FileExplorer/NewWindow.h Widgets/Settings/Debugger/DebuggerSettings.cpp Widgets/Settings/Debugger/DebuggerSettings.h Widgets/FileExplorer/SourceTemplates.h Widgets/NodeView/NodeView.cpp Widgets/NodeView/NodeView.h Widgets/PlainTextEdit/GoToLineColumn.cpp Widgets/PlainTextEdit/GoToLineColumn.h Widgets/ProgressBar/ProgressBar.cpp Widgets/ProgressBar/ProgressBar.h)




#set(Qt5Core_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Core/")
#set(Qt5Widgets_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Widgets/")
#set(Qt5Console_DIR "/opt/Qt/5.9.3/gcc_64/lib/cmake/Qt5Console/")


SET(CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        /usr/local/include         # llvm, lldb, clang
        /usr/local/include/clangd  # clangd -> indexer, completer, parser etc.
        /usr/local/include/clangd/support
)

# required libs by clang ...
set(SYSTEM_LIBS rt dl tinfo pthread m z)

set(LLDB_LIBS_SHARED lldb) # one shared lib

# clang libraries; also added manually:
# clangd has no shared libs, so if i want to use clang shared lib in the future and recompile clang by in this manner
# i would need to go deep into clangd and extract, what i need -> so yes later

set(CLANG_LIBS_SHARED   clang
                        #clang-cpp  # -> does not work combined with clang.
        )

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
                        LLVM #llvm -> in /usr/lib/llvm11
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


link_directories(/usr/local/lib)
#link_directories(/usr/lib/llvm-11)


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
        )
#find_package( Qt5Widgets REQUIRED )
#find_package( Qt5Gui REQUIRED )
#find_package( Qt5Xml REQUIRED )
#find_package( Qt5PrintSupport REQUIRED )

#qt5_use_modules( editor Core Widgets Gui Xml PrintSupport)

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

target_link_libraries(editor Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Xml Qt5::PrintSupport
        -Wl,--start-group ${SYSTEM_LIBS} ${LLVM_LIBS_SHARED} ${CLANG_LIBS_SHARED} ${LLDB_LIBS_SHARED} -Wl,--end-group)
