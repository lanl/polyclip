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
        
        int total_cells = 10;
        int max_edges_per_cell = 6;
        int total_points = 17;

        // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
        Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
	Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell);

        int vertices[10] = {5, 3, 6, 4, 3, 5, 3, 4, 4, 4};
        double mixed[10] = {-0.375, -0.4419417382415923, -0.5, -0.8838834764831844, -2, -2, -2, -2, -2, -2}; 

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

        // New points
        mesh.add_points(11, {0.0, 0.375});
        mesh.add_points(12, {0.0, 0.625});
        mesh.add_points(13, {0.25, 0.625});
        mesh.add_points(14, {0.0, 0.875});
        mesh.add_points(15, {1.0, 0.0});
        mesh.add_points(16, {1.0, 0.875});

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

        // New Shapes ///////////////////////////
        mesh.add_edge(4, 0, {4, 3});        // Cell 4
        mesh.add_edge(4, 1, {3, 11});
        mesh.add_edge(4, 2, {11, 4});

        mesh.add_edge(5, 0, {11, 3});       // Cell 5
        mesh.add_edge(5, 1, {3, 8});
        mesh.add_edge(5, 2, {8, 13});
        mesh.add_edge(5, 3, {13, 12});
        mesh.add_edge(5, 4, {12, 11});

        mesh.add_edge(6, 0, {12, 13});      // Cell 6
        mesh.add_edge(6, 1, {13, 14});
        mesh.add_edge(6, 2, {14, 12});

        mesh.add_edge(7, 0, {13, 8});        // Cell 7
        mesh.add_edge(7, 1, {8, 10});
        mesh.add_edge(7, 2, {10, 14});
        mesh.add_edge(7, 3, {14, 13});

        mesh.add_edge(8, 0, {1, 15});        // Cell 8
        mesh.add_edge(8, 1, {15, 6});
        mesh.add_edge(8, 2, {6, 5});
        mesh.add_edge(8, 3, {5, 1});

        mesh.add_edge(9, 0, {6, 16});        // Cell 9
        mesh.add_edge(9, 1, {16, 9});
        mesh.add_edge(9, 2, {9, 7});
        mesh.add_edge(9, 3, {7, 6});
 
        // Declare the Number of Vertices
        for(int i = 0; i < total_cells; i++){
            mesh.mirror_num_verts_per_cell_(i) = vertices[i];
        }
       	
       	// CPU to GPU
        mesh.send_to_gpu();

        // Max Threads and Timer
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
        auto start = timer::now();      


        // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int i) {
	       if(i == 0){                                      // Vertical Lines
               clipped_part.line_(i).n = {1.0, 0.0};
               clipped_part.line_(i).d = mixed[i];
           }
           else if(i == 1 || i == 3){                       // Arbitrary Lines
               clipped_part.line_(i).n = {0.70710678, 0.70710678};
               clipped_part.line_(i).d = mixed[i];
           }
           else{
                clipped_part.line_(i).n = {0.0, 1.0};        // Horizontal Line
       	        clipped_part.line_(i).d = mixed[i];
            }
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


