cmake_minimum_required(VERSION 3.23)
project(DA_Trains)

set(CMAKE_CXX_STANDARD 14)

add_executable(DA_Trains
        src/main.cpp
        src/Reader.cpp
        src/Reader.h)

add_subdirectory(graph)
