cmake_minimum_required(VERSION 3.7)
project(standard_library)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(standard_library ${SOURCE_FILES})