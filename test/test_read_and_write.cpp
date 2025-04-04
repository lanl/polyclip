
#include "../gmv/io.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
  using namespace polyclip;

  if (argc < 2) {
    std::cout << "You need to pass the file name bro." << std::endl;
    return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {
    std::string file_name = argv[1];
    Mesh_Kokkos mesh = io::read_mesh(file_name);

    std::cout << "points \n-------------------\n";
    for (int i = 0; i < mesh.mirror_points_.extent(0); i++) {
      auto const& p = mesh.mirror_points_(i);
      std::cout << "p[" << i << "]: (" << p.x << ", " << p.y << ")\n";
    }

    std::cout << "cells\n-------------------\n";

    for (int i = 0; i < mesh.mirror_cells_.extent(0); i++) {
      std::cout << "c[" << i << "]: [";
      for (int j = 0; j < mesh.mirror_num_verts_per_cell_(i); j++) {
        std::cout << mesh.mirror_cells_(i, j, 0) << ", ";
      }
      std::cout << "]" << std::endl;
    }

    std::cout << "Now let's write this to 'test_read_write.gmv'" << std::endl;
    io::write_mesh(mesh, "test_read_write.gmv");
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}