cmake_minimum_required(VERSION 3.13)

project(thesis CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(USE_KOKKOS_GPU "Run on Ndvidia's GPU using Kokkos" OFF)

if (USE_KOKKOS_GPU)
  add_executable(clip_grid_diagonal)
  target_include_directories(clip_grid_diagonal PUBLIC test core)
  target_sources(clip_grid_diagonal PUBLIC  test/test_grid_diagonal.cpp  core/mesh.cpp core/clipped_part.cpp)

  add_executable(clip_grid_horizontal)
  target_include_directories(clip_grid_horizontal PUBLIC test core) 
  target_sources(clip_grid_horizontal PUBLIC  test/test_grid_horizontal.cpp  core/mesh.cpp core/clipped_part.cpp)

  add_executable(clip_poly_arbitrary) 
  target_include_directories(clip_poly_arbitrary PUBLIC test core)
  target_sources(clip_poly_arbitrary PUBLIC  test/test_poly_arbitrary.cpp  core/mesh.cpp core/clipped_part.cpp)

  add_executable(clip_poly_legacy)
  target_include_directories(clip_poly_legacy PUBLIC test core)
  target_sources(clip_poly_legacy PUBLIC  test/test_poly_legacy.cpp  core/mesh.cpp core/clipped_part.cpp)

  # link with Kokkos
  find_package(Kokkos REQUIRED)
  target_link_libraries(clip_grid_diagonal PUBLIC Kokkos::kokkos)
  target_link_libraries(clip_grid_horizontal PUBLIC Kokkos::kokkos)
  target_link_libraries(clip_poly_arbitrary PUBLIC Kokkos::kokkos)
  target_link_libraries(clip_poly_legacy PUBLIC Kokkos::kokkos)
endif ()

add_executable(gmv_writer)
target_include_directories(gmv_writer PUBLIC gmv)
target_sources(gmv_writer PUBLIC gmv/test_write.cpp gmv/io.cpp core/mesh.cpp core/clipped_part.cpp)
find_package(Kokkos REQUIRED)
target_link_libraries(gmv_writer PUBLIC Kokkos::kokkos)

add_executable(gmv_reader)
target_include_directories(gmv_reader PUBLIC gmv)
target_sources(gmv_reader PUBLIC gmv/test_read.cpp gmv/io.cpp test/mesh_gpu.cpp test/clipped_part_gpu.cpp)
find_package(Kokkos REQUIRED)
target_link_libraries(gmv_reader PUBLIC Kokkos::kokkos)

add_executable(gmv_read_and_write)
target_include_directories(gmv_reader PUBLIC gmv)
target_sources(gmv_read_and_write PUBLIC gmv/test_read_and_write.cpp gmv/io.cpp test/mesh_gpu.cpp test/clipped_part_gpu.cpp)
find_package(Kokkos REQUIRED)
target_link_libraries(gmv_read_and_write PUBLIC Kokkos::kokkos)

