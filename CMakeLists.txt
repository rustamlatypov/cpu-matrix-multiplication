cmake_minimum_required(VERSION 3.14)
project(parallel-matrix-multiplication)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(common)

add_executable(parallel-matrix-multiplication main.cpp)