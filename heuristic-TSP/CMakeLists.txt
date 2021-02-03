cmake_minimum_required(VERSION 3.17)
project(lab_4_heuristic_search_ABrkich)

set(CMAKE_CXX_STANDARD 14)

include_directories(Code)

add_executable(lab_4_heuristic_search_ABrkich
        Code/algorithms.h
        Code/load.cpp
        Code/load.h
        Code/main.cpp
        Code/output.cpp
        Code/output.h
        Code/tsp.cpp
        Code/tsp.h)
