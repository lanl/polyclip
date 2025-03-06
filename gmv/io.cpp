#include "io.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace polyintersect {

  void io::write_gmv(Mesh_Kokkos mesh, Clipped_Part clipped_part, const std::string& basename) {

    std::string file_name = basename + ".gmv";
    std::fstream gmv_file(file_name, std::ios::app);

    int num_of_cells = mesh.mirror_cells_.extent(0);
    int points = mesh.mirror_points_.extent(0);
    
    // Total Nodes + Intersecting points
    int all_points = (num_of_cells * 2) + points;

    // Print all points
    gmv_file << "gmvinput ascii\n";
    gmv_file << "nodev " << all_points << "\n";

    // First print original points 
    for(int i = 0; i < points; i++) {
      auto point = mesh.mirror_points_(i);
      gmv_file << std::scientific << std::setprecision(17);
      gmv_file << point.x  << " " << point.y << " " << 0.0 << "\n";
    }

    // Then print intersecting points
    for(int i = 0; i < num_of_cells; i++){
        auto const pa = clipped_part.mirror_intersect_points_(i).a;
        auto const pb = clipped_part.mirror_intersect_points_(i).b;
        gmv_file << pa.x << " "<< pa.y << " " << 0.0 << "\n";
        gmv_file << pb.x << " "<< pb.y << " " << 0.0 << "\n";
    }

    // Print Cells and Clipped Cells
    gmv_file << "cells " << num_of_cells + num_of_cells << "\n";

    // Original Cells
    for(int i = 0; i < num_of_cells; i++) {
      gmv_file << "general 1 ";
      int number_of_vertex = 0;
      std::string store_points = "";

      int num_verts = mesh.mirror_num_verts_per_cell_(i);
      for(int j = 0; j < num_verts ; j++) {
        auto& node_val = mesh.mirror_cells_(i, j, 0);
        store_points += std::to_string(node_val + 1) + " ";
     }
      gmv_file << std::to_string(num_verts) << " " << store_points;
      gmv_file << "\n";
    }

    // Clipped Cells
    for(int c = 0; c < num_of_cells; c++){
	gmv_file << "general 1 ";
	std::string store_points = "";

        int below = clipped_part.mirror_size_output_(c, 0);

        for(int i = 0; i < below; i++){
            int const j = clipped_part.mirror_output_(c, 0, i);
            auto const p = clipped_part.mirror_allPoints_(c, j);

        }

	gmv_file << std::to_string(below) << " " << store_points;
    	gmv_file << "\n";
    }

    gmv_file << "endgmv\n";
    gmv_file.close();
 }
}
