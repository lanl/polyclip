//
// Created by Jonathan Mathurin on 2/16/25.
//

#include "gmv_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>

void gmvstream::set_header(std::fstream &gmv_file) {
  gmv_file << "gmvinput ascii\n";
}

void gmvstream::set_node(std::fstream &gmv_file, polyintersect::Mesh_Kokkos mesh) {
  gmv_file << "nodev " << mesh.mirror_points_.extent(0) << "\n";

}


void gmvstream::set_cell_header(std::fstream &gmv_file, polyintersect::Mesh_Kokkos mesh) {
  gmv_file << "cells " << mesh.mirror_cells_.extent(0) << "\n";
}

void gmvstream::write_gmv(polyintersect::Mesh_Kokkos mesh, const std::string& file_name) {

  std::string lol;
  clear_gmv(file_name);
  std::fstream gmv_file(file_name, std::ios::app);
  set_header(gmv_file);
  set_node(gmv_file, mesh);

  for(int i = 0; i < mesh.mirror_points_.extent(0); i++) {
    auto point = mesh.mirror_points_(i);
    set_point(gmv_file, point);

  }
  set_cell_header(gmv_file, mesh);
  int num_of_cells = mesh.mirror_cells_.extent(0);
  for(int i = 0; i < num_of_cells; i++) {
    gmv_file << "general 1";
    int number_of_vertex = 0;
    int number_of_iterations = mesh.mirror_cells_.extent(1);
    std::string store_points = "";
    for(int j = 0; j < number_of_iterations-1; j++) {
      //polyintersect::Edge const& edge_node = mesh.mirror_cells_(i, j);
      auto& node_val = mesh.mirror_cells_(i, j, 0);
      if(node_val != nullptr) {
        number_of_vertex++;
        store_points += store_points + std::to_string(node_val) + " ";
        //gmv_file << edge_node.a << " ";
      }
    }

    polyintersect::Edge const& edge_node = mesh.mirror_cells_(i, number_of_iterations-1);
    if(edge_node != nullptr) {
      store_points += store_points + std::to_string(edge_node.a);
      number_of_vertex++;
      //gmv_file << edge_node.a;
    }
    gmv_file << std::to_string(number_of_vertex) << " " << store_points;
    gmv_file << "\n";
  }

  gmv_file << "endgmv\n";
  gmv_file.close();
}

polyintersect::Mesh_Kokkos gmvstream::read_gmv(std::string file_name) {

  polyintersect::Mesh_Kokkos new_mesh = polyintersect::Mesh_Kokkos();
  std::fstream gmv_file(file_name, std::ios::in);
  std::string buffer;

  while(std::getline(gmv_file, buffer)) {
    std::stringstream tokenizer(buffer);
    std::string token;


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
