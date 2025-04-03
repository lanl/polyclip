
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

    std::cout << "POINTS (NODES)\n-------------------\n";
    for (int i = 0; i < 11; i++) {
      std::cout << "Point " << i << ": (" << mesh.mirror_points_(i).x << ", "
                << mesh.mirror_points_(i).y << ")\n";
    }

    std::cout << "CELLS\n-------------------\n";

    for (int i = 0; i < 4; i++) {
      std::cout << "Cell #" << i << ": ";
      for (int j = 0; j < mesh.mirror_num_verts_per_cell_(i); j++) {
        std::cout << mesh.mirror_cells_(i, j, 0) << ", ";
      }

      std::cout << "\n";
    }
  }

  Kokkos::finalize();

  return 0;
}
