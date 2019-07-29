cmake_minimum_required(VERSION 3.14)
project(mm)

set(CMAKE_CXX_STANDARD 14)

include_directories(common)

add_executable(mm main.cpp)