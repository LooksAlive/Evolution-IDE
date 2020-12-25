cmake_minimum_required(VERSION 3.5.1)
project(Evolution-IDE)

set(CMAKE_CXX_STANDARD 11)

#add_subdirectory(Widgets)

#include_directories(Widgets)

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
                mainwindow.ui
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
                Widgets/ConsoleDock/findreplace.cpp
                Widgets/ConsoleDock/findreplace.h
                Widgets/BinaryInfo/binaryview.cpp
                Widgets/BinaryInfo/binaryview.h
                ${HEX_VIEW}
                Widgets/HexView/hexview.cpp
                Widgets/HexView/hexview.h
                Widgets/Converter/converter.cpp
                Widgets/Converter/converter.h

                Debugger/lldbbridge.cpp
                Debugger/lldbbridge.h
                Debugger/debuggerdock.cpp
                Debugger/debuggerdock.h
                Clang/indexer.cpp
                Clang/indexer.h
                Clang/parser.cpp
                Clang/parser.h
                llvm/llvmbridge.cpp
                llvm/llvmbridge.h

                commandlineexecutor.cpp
                commandlineexecutor.h
                Widgets/Settings/CmakeGenerator/cmakegenerator.cpp
                Widgets/Settings/CmakeGenerator/cmakegenerator.h
                Widgets/Settings/CmakeGenerator/cmagegeneratorwidget.cpp
                Widgets/Settings/CmakeGenerator/cmagegeneratorwidget.h
                Widgets/Settings/git/gitbridge.cpp
                Widgets/Settings/git/gitbridge.h
                Widgets/Settings/git/gitwidget.cpp
                Widgets/Settings/git/gitwidget.h
                )

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
)

add_executable(editor ${SOURCE_FILES} resources.qrc)



find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5Xml REQUIRED )
find_package( Qt5PrintSupport REQUIRED )

qt5_use_modules( editor Core Widgets Gui Xml PrintSupport)
