cmake_minimum_required(VERSION 3.5.1)
project(Evolution-IDE)

set(CMAKE_CXX_STANDARD 11)

#add_subdirectory(Widgets)

#include_directories(Widgets)

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
                Widgets/BinaryInfo/binaryinfo.cpp
                Widgets/BinaryInfo/binaryinfo.h
                #Widgets/HexView/hexview.cpp
                #Widgets/HexView/hexview.h
                #Widgets/Converter/converter.cpp
                #WWidgets/Converter/converter.h
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
