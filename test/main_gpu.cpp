#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "intersect_gpu.h"
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

	int line_rep = 3; // 1) Arbitrary lines intersect points. 2) Horizontal overlapping lines, 3) Arbitrary overlapping lines 

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

       	// CPU to GPU
        mesh.send_to_gpu();

	// Important Data Memebers //////////////////////////////////////////////////////////////////////////////
        Kokkos::View<Line*> line("line", total_cells);
        Kokkos::View<Line*> intersect_points("intersect", total_cells);
        Kokkos::View<int**> output("output", total_cells, max_edges_per_cell);  // Cell ID, Edge ID
        Kokkos::View<int*> size_output("sizeoutput", total_cells);                                           
        Kokkos::View<Point**> allPoints("allpoints", total_cells, (max_edges_per_cell + 2));  // Cell ID, All Points Coordinate (Vertices + intersect points)

        // Max Threads and Timer
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
        auto start = timer::now();      

        // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int i) {
	  if (line_rep == 0){
            switch(i){
              case 0:     // Cell 0
                 line(i) = {{.375, 0}, {0.015625, 0.257812}};
                 break;
              case 1:     // Cell 1
                 line(i) = {{0.575, 0.05}, {0.5, 0.125}};
                 break;
              case 2:     // Cell 2
                 line(i) = {{0.625, 0.25}, {0.375, 0.5}};
                 break;
              case 3:     // Cell 3
                 line(i) = {{0.625, 0.625}, {0.5, 0.75}};
                 break;
              default:
                 line(i) = {{-1.0, -1.0}, {-1.0, -1.0}};
                 break;
                }
	   } else if (line_rep == 2){
	     switch(i){
              case 0:     // Cell 0
                 line(i) = {{1, 0.125}, {-1, 0.125}};
                 break;
              case 1:     // Cell 1
                 line(i) = {{1, 0.125}, {-1, 0.125}};
                 break;
              case 2:     // Cell 2
                 line(i) = {{1.5, 0.5}, {-1, 0.5}};
                 break;
              case 3:     // Cell 3
                 line(i) = {{1.5, 0.75}, {-1, 0.75}};
                 break;
              default:
                 line(i) = {{-1.0, -1.0}, {-1.0, -1.0}};
                 break;
                }
	   } else{
	      switch(i){
              case 0:     // Cell 0
                 line(i) = {{0.625, -0.25}, {-0.125, 0.375}};
                 break;
              case 1:     // Cell 1
                 line(i) = {{.75, -0.125}, {0.375, 0.25}};
                 break;
              case 2:     // Cell 2
                 line(i) = {{0.875, 0.0}, {0.25, 0.625}};
                 break;
              case 3:     // Cell 3
                 line(i) = {{0.75, 0.5}, {0.375, 0.875}};
                 break;
              default:
                 line(i) = {{-1.0, -1.0}, {-1.0, -1.0}};
                 break;
                }
	   }
        });

        // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int c) {            
	    intersect_points(c) = intersect_cell_with_line(mesh.device_points_, mesh.device_cells_, c, line(c), mesh.num_verts_per_cell_);
            clip_below_3(c, mesh.device_points_, mesh.device_cells_, intersect_points(c),
                         output, size_output, mesh.num_verts_per_cell_, mesh.signs_, allPoints);
        });
	
	// Verify Results by Printing on the CPU //////////////////////////////////////////////////////////////// 
        auto const end = timer::elapsed(start); // time deep copy

        // CPU Copy 
        auto mirror_output = Kokkos::create_mirror_view(output);
	auto mirror_size_output = Kokkos::create_mirror_view(size_output);
        auto mirror_allPoints = Kokkos::create_mirror_view(allPoints);
        auto mirror_intersect_points = Kokkos::create_mirror_view(intersect_points);
        auto mirror_line = Kokkos::create_mirror_view(line);

	Kokkos::deep_copy(mirror_output, output);
	Kokkos::deep_copy(mirror_size_output, size_output);
        Kokkos::deep_copy(mirror_allPoints, allPoints);
        Kokkos::deep_copy(mirror_intersect_points, intersect_points);
	Kokkos::deep_copy(mirror_line, line);

	// Send to CPU
	mesh.send_to_cpu();
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
            for (int i = 0; i < max_edges_per_cell; i++) {      // Edge       
                std::cout << "Edge " << i << " (" << mesh.mirror_cells_(j , i, 0) << ", ";
                std::cout << mesh.mirror_cells_(j, i, 1) << ") ";

                if(j == 0 && i == 4 || j == 1 && i == 2 || j == 3 && i == 3){   
                    break;
                }
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
            auto const pa = mirror_line(j).a;
            auto const pb = mirror_line(j).b;
            std::cout << "Line at Cell  "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
        }
        
	// Print Intersect Points
        std::cout << std::endl;
        std::cout << "------ Intersect Points ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
            auto const pa = mirror_intersect_points(j).a;
            auto const pb = mirror_intersect_points(j).b;
            std::cout << "Intersection Points at Cell  "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
        }

	// Print all Points (Vertices + Intersect Points)
        std::cout << std::endl;
	std::cout << "------ All Points ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
		int t = mesh.mirror_num_verts_per_cell_(c) + 2;
            for(int i = 0; i < t; i++){
                auto const p = mirror_allPoints(c, i);
                std::cout << "Points at Cell  " << c << ": (" << p.x << ", "<< p.y << ") "<< std::endl;
            }
            std::cout << std::endl;
        }

        // Output Results
        std::cout << std::endl;
	std::cout << "------ Output ------" << std::endl;
        for(int c = 0; c < total_cells; c++){
	    int t = mirror_size_output(c); 
	   // std::cout << "Total outputs: " << t << std::endl;
	    for(int i = 0; i < t; i++){
                int const j = mirror_output(c, i);
                auto const p = mirror_allPoints(c, j);
                std::cout << "Below line at cell " << c << ", Coordinate " << i << ": (";
	        std::cout << p.x << ", "<< p.y << ") "<< std::endl;
            }
            std::cout << std::endl;
        }

	// Print signs
	std::cout << "------ Signs ------" << std::endl;
        for(int i = 0; i < total_cells; i++){
            int t = mesh.mirror_num_verts_per_cell_(i) + 2;
            for(int j = 0; j < t; j++){
                std::cout << "Sign at cell "<< i << ": " << mesh.mirror_signs_(i, j) << std::endl;
            }
            std::cout << std::endl;
        }
	

	std::cout << "----------- CPU Sign RESULTS -----------" << std::endl;
	for(int cell = 0; cell < total_cells; cell++){
	    Point normal = normVec(mirror_intersect_points(cell));
	    Point middle = middle_point(mirror_intersect_points(cell));
         
            double dp;
            int n = mesh.mirror_num_verts_per_cell_(cell) + 2;

            for(int p = 0; p < n; p++){ // n = num_verts_per_cell + 2
                // Vector of Node
	       Point const V = pointVec(mirror_allPoints(cell, p), middle);

                // Dot Product of normal and node vector
                dp = (V.x * normal.x) + (V.y * normal.y);

                // Convection of placement with respect to the line
             if (dp < 0) {           // Below the line
                    mesh.mirror_signs_(cell, p) = -1;
                } else if (dp > 0) {    // Above the line
                   mesh. mirror_signs_(cell, p) = 1;
                } else {                // On the line
                    mesh.mirror_signs_(cell, p) = 0;
                }
                // index++;
                std::cout << "Cell " << cell << ", at coordinate " << p << ": " << mesh.mirror_signs_(cell, p) << std::endl;
	/*	if(cell == 1 and p == 3 or p == 4 and cell == 1){
			std::cout << "MIRROR POINT: " << mirror_allPoints(cell, p).x << ", " << mirror_allPoints(cell, p).y << std::endl;
			std::cout << "MIRROR LINE: " << mirror_intersect_points(cell).a.x << ", " << mirror_intersect_points(cell).a.y << "      " << mirror_intersect_points(cell).b.x << ", " << mirror_intersect_points(cell).b.y << std::endl;
			std::cout << "MIRROR MIDDLE: " << middle.x << ", " << middle.y << std::endl;
		}*/

	    }
	    std::cout << std::endl; 
        }


    }
    Kokkos::finalize();
    return EXIT_SUCCESS;
}

