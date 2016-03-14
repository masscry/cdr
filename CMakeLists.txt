cmake_minimum_required(VERSION 3.3)
project(cdr-project)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

include_directories(contrib/cutest)
include_directories(include)

add_subdirectory(contrib/cutest)

add_subdirectory(src)