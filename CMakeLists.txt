cmake_minimum_required(VERSION 3.14)
project(parallel_matrix_multiplication)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O -fopenmp")

include_directories(common)
include_directories(scripts)

add_executable(parallel_matrix_multiplication scripts/benchmark.cc)
