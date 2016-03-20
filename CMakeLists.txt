cmake_minimum_required(VERSION 3.4)
project(MOWNIT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES problem.cpp problem.h mz_axb.h mz_axb.cpp main.cpp)

add_executable(MOWNIT ${SOURCE_FILES})