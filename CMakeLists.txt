cmake_minimum_required(VERSION 3.14)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(untitled1 main.cpp)