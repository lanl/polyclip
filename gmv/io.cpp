//
// Created by Jonathan Mathurin on 2/16/25.
//

#include "io.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace polyintersect {

  void io::write_gmv(Mesh_Kokkos mesh, const std::string& basename) {

    std::string file_name = basename + ".gmv";
    std::fstream gmv_file(file_name, std::ios::app);

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
      }

      gmv_file << std::to_string(number_of_vertex) << " " << store_points;
      gmv_file << "\n";
    }

    gmv_file << "endgmv\n";
    gmv_file.close();
  }
}

