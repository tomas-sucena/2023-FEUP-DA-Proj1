cmake_minimum_required(VERSION 3.23)
project(DA_Trains)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS
        graph/src/DGraph.h
        graph/src/implementation/Graph.h
        src/network/Station.hpp
        src/network/Ride.hpp
        src/Reader.h )

set(PROJECT_SOURCES
        graph/src/DGraph.cpp
        graph/src/implementation/Graph.cpp
        src/main.cpp
        src/Reader.cpp)

add_executable(DA_Trains
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})

add_subdirectory(graph)

include_directories(graph/src)
