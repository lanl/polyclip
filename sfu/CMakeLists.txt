add_executable(test_sfu_mesh test_sfu_mesh.cpp)
target_link_libraries(test_sfu_mesh PUBLIC polyclip)

file(COPY ${PROJECT_SOURCE_DIR}/data/mesh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/data/lines DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
