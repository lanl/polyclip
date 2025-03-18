cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(USE_KOKKOS_GPU "Run on Ndvidia's GPU using Kokkos" OFF)
option(USE_KOKKOS_OMP "Run with multiple threads using Kokkos" OFF)
option(USE_NATIVE_OMP "Run with multiple threads using OpenMP" OFF)

if (USE_KOKKOS_GPU)
  add_executable(clip_gpu)
  target_include_directories(clip_gpu PUBLIC test)
  target_sources(clip_gpu PUBLIC  test/structured_main_gpu.cpp  test/mesh_gpu.cpp test/clipped_part_gpu.cpp)

  # link with Kokkos
  find_package(Kokkos REQUIRED)
  target_link_libraries(clip_gpu PUBLIC Kokkos::kokkos)
else()
  if (USE_KOKKOS_OMP)
    add_executable(clip_kokkos)
    target_include_directories(clip_kokkos PUBLIC kokkos)
    target_sources(clip_kokkos PUBLIC
      kokkos/main_kokkos.cpp
      kokkos/geometry.cpp
      kokkos/clippings_kokkos.cpp
      kokkos/mesh_kokkos.cpp
      kokkos/intersect_kokkos.cpp)

    # link with Kokkos
    find_package(Kokkos REQUIRED)
    target_link_libraries(clip_kokkos PUBLIC Kokkos::kokkos)
  endif()
  if (USE_NATIVE_OMP)
    add_executable(allclips)
    target_include_directories(allclips PUBLIC omp)
    target_sources(allclips PUBLIC
      omp/main.cpp
      omp/clippings.cpp
      omp/mesh.cpp
      omp/geometry.cpp
      omp/intersect.cpp)

    # link with OpenMP
    find_package(OpenMP REQUIRED)
    target_link_libraries(allclips PUBLIC OpenMP::OpenMP_CXX)
  endif ()
 endif ()


add_executable(gmv_writer)
target_include_directories(gmv_writer PUBLIC gmv)
target_sources(gmv_writer PUBLIC gmv/test_write.cpp gmv/io.cpp test/mesh_gpu.cpp test/clipped_part_gpu.cpp)
find_package(Kokkos REQUIRED)
target_link_libraries(gmv_writer PUBLIC Kokkos::kokkos)


