if (Kokkos_ENABLE_CUDA_UVM)
  add_executable(test_uvm_mesh test_uvm_mesh.cpp)
  target_link_libraries(test_uvm_mesh PUBLIC polyclip)

  file(COPY ${PROJECT_SOURCE_DIR}/data/mesh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${PROJECT_SOURCE_DIR}/data/lines DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()