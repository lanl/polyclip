#include "io.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace polyintersect {

  void io::write_gmv(Mesh_Kokkos mesh, Clipped_Part clipped_part, int num_total_nodes, int num_total_polys, const std::string& file_name) {

    std::fstream gmv_file(file_name, std::ios::app);

    // Original cells and pointsf
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

    gmv_file << "material\n";
    gmv_file << num_total_polys << " 0\n";
    for(int i = 1; i <= num_total_polys; i++) {
      gmv_file << "mat" << i << "\n";
    }

    for(int i = 0; i < num_total_polys; i++) {
      gmv_file << i << " ";
    }

    gmv_file << "\n";


    gmv_file << "endgmv\n";
    gmv_file.close();
  }


  Mesh_Kokkos io::read_gmv(std::string& file_name, Clipped_Part clipped_part,){


      // std::cout << "File name: " << file_name << "\n";
      Mesh_Kokkos mesh = Mesh_Kokkos();
    //  Mesh_Kokkos mesh(11, 4, 6);
     // std::string file_name = basename + ".gmv";
      std::ifstream gmv_file(file_name);
      std::string buffer;


      while(std::getline(gmv_file, buffer)) {
        std::stringstream tokenizer(buffer);
        std::string token;

        // std::cout << "Testing if whipe loop is parsing. . .\n";
        tokenizer >> token;

        // std::cout << "Token = " << token << "\n";
        if(token == "gmvinput") {
          continue;
        }

        if(token == "nodev") {
          std::string node_value;
          tokenizer >> node_value;
          int num_of_nodes = std::stoi(node_value);
          // std::cout << "Node value is. . . " << num_of_nodes << "\n";


          Kokkos::resize(mesh.device_points_, num_of_nodes);  // malloc
          mesh.mirror_points_ = Kokkos::create_mirror_view(mesh.device_points_);


          for(int i = 0; i < num_of_nodes; i++) {
            std::stringstream out;
            std::getline(gmv_file, buffer);
            std::stringstream point_parser(buffer);
            std::string point_data;

            float x, y;


            point_parser >> point_data;
            // out << std::fixed << std::setprecision(17) << std::stof(point_data);

            // std::cout << "Point data  = " << point_data << "\n";
            //
            // auto test_float = std::stof(out.str());
            // std::cout << "Out = " << test_float << "\n";
            x = std::stof(point_data);

            point_parser >> point_data;

            y = std::stof(point_data);

            std::cout << "{x, y} = " << x << " " << y << " " << "\n";

            mesh.add_points(i, {x, y});

          }
        }

        else if(token == "cells") {
          std::string cell_value;
          tokenizer >> cell_value;
          int num_of_cells = std::stoi(cell_value);
          std::cout << "Num Of Cells. . . " << num_of_cells << "\n";

          Kokkos::resize(mesh.device_cells_, num_of_cells, MAX_NUM_EDGES_PER_CELL, 2);
          mesh.mirror_cells_ = Kokkos::create_mirror_view(mesh.device_cells_);
          Kokkos::resize(mesh.num_verts_per_cell_, num_of_cells);
          mesh.mirror_num_verts_per_cell_ = Kokkos::create_mirror_view(mesh.num_verts_per_cell_);
          Kokkos::resize(mesh.signs_, num_of_cells, MAX_NUM_EDGES_PER_CELL + 2);
          mesh.mirror_signs_ = Kokkos::create_mirror_view(mesh.signs_);

          for(int i = 0; i < num_of_cells; i++) {
            std::getline(gmv_file, buffer);
            std::stringstream cell_parser(buffer);
            std::string cell_data;

            //Let's discard the general token and the cell id token.
            cell_parser >> cell_data;
            cell_parser >> cell_data;

            //The next token will signify the # of edges for the cell.

            cell_parser >> cell_data;
            int num_of_edges = std::stoi(cell_data);
            std::cout << "num_of_edges. . . " << num_of_edges << "\n";

            mesh.mirror_num_verts_per_cell_(i) = num_of_edges;
            std::vector<int> list_of_nodes(num_of_edges);
            for(int j = 0; j < num_of_edges; j++) {
              cell_parser >> cell_data;
              list_of_nodes[j] = std::stoi(cell_data) - 1;
              std::cout << "Value added is: " << std::stoi(cell_data) - 1 << "\n";
            }

            for(int k = 0; k < num_of_edges; k++) {
              auto edge_x = list_of_nodes[k];
              auto edge_y = list_of_nodes[(k+1) % (num_of_edges)];
              std::cout << "edge_x/edge_y = " << "( " << edge_x << ", " << edge_y << " )\n";
              mesh.add_edge(i, k, {edge_x, edge_y});
            }
          }
        }
      }

    return mesh;
  }
}
