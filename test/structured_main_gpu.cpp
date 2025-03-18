#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "clipped_part_gpu.h"
//#include "intersect_gpu.h"
#include "intersect_n_d_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

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
        double horizontal[16] = {-2, -2, -2, -2, -2, -2, -2, -2, -0.625, -0.625, -0.625, -0.625, -2, -2, -2, -2}; 

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
	       clipped_part.line_(i).n = {0.0, 1.0};        // Horizontal Lines
       	   clipped_part.line_(i).d = horizontal[i];
        }); 

        // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int c) {            
	    clipped_part.intersect_points_(c) = intersect_cell_with_line_n_d(mesh.device_points_, mesh.device_cells_, c, clipped_part.line_(c), mesh.num_verts_per_cell_);
	    
	    // Check if cell contains intersect points
	    if(intersects(mesh.device_points_, mesh.device_cells_, c, clipped_part.intersect_points_(c), mesh.num_verts_per_cell_)){
            	clip_below_3(c, mesh.device_points_, mesh.device_cells_, clipped_part.intersect_points_(c),
                             clipped_part.output_, clipped_part.size_output_, mesh.num_verts_per_cell_, mesh.signs_, 
			     clipped_part.allPoints_, clipped_part.line_(c));
           }
        });
	
	// Verify Results by Printing on the CPU //////////////////////////////////////////////////////////////// 
        auto const end = timer::elapsed(start); // time deep copy

       	// Send to CPU
	mesh.send_to_cpu();
	clipped_part.send_to_cpu();

        auto const end_including_copy = timer::elapsed(start);

        // Print elapsed time 
        std::cout << "Duration: " << end << " µs" << std::endl;
        std::cout << "Deep copy: " << end_including_copy << " µs" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 

        // Print Cells 
        std::cout << std::endl;
        std::cout << "---------------- GPU Results ----------------" << std::endl;
	std::cout << std::endl;

	std::cout << "------ Cell + Edges ------" << std::endl;
        for(int j = 0; j < total_cells; j++){   // Cell
            std::cout << "Cell " << j << ":" << std::endl;
	    int t = mesh.mirror_num_verts_per_cell_(j);
            for (int i = 0; i < t; i++) {      // Edge       
                std::cout << "Edge " << i << " (" << mesh.mirror_cells_(j , i, 0) << ", ";
                std::cout << mesh.mirror_cells_(j, i, 1) << ") ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // Print Point Coordinates
	std::cout << "------ Cell Vertices ------" << std::endl;
        for (int j = 0; j < total_points; j++) {             // All Points
                std::cout << "Point " << j << ": (" << mesh.mirror_points_(j).x << ", " << mesh.mirror_points_(j).y << ")" << std::endl;
        
        }

	// Print Line
        std::cout << std::endl;
        std::cout << "------ Line ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
            auto const pa = clipped_part.mirror_line_(j).n;
            auto const dist = clipped_part.mirror_line_(j).d;
            std::cout << "Line at Cell  "<< j << ": normal = ("<< pa.x << ", "<< pa.y << ") and distance = " << dist << std::endl;
        }
       
	// Print Intersect Points
        std::cout << std::endl;
        std::cout << "------ Intersect Points ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
            auto const pa = clipped_part.mirror_intersect_points_(j).a;
            auto const pb = clipped_part.mirror_intersect_points_(j).b;
            std::cout << "Intersection Points at Cell  "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
        }

	// Print all Points (Vertices + Intersect Points)
        std::cout << std::endl;
	std::cout << "------ All Points ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
            int below = clipped_part.mirror_size_output_(c, 0);
            int t;
            if(below == 0){
                t = mesh.mirror_num_verts_per_cell_(c);
            }
            else{
	        t = mesh.mirror_num_verts_per_cell_(c) + 2;
            }
            for(int i = 0; i < t; i++){
                auto const p = clipped_part.mirror_allPoints_(c, i);
                std::cout << "Points at Cell  " << c << ": (" << p.x << ", "<< p.y << ") "<< std::endl;
            }
            std::cout << std::endl;
        }

        // Output Results
        std::cout << std::endl;
	std::cout << "------ Output ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
	    int below = clipped_part.mirror_size_output_(c, 0);
	    int above = clipped_part.mirror_size_output_(c, 1);
	    if(below > 0){
	    	std::cout << "Cell " << c << " Output: " << std::endl;
	    	for(int i = 0; i < below; i++){
                	int const j = clipped_part.mirror_output_(c, 0, i);
                	auto const p = clipped_part.mirror_allPoints_(c, j);
	        	std::cout << "Below: (" << p.x << ", "<< p.y << ") "<< std::endl;
            	}
	    	for(int i = 0; i < above; i++){
               	 	int const j = clipped_part.mirror_output_(c, 1, i);
                	auto const p = clipped_part.mirror_allPoints_(c, j);
                	std::cout << "Above: (" << p.x << ", "<< p.y << ") "<< std::endl;
            	}	
	    	std::cout << std::endl;
	    }
        }

	// Print signs
	std::cout << "------ Signs ------" << std::endl;
        for(int i = 0; i < total_cells; i++){
	    int below = clipped_part.mirror_size_output_(i, 0);
            int t = mesh.mirror_num_verts_per_cell_(i) + 2;
	    if(below > 0){
            	for(int j = 0; j < t; j++){
                	std::cout << "Sign at cell "<< i << ": " << mesh.mirror_signs_(i, j) << std::endl;
            	}
            	std::cout << std::endl;
	    }
        }
	
    }


    Kokkos::finalize();
    return EXIT_SUCCESS;
}

