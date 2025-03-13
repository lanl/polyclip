#include "io.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace polyintersect {

  void io::write_gmv(Mesh_Kokkos mesh, Clipped_Part clipped_part, int num_total_nodes, int num_total_polys, const std::string& basename) {

    std::string file_name = basename + ".gmv";
    std::fstream gmv_file(file_name, std::ios::app);

    // Original cells and points
    int total_cells = mesh.mirror_cells_.extent(0);
    int points = mesh.mirror_points_.extent(0);

    // Print all points
    gmv_file << "gmvinput ascii\n";
    gmv_file << "nodev " << num_total_nodes << "\n";

    // Print all points ////////////////////////////////////////////////////////////////////////////////////
    for(int c = 0; c < total_cells; c++){
	int below = clipped_part.mirror_size_output_(c, 0);
	int v;

	// Check if its a Clipped Cell
	if(below == 0){		// Non-clipped cell
        	v = mesh.mirror_num_verts_per_cell_(c);
		for(int i = 0; i < v; i++){
		    gmv_file << std::scientific << std::setprecision(17);
		    int id = mesh.mirror_cells_(c, i, 0);
		    auto const p = mesh.mirror_points_(id);
		    gmv_file << p.x << " "<< p.y << " " << 0.0 << "\n";
		}
	}else{			// Clipped cell
		v = mesh.mirror_num_verts_per_cell_(c) + 2;
		for(int i = 0; i < v; i++){
            	    gmv_file << std::scientific << std::setprecision(17);
            	    auto const p = clipped_part.mirror_allPoints_(c, i);
            	    gmv_file << p.x << " "<< p.y << " " << 0.0 << "\n";
        	}
	}
    }

    // Print Cells + All Clipped Cells
    gmv_file << "cells " << num_total_polys << "\n";

    // Print Cell Nodes ////////////////////////////////////////////////////////////////////////////////////
    int node_increment = 0;	// keep track of what node we are on with respect to the gmv file
    for(int c = 0; c < total_cells; c++){
        gmv_file << "general 1 ";
        std::string store_points = "";
        int num_verts = mesh.mirror_num_verts_per_cell_(c);

        int below = clipped_part.mirror_size_output_(c, 0);
        int above = clipped_part.mirror_size_output_(c, 1);

        // Cell with no Clipping ///////////////////
        if(below == 0){
           for(int j = 0; j < num_verts ; j++) {
               int const node_id = j + node_increment;
               store_points += std::to_string(node_id + 1) + " ";
           }
           gmv_file << std::to_string(num_verts) << " " << store_points << "\n";

           // vertex increment 
           node_increment += num_verts;
        }

        // Clipped Cell //////////////////////////
        else{ 
            for(int i = 0; i < below; i++){
                int const j = clipped_part.mirror_output_(c, 0, i);
                int const node_id = j + node_increment;
                store_points += std::to_string(node_id + 1) + " ";
            }

            // Cell Below
            gmv_file << std::to_string(below) << " " << store_points << "\n";
            std::string store_points = "";

            for(int i = 0; i < above; i++){
                int const j = clipped_part.mirror_output_(c, 1, i);
                int const node_id = j + node_increment;
                store_points += std::to_string(node_id + 1) + " ";
            }

            // Cell Above
            gmv_file << "general 1 ";
            gmv_file << std::to_string(above) << " " << store_points << "\n";

            // vertices + intersect points increment
            node_increment += num_verts;  
            node_increment += 2;       


        }
    }

    gmv_file << "endgmv\n";
    gmv_file.close();
 }
}
