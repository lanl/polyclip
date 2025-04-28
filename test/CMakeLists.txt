
add_executable(test_clip_poly_legacy test_poly_legacy.cpp)
add_executable(test_clip_poly_arbitrary test_poly_arbitrary.cpp)
add_executable(test_clip_grid_horizontal test_grid_horizontal.cpp)
add_executable(test_clip_grid_diagonal test_grid_diagonal.cpp)
add_executable(test_gmv_writer test_write.cpp)
add_executable(test_gmv_reader test_read.cpp)
add_executable(test_gmv_read_and_write test_read_and_write.cpp)
add_executable(compare_outputs compare_outputs.cpp)
target_link_libraries(test_clip_poly_legacy PUBLIC polyclip)
target_link_libraries(test_clip_poly_arbitrary PUBLIC polyclip)
target_link_libraries(test_clip_grid_horizontal PUBLIC polyclip)
target_link_libraries(test_clip_grid_diagonal PUBLIC polyclip)
target_link_libraries(test_gmv_writer PUBLIC polyclip)
target_link_libraries(test_gmv_reader PUBLIC polyclip)
target_link_libraries(test_gmv_read_and_write PUBLIC polyclip)
target_link_libraries(compare_outputs PUBLIC polyclip)

add_test(NAME test_clip_poly_legacy_horizontal COMMAND test_clip_poly_legacy 1 1.e-15 "test_clip_poly_legacy_horizontal.txt")
add_test(NAME test_clip_poly_legacy_horizontal_debug COMMAND test_clip_poly_legacy 1 1.e-15 "test_clip_poly_legacy_horizontal.txt" "DEBUG")

add_test(NAME test_clip_poly_legacy_vertical COMMAND test_clip_poly_legacy 2 1.e-15 "test_clip_poly_legacy_vertical.txt")
add_test(NAME test_clip_poly_legacy_vertical_debug COMMAND test_clip_poly_legacy 2 1.e-15 "test_clip_poly_legacy_vertical.txt" "DEBUG")

add_test(NAME test_clip_poly_legacy_diagonal COMMAND test_clip_poly_legacy 3 5.e-9 "test_clip_poly_legacy_diagonal.txt")
add_test(NAME test_clip_poly_legacy_diagonal_debug COMMAND test_clip_poly_legacy 3 5.e-9 "test_clip_poly_legacy_diagonal.txt" "DEBUG")

add_test(NAME test_clip_poly_arbitrary COMMAND test_clip_poly_arbitrary 5.e-9 "test_clip_poly_arbitrary.txt")
add_test(NAME test_clip_poly_arbitrary_debug COMMAND test_clip_poly_arbitrary 5.e-9 "test_clip_poly_arbitrary.txt" "DEBUG")

add_test(NAME test_clip_grid_horizontal COMMAND test_clip_grid_horizontal 1.e-15 "test_clip_grid_horizontal.txt")
add_test(NAME test_clip_grid_horizontal_debug COMMAND test_clip_grid_horizontal 1.e-15 "test_clip_grid_horizontal.txt" "DEBUG")

add_test(NAME test_clip_grid_diagonal_coincide COMMAND test_clip_grid_diagonal 0 1.e-15 "test_clip_grid_diagonal_coincide.txt")
add_test(NAME test_clip_grid_diagonal_coincide_debug COMMAND test_clip_grid_diagonal 0 1.e-15 "test_clip_grid_diagonal_coincide.txt" "DEBUG")

add_test(NAME test_clip_grid_diagonal_parallel COMMAND test_clip_grid_diagonal 1 5.e-10 "test_clip_grid_diagonal_parallel.txt")
add_test(NAME test_clip_grid_diagonal_parallel_debug COMMAND test_clip_grid_diagonal 1 5.e-10 "test_clip_grid_diagonal_parallel.txt" "DEBUG")

add_test(NAME test_gmv_writer COMMAND test_gmv_writer)
add_test(NAME test_gmv_reader COMMAND test_gmv_reader mesh/inmesh.gmv)
add_test(NAME test_gmv_read_and_write COMMAND test_gmv_read_and_write mesh/inmesh.gmv)

file(COPY ${PROJECT_SOURCE_DIR}/data/mesh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})