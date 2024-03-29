cmake_minimum_required(VERSION 3.16.3)
project(DA_Trains)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS
        graph/src/implementation/Graph.h
        graph/src/UGraph.h
        src/Helpy.h
        src/libfort/fort.hpp
        src/network/RailGraph.h
        src/network/Railway.hpp
        src/network/Station.hpp
        src/Reader.h
        src/Utils.hpp)

set(PROJECT_SOURCES
        graph/src/implementation/Graph.cpp
        graph/src/UGraph.cpp
        src/Helpy.cpp
        src/libfort/fort.c
        src/main.cpp
        src/network/RailGraph.cpp
        src/Reader.cpp)

add_executable(DA_Trains
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})

add_subdirectory(graph)

include_directories(graph/src)
