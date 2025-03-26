
#include "io.h"
#include <iostream>
#include <fstream>


int main(int argc, char* argv[]){
  using namespace polyintersect;

  if(argc < 2){
    std::cout << "You need to pass the file name bro." << std::endl;
  }

  Kokkos::initialize(argc, argv);
  {
    std::string file_name = argv[1];
    Mesh_Kokkos mesh = io::read_mesh(file_name);

    std::cout << "POINTS (NODES)\n-------------------\n";
    for(int i = 0; i < 11; i++){
      std::cout << "Point " << i << ": (" << mesh.mirror_points_(i).x << ", " <<  mesh.mirror_points_(i).y << ")\n";
    }

    std::cout << "CELLS\n-------------------\n";

    for(int i = 0; i < 4; i++){

      std::cout << "Cell #" << i << ": ";
      for(int j = 0; j < mesh.mirror_num_verts_per_cell_(i); j++){
        std::cout << mesh.mirror_cells_(i, j, 0) << ", ";
      }

      std::cout << "\n";

    }


    std::cout << "Now let's write this to a new file called tester2.gmv\n";
    //io::write_gmv(mesh, "tester2.gmv");


  }


  Kokkos::finalize();

  return 0;
}