
#include "io.h"
#include <iostream>
#include <fstream>
int main(int argc, char* argv[]){


    using namespace polyintersect;

    Kokkos::initialize(argc, argv);
	{
		// initialize variables for the unstructured mesh
    	int total_points = 11;
    	int total_cells = 4;
    	int max_edges_per_cell = 6;

    	int line_rep = 1; // 1) Horizontal overlapping lines, 2) Arbitrary overlapping lines, 3) Vertical overlapping lines

    	// Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    	Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);

    	// All Nodes
    	mesh.add_points(0, {0.0, 0.0});
    	mesh.add_points(1, {0.5, 0.0});
    	mesh.add_points(2, {0.5, 0.25});
    	mesh.add_points(3, {0.25, 0.375});
    	mesh.add_points(4, {0.0, 0.25});
    	mesh.add_points(5, {0.875, 0.25});
    	mesh.add_points(6, {1.0, 0.375});
    	mesh.add_points(7, {0.875, 0.625});
    	mesh.add_points(8, {0.5, 0.625});
    	mesh.add_points(9, {0.5, 0.875});
    	mesh.add_points(10, {0.875, 0.875});

    	// Pentagon
    	mesh.add_edge(0, 0, {0, 1});
    	mesh.add_edge(0, 1, {1, 2});
    	mesh.add_edge(0, 2, {2, 3});
    	mesh.add_edge(0, 3, {3, 4});
    	mesh.add_edge(0, 4, {4, 0});

    	// Triangle
    	mesh.add_edge(1, 0, {1, 5});
    	mesh.add_edge(1, 1, {5, 2});
    	mesh.add_edge(1, 2, {2, 1});

    	// Hexagon
    	mesh.add_edge(2, 0, {3, 2});
    	mesh.add_edge(2, 1, {2, 5});
    	mesh.add_edge(2, 2, {5, 6});
    	mesh.add_edge(2, 3, {6, 7});
    	mesh.add_edge(2, 4, {7, 8});
    	mesh.add_edge(2, 5, {8, 3});

    	// Square
    	mesh.add_edge(3, 0, {8, 7});
    	mesh.add_edge(3, 1, {7, 10});
    	mesh.add_edge(3, 2, {10, 9});
    	mesh.add_edge(3, 3, {9, 8});

    	// Declare the Number of Vertices
    	mesh.mirror_num_verts_per_cell_(0) = 5;
    	mesh.mirror_num_verts_per_cell_(1) = 3;
    	mesh.mirror_num_verts_per_cell_(2) = 6;
    	mesh.mirror_num_verts_per_cell_(3) = 4;

    	io::write_gmv(mesh, "test");

	}

	std::cout << "I like eggs. . " << std::endl;
	Kokkos::finalize();
  return 0;
}
