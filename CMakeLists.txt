cmake_minimum_required(VERSION 3.17)
project(madfm)

set(CMAKE_CXX_STANDARD 17)

add_executable(madfm
        main.cpp
        headers/gui/ConsoleGuiHandler.h
        headers/files/Filedirectory.h
        headers/KeypressHandler.h
        src/gui/ConsoleGuiHandler.cpp
        src/files/Filedirectory.cpp
        src/KeypressHandler.cpp
        headers/files/File.h
        src/files/File.cpp
        headers/files/FileDirectoryUtils.h
        src/files/FileDirectoryUtils.cpp
        headers/gui/ConsoleLine.h
        src/gui/ConsoleLine.cpp
        headers/gui/ConsoleGuiUtils.h
        src/gui/ConsoleGuiUtils.cpp
        headers/gui/modules/MenuDrawer.h
        src/gui/modules/MenuDrawer.cpp
        headers/gui/modules/HelpDrawer.h
        src/gui/modules/HelpDrawer.cpp
        headers/gui/modules/FileCreator.h
        src/gui/modules/FileCreator.cpp
        headers/gui/modules/FileRenamer.h
        src/gui/modules/FileRenamer.cpp
        headers/gui/modules/FileDeleter.h
        src/gui/modules/FileDeleter.cpp
        headers/gui/modules/FilePreviewer.h
        src/gui/modules/FilePreviewer.cpp
        headers/gui/modules/FileMover.h src/gui/modules/FileMover.cpp)