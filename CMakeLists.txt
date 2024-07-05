cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(singlecell)
add_executable(clippingfull)
add_executable(clippinghalf)
add_executable(polygon)
add_executable(sort)

target_sources(singlecell PUBLIC SingleCell.cpp)
target_sources(clippingfull PUBLIC SingleCellClipping.cpp)
target_sources(clippinghalf PUBLIC SingleCellClippingHalf.cpp)
target_sources(polygon PUBLIC PointInPolygon.cpp)
target_sources(sort PUBLIC Sorting.cpp)