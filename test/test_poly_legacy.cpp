#include "../core/clippings.h"
#include "../core/mesh.h"
#include "../core/print.h"
#include "../core/clip.h"
#include "../core/clipped_part.h"
//#include "../core/intersect.h"
#include "../core/intersect_n_d.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

int main(int argc, char * argv[]) {

    using namespace polyintersect;

    Kokkos::initialize(argc, argv);
    {
        // initialize variables for the unstructured mesh
        int total_points = 11;
        int total_cells = 4;
        int max_edges_per_cell = 6;

	int line_rep = 1; // 1) Horizontal overlapping lines, 2) Vertical overlapping lines,  3) Arbitrary overlapping lines 
	
	// Testing: distances for every cell
	double horizontal[4] = {-0.125, -0.125, -0.5, -0.75}; 
	double vertical[4] = {-1 /*-0.375*/, -0.625, -0.75, -0.625}; //Test dummy: replace -0.375 with -1
	double arbitrary[4] = {-1 /*-0.26516504294495535*/, -0.4419417382415923, -0.618718433538229, -0.8838834764831844}; // Test dummy: replace with -1
	
        // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
        Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
	Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell);

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

       	// CPU to GPU
        mesh.send_to_gpu();

        // Max Threads and Timer
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
        auto start = timer::now();      


        // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int i) {
	   if (line_rep == 1){				// Horizontal Lines
	       clipped_part.line_(i).n = {0.0, 1.0};
       	       clipped_part.line_(i).d = horizontal[i];
	   } else if (line_rep == 2){			// Vertical Lines
               clipped_part.line_(i).n = {1.0, 0.0};
               clipped_part.line_(i).d = vertical[i];
	   } else{					// Arbitrary Lines
               clipped_part.line_(i).n = {0.70710678, 0.70710678};
               clipped_part.line_(i).d = arbitrary[i];
	     }
        }); 

        // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
       	clip(total_cells, mesh.device_points_, mesh.device_cells_, clipped_part.intersect_points_, clipped_part.line_, mesh.num_verts_per_cell_,
	      clipped_part.allPoints_, clipped_part.size_output_, clipped_part.output_, mesh.signs_);

        int const end = timer::elapsed(start); // time deep copy

       	// Send to CPU
	mesh.send_to_cpu();
	clipped_part.send_to_cpu();
	int const end_including_copy = timer::elapsed(start);
	
	// Verify Results by Printing on the CPU ////////////////////////////////////////////////////////////////
	print_results(end, end_including_copy, max_threads, total_cells, total_points, mesh.mirror_points_, 
                      mesh.mirror_cells_, clipped_part.mirror_intersect_points_, clipped_part.mirror_line_, 
                      mesh.mirror_num_verts_per_cell_, clipped_part.mirror_allPoints_, 
                      clipped_part.mirror_size_output_, clipped_part.mirror_output_, mesh.mirror_signs_);
	
    }


    Kokkos::finalize();
    return EXIT_SUCCESS;
}

