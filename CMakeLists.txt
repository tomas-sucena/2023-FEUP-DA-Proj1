cmake_minimum_required(VERSION 3.23)
project(DA_Trains)

set(CMAKE_CXX_STANDARD 14)

add_executable(DA_Trains
        src/main.cpp
        src/Reader.cpp
        src/Reader.h src/network/Station.h)

add_subdirectory(graph)

include_directories(graph/src)
