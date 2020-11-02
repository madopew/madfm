cmake_minimum_required(VERSION 3.17)
project(madfm)

set(CMAKE_CXX_STANDARD 17)

add_executable(madfm main.cpp headers/ConsoleGuiHandler.h headers/Filedirectory.h headers/KeypressHandler.h src/ConsoleGuiHandler.cpp src/Filedirectory.cpp src/KeypressHandler.cpp)