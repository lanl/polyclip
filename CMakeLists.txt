cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(singlecell)
add_executable(noglobal)

target_sources(singlecell PUBLIC SingleCell.cpp)
target_sources(noglobal PUBLIC NoGlobalVars.cpp)