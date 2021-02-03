cmake_minimum_required(VERSION 3.17)
project(Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Code
        algorithms.h
        load.cpp
        load.h
        main.cpp
        output.cpp
        output.h
        tsp.cpp
        tsp.h)
