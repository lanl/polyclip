cmake_minimum_required(VERSION 3.13)

project(polyclip CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FORMAT_CODE "Use clang-format for formatting" OFF)
option(BUILD_TESTS "Build tests" ON)

# core
find_package(Kokkos REQUIRED)
add_library(polyclip core/mesh.cpp core/clipped_part.cpp gmv/io.cpp)
target_include_directories(polyclip PUBLIC core gmv)
target_link_libraries(polyclip PUBLIC Kokkos::kokkos)

# tests
if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif ()

if (FORMAT_CODE)
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
  include(ClangFormat)
  file(GLOB_RECURSE ALL_SOURCE_FILES
    CONFIGURE_DEPENDS "core/*.h" "core/*.cpp" "test/*.h" "test/*.cpp" "gmv/*.h" "gmv/*.cpp")
  clangformat_setup(${ALL_SOURCE_FILES})
endif()