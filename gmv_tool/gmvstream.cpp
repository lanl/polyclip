//
// Created by Jonathan Mathurin on 2/16/25.
//

#include "gmvstream.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// void gmvstream::set_header(std::fstream &gmv_file) {
//   gmv_file << "gmvinput ascii\n";
// }

// void gmvstream::set_point(std::fstream &gmv_file, polyintersect::Point point) {
//   gmv_file << std::scientific << std::setprecision(17);
//   gmv_file << point.x  << " " << point.y << " " << 0.0 << "\n";
// }

// void gmvstream::set_node(std::fstream &gmv_file, polyintersect::Mesh_Kokkos mesh) {
//   gmv_file << "nodev " << mesh.mirror_points_.extent(0) << "\n";
//
// }


// void polyintersect::gmvstream::set_cell_header(std::fstream &gmv_file, polyintersect::Mesh_Kokkos mesh) {
//   gmv_file << "cells " << mesh.mirror_cells_.extent(0) << "\n";
// }
namespace polyintersect {

void gmvstream::write_gmv(Mesh_Kokkos mesh, const std::string& basename) {


  std::string meta_name = basename + ".meta";
  std::string file_name = basename + ".gmv";

  std::ofstream metadata_file(meta_name, std::ios::in);
  std::ofstream gmv_file(file_name, std::ios::in);


  metadata_file << "max_vertex " << mesh.mirror_cells_.extent(1) << "\n";
  metadata_file << "num_of_cells " << mesh.mirror_cells_.extent(0) << "\n";
  metadata_file << "num_of_nodes " << mesh.mirror_points_.extent(0) << "\n";

  gmv_file << "gmvinput ascii\n";
  gmv_file << "nodev " << mesh.mirror_points_.extent(0) << "\n";
  for(int i = 0; i < mesh.mirror_points_.extent(0); i++) {
    auto point = mesh.mirror_points_(i);
    gmv_file << std::scientific << std::setprecision(17);
    gmv_file << point.x  << " " << point.y << " " << 0.0 << "\n";
  }
  gmv_file << "cells " << mesh.mirror_cells_.extent(0) << "\n";
  int num_of_cells = mesh.mirror_cells_.extent(0);
  for(int i = 0; i < num_of_cells; i++) {
    gmv_file << "general 1 ";
    int number_of_vertex = 0;
    std::string store_points = "";

    for(int j = 0; j < mesh.mirror_num_verts_per_cell_(i); j++) {
      //polyintersect::Edge const& edge_node = mesh.mirror_cells_(i, j);
      auto& node_val = mesh.mirror_cells_(i, j, 0);
      number_of_vertex++;
      store_points += std::to_string(node_val + 1) + " ";
      // if(node_val != -1) {
      //   number_of_vertex++;
      //   store_points += std::to_string(node_val + 1) + " ";
      //   //gmv_file << edge_node.a << " ";
      // }
    }

    // auto& node_val = mesh.mirror_cells_(i,number_of_iterations-1, 0);
    // if(node_val != -1) {
    //   store_points += std::to_string(node_val+1);
    //   number_of_vertex++;
    //   //gmv_file << edge_node.a;
    // }
    gmv_file << std::to_string(number_of_vertex) << " " << store_points;
    gmv_file << "\n";
  }

  gmv_file << "endgmv\n";
  gmv_file.close();
}

Mesh_Kokkos gmvstream::read_gmv(std::string& file_name) {

  Mesh_Kokkos new_mesh = Mesh_Kokkos();
  std::fstream gmv_file(file_name, std::ios::in);
  std::string buffer;

  while(std::getline(gmv_file, buffer)) {
    std::stringstream tokenizer(buffer);
    std::string token;
    while(tokenizer >> token) {

      //Let's test if this parses correctly.
      std::cout << token << std::endl;

    }

  }
  //skip line 1 for now
  //read the next lineot a buffer. tokenize it.
    //if the first token is nodev, we extract the second token and store it as num_of_nodes
  //read the next lines from i to num_of_nodes
    //each read should be 3 tokens that we break each into a float.
    //make a list of floats, that we store these into for later use.
  //when we get to cells, we parse to see how many cells there are and store it into num_of_cells
  //loop from i to num_of_cells
    //each time tokenize the buffer.
    // for this purpose, we will throw away the first and second token.
    // third token = number of points
        //from j to num_of_points
              //we extract the remaining token and store it in a vector of vectors.

  //Once we've gathered all the information, it is time to popular the mesh.
    //

  return new_mesh;
}

/* A simple function that will clear out the contents of a GMV. This way we know that the file we're working on is
 * blank. */
void gmvstream::clear_gmv(std::string file_name) {
  std::ofstream clear_file(file_name, std::ios::trunc);
  clear_file.close();
}
