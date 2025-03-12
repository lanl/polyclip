//
// Created by Jonathan Mathurin on 2/19/25.
//

using namespace polyintersect;

int main(int argc, char* argv[]){

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
