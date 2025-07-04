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

add_executable(test_clip_poly_legacy test_poly_legacy.cpp)
add_executable(test_clip_poly_arbitrary test_poly_arbitrary.cpp)
add_executable(test_clip_grid_horizontal test_grid_horizontal.cpp)
add_executable(test_clip_grid_diagonal test_grid_diagonal.cpp)
add_executable(test_gmv_writer test_write.cpp)
add_executable(test_gmv_reader test_read.cpp)
add_executable(test_gmv_read_and_write test_read_and_write.cpp)
add_executable(test_mesh test_mesh.cpp)



target_link_libraries(test_clip_poly_legacy PUBLIC polyclip)
target_link_libraries(test_clip_poly_arbitrary PUBLIC polyclip)
target_link_libraries(test_clip_grid_horizontal PUBLIC polyclip)
target_link_libraries(test_clip_grid_diagonal PUBLIC polyclip)
target_link_libraries(test_gmv_writer PUBLIC polyclip)
target_link_libraries(test_gmv_reader PUBLIC polyclip)
target_link_libraries(test_gmv_read_and_write PUBLIC polyclip)
target_link_libraries(test_gmv_read_and_write PUBLIC polyclip)
target_link_libraries(test_mesh PUBLIC polyclip)

add_test(NAME test_clip_poly_legacy_horizontal COMMAND test_clip_poly_legacy 1 1.e-15 lines/legacy_horizontal_line.dat)
add_test(NAME test_clip_poly_legacy_vertical COMMAND test_clip_poly_legacy 2 1.e-15 lines/legacy_vertical_lines.dat)
add_test(NAME test_clip_poly_legacy_diagonal COMMAND test_clip_poly_legacy 3 5.e-9 lines/legacy_diagonal_lines.dat)
add_test(NAME test_clip_poly_arbitrary COMMAND test_clip_poly_arbitrary 5.e-9 lines/arbitrary_lines.dat)
add_test(NAME test_clip_grid_horizontal COMMAND test_clip_grid_horizontal 1.e-15 lines/horizontal_lines.dat)
add_test(NAME test_clip_grid_diagonal_coincide COMMAND test_clip_grid_diagonal 0 1.e-15 lines/coincide_lines.dat)
add_test(NAME test_clip_grid_diagonal_parallel COMMAND test_clip_grid_diagonal 1 5.e-10 lines/diagonal_lines.dat)
add_test(NAME test_gmv_writer COMMAND test_gmv_writer)
add_test(NAME test_gmv_reader COMMAND test_gmv_reader mesh/inmesh.gmv)
add_test(NAME test_gmv_read_and_write COMMAND test_gmv_read_and_write mesh/inmesh.gmv)

file(COPY ${PROJECT_SOURCE_DIR}/data/mesh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/data/lines DESTINATION ${CMAKE_CURRENT_BINARY_DIR})