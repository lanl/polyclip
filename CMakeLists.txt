cmake_minimum_required(VERSION 3.13)

project(internship CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(allclips)
target_include_directories(allclips PUBLIC .)
target_sources(allclips PUBLIC main.cpp clippings.cpp mesh.cpp geometry.cpp intersect.cpp)

add_executable(clip_kokkos)
target_include_directories(clip_kokkos PUBLIC .)
target_sources(clip_kokkos PUBLIC main_kokkos.cpp clippings.cpp mesh.cpp geometry.cpp intersect_kokkos.cpp clippings_kokkos.cpp mesh_kokkos.cpp intersect_kokkos.cpp )

# link with OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(allclips PUBLIC OpenMP::OpenMP_CXX)

# link with kokkos
find_package(Kokkos REQUIRED)
target_link_libraries(allclips PUBLIC Kokkos::kokkos)

# link with OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(clip_kokkos PUBLIC OpenMP::OpenMP_CXX)

# link with kokkos
find_package(Kokkos REQUIRED)
target_link_libraries(clip_kokkos PUBLIC Kokkos::kokkos)
