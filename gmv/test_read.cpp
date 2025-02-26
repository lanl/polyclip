
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
    Mesh_Kokkos mesh = io::read_gmv(file_name);

  }


  Kokkos::finalize();

  return 0;
}
