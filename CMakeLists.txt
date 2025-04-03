cmake_minimum_required(VERSION 3.13)

project(thesis CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_CLANG_FORMAT "Use clang-format for formatting" OFF)
option(BUILD_TESTS "Build tests" ON)

# core
find_package(Kokkos REQUIRED)
add_library(polyclip core/mesh.cpp core/clipped_part.cpp gmv/io.cpp)
target_include_directories(polyclip PUBLIC core gmv)
target_link_libraries(polyclip PUBLIC Kokkos::kokkos)

# tests
if (BUILD_TESTS)
  enable_testing()

  add_executable(test_clip_grid_diagonal test/test_grid_diagonal.cpp)
  add_executable(test_clip_grid_horizontal test/test_grid_horizontal.cpp)
  add_executable(test_clip_poly_arbitrary test/test_poly_arbitrary.cpp)
  add_executable(test_clip_poly_legacy test/test_poly_legacy.cpp)
  add_executable(test_gmv_writer test/test_write.cpp)
  add_executable(test_gmv_reader test/test_read.cpp)
  add_executable(test_gmv_read_and_write test/test_read_and_write.cpp)

  target_link_libraries(test_clip_grid_diagonal PUBLIC polyclip)
  target_link_libraries(test_clip_grid_horizontal PUBLIC polyclip)
  target_link_libraries(test_clip_poly_arbitrary PUBLIC polyclip)
  target_link_libraries(test_clip_poly_legacy PUBLIC polyclip)
  target_link_libraries(test_gmv_writer PUBLIC polyclip)
  target_link_libraries(test_gmv_reader PUBLIC polyclip)
  target_link_libraries(test_gmv_read_and_write PUBLIC polyclip)

  add_test(NAME test_clip_grid_diagonal COMMAND test_clip_grid_diagonal)
  add_test(NAME test_clip_grid_horizontal COMMAND test_clip_grid_horizontal)
  add_test(NAME test_clip_poly_arbitrary COMMAND test_clip_poly_arbitrary)
  add_test(NAME test_clip_poly_legacy COMMAND test_clip_poly_legacy)
  add_test(NAME test_gmv_writer COMMAND test_gmv_writer)
  add_test(NAME test_gmv_reader COMMAND test_gmv_reader)
  add_test(NAME test_gmv_read_and_write COMMAND test_gmv_read_and_write)
endif ()

if (USE_CLANG_FORMAT)
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
  include(ClangFormat)
  file(GLOB_RECURSE ALL_SOURCE_FILES
    CONFIGURE_DEPENDS "core/*.h" "core/*.cpp" "test/*.h" "test/*.cpp" "gmv/*.h" "gmv/*.cpp")
  clangformat_setup(${ALL_SOURCE_FILES})
endif()