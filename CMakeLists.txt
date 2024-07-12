cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(allclips)
target_include_directories(allclips PUBLIC .)
target_sources(allclips PUBLIC main.cpp Clippings.cpp)