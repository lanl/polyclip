if (Kokkos_ENABLE_CUDA_UVM)
  add_executable(test_uvm_mesh test_uvm_mesh.cpp)
  target_link_libraries(test_uvm_mesh PUBLIC polyclip)
endif ()