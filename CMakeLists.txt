cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(USE_KOKKOS_GPU "Run on Ndvidia's GPU using Kokkos" OFF)

if (USE_KOKKOS_GPU)
  add_executable(clip_gpu)
  target_include_directories(clip_gpu PUBLIC test core)
  target_sources(clip_gpu PUBLIC  test/test_poly_legacy.cpp  core/mesh_gpu.cpp core/clipped_part_gpu.cpp)

  # link with Kokkos
  find_package(Kokkos REQUIRED)
  target_link_libraries(clip_gpu PUBLIC Kokkos::kokkos)
endif ()


add_executable(gmv_writer)
target_include_directories(gmv_writer PUBLIC gmv)
target_sources(gmv_writer PUBLIC gmv/test_write.cpp gmv/io.cpp core/mesh_gpu.cpp core/clipped_part_gpu.cpp)
find_package(Kokkos REQUIRED)
target_link_libraries(gmv_writer PUBLIC Kokkos::kokkos)



