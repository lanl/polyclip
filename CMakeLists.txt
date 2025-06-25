# (c) 2025. Triad National Security, LLC. All rights reserved.
# This program was produced under U.S. Government contract 89233218CNA000001
# for Los Alamos National Laboratory (LANL), which is operated by Triad National
# Security, LLC for the U.S. Department of Energy/National Nuclear Security
# Administration. All rights in the program are reserved by Triad National
# Security, LLC, and the U.S. Department of Energy/National Nuclear Security
# Administration. The Government is granted for itself and others acting on its
# behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
# to reproduce, prepare derivative works, distribute copies to the public,
# perform publicly and display publicly, and to permit others to do so.

cmake_minimum_required(VERSION 3.13)

project(polyclip CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FORMAT_CODE "Use clang-format for formatting" OFF)
option(BUILD_TESTS "Build tests" ON)

# kokkos
add_compile_options(-Wno-deprecated-declarations)
add_subdirectory(external/kokkos)


# core
add_library(polyclip core/mesh.cpp core/clipped_part.cpp gmv/io.cpp)
target_include_directories(polyclip PUBLIC core gmv)
target_link_libraries(polyclip PUBLIC Kokkos::kokkos)

# tests
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
if (BUILD_TESTS)
  set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
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