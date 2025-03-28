#include "../core/clippings_gpu.h"
#include "../core/mesh_gpu.h"
#include "../core/print_gpu.h"
#include "../core/clip_gpu.h"
#include "../core/clipped_part_gpu.h"
//#include "../core/intersect_gpu.h"
#include "../core/intersect_n_d_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

/*#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "print_gpu.h"
#include "clip_gpu.h"
#include "clipped_part_gpu.h"
//#include "intersect_gpu.h"
#include "intersect_n_d_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"
*/
int main(int argc, char * argv[]) {

    using namespace polyintersect;

    Kokkos::initialize(argc, argv);
    {
        // initialize variables for the unstructured mesh
        
        int n_cells = 4;
        int total_cells = n_cells * n_cells;
        int max_edges_per_cell = 4;
        int n_nodes = n_cells + 1;
        int total_points = n_nodes * n_nodes;

        // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
        Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
	Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell);
        double arbitrary[2] = {0.0, -0.08838834765}; 
	int line_rep = 0;	/ (0) Line goes through nodes (1) Line doesnt go through nodes 

        // All Nodes 
	double lengthPerAxis = 1.0;
        double dy = lengthPerAxis / n_cells;
        double dx = dy;
        
	for(int j = 0; j < n_nodes; j++){
            for(int i = 0; i < n_nodes; i++){
            	int k = i + (j * n_nodes);                        // Identify the elements
		double x = (i * dx);
                double y = (j * dy);

            	mesh.add_points(k, {x, y});
	    }
        }
	std::cout << std::endl;

        // All Cells
        for (int j = 0; j < n_cells; j++){                          // Cell Rows
            for (int i = 0; i < n_cells; i++) {                     // Cell Columns
		 int k = i + (j * n_cells);                      // Identify the individual cell elements
                 int a = (k + j);
                 int b = ((k + 1) + j);
                 int c = (((k + 1) + n_nodes) + j);
                 int d = (k + n_nodes + j);

		mesh.add_edge(k , 0, {a, b});
                mesh.add_edge(k , 1, {b, c});
                mesh.add_edge(k , 2, {c, d});
                mesh.add_edge(k , 3, {d, a}); 
            }
        }

        // All Vertices
        for(int i = 0; i < total_cells; i++){
            mesh.mirror_num_verts_per_cell_(i) = 4;
        }

       	// CPU to GPU
        mesh.send_to_gpu();

        // Max Threads and Timer
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
        auto start = timer::now();      


        // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int i) {
	       clipped_part.line_(i).n = {-0.70710678, 0.70710678};        // Arbitrary Lines
       	   clipped_part.line_(i).d = arbitrary[line_rep];
        }); 

        // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
       	clip(total_cells, mesh.device_points_, mesh.device_cells_, clipped_part.intersect_points_, clipped_part.line_, mesh.num_verts_per_cell_,
	      clipped_part.allPoints_, clipped_part.size_output_, clipped_part.output_, mesh.signs_);
 

        auto const end = timer::elapsed(start); // time deep copy

       	// Send to CPU
	mesh.send_to_cpu();
	clipped_part.send_to_cpu();

        auto const end_including_copy = timer::elapsed(start);
	
	// Verify Results by Printing on the CPU ////////////////////////////////////////////////////////////////
	print_results(end, end_including_copy, max_threads, total_cells, total_points, mesh.mirror_points_,
                      mesh.mirror_cells_, clipped_part.mirror_intersect_points_, clipped_part.mirror_line_,
                      mesh.mirror_num_verts_per_cell_, clipped_part.mirror_allPoints_,
                      clipped_part.mirror_size_output_, clipped_part.mirror_output_, mesh.mirror_signs_);

	
    }


    Kokkos::finalize();
    return EXIT_SUCCESS;
}


