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

        // Create mesh
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

        Kokkos::View<Line*> line("line", total_cells);
        Kokkos::View<Line*> interface("interface", total_cells);
        Kokkos::View<int**> output("output", total_cells, max_edges_per_cell);
        Kokkos::View<int*> size_output("sizeoutput", total_cells);                                           
        Kokkos::View<Point**> allPoints("allpoints", total_cells, (max_edges_per_cell + 2));    // Cell points + intersect points

        // Max Threads
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;

        // Timer
        auto start = timer::now();      

        // Clipping below for Every Cell 
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int c) {            
            line(c) = fake_intersect_cell(c);
	    interface(c) = intersect_cell_with_line(mesh.device_points_, mesh.device_cells_, c, line(c), mesh.num_verts_per_cell_);
            clip_below_3(c, mesh.device_points_, mesh.device_cells_, interface(c),
                         output, size_output, mesh.num_verts_per_cell_, mesh.signs_, allPoints);
        });

        auto const end = timer::elapsed(start);

        // CPU Copy 
        auto mirror_output = Kokkos::create_mirror_view(output);
	auto mirror_size_output = Kokkos::create_mirror_view(size_output);
        auto mirror_allPoints = Kokkos::create_mirror_view(allPoints);
        auto mirror_interface = Kokkos::create_mirror_view(interface);
       // auto mirror_line = Kokkos::create_mirror_view(line);

	Kokkos::deep_copy(mirror_output, output);
	Kokkos::deep_copy(mirror_size_output, size_output);
        Kokkos::deep_copy(mirror_allPoints, allPoints);
        Kokkos::deep_copy(mirror_interface, interface);
        //Kokkos::deep_copy(mirror_line, line);

	mesh.send_to_cpu();

        auto const end_including_copy = timer::elapsed(start);

        // Print elapsed time 
        std::cout << "Duration: " << end << " µs" << std::endl;
        std::cout << "Deep copy: " << end_including_copy << " µs" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 

  
        // GPU Results ///////////////////////////////////////////////////////////////////////
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
        
	// Print Interface 
        std::cout << std::endl;
        std::cout << "------ Interface ------" << std::endl;
        for (int j = 0; j < total_cells; ++j) {
            auto const pa = mirror_interface(j).a;
            auto const pb = mirror_interface(j).b;
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
	    for(int i = 0; i < t; i++){
                int const j = mirror_output(c, i);
                auto const p = mirror_allPoints(c, j);
                std::cout << "Below line at cell " << c << ", Coordinate " << i << ": (";
		std::cout << p.x << ", "<< p.y << ") "<< std::endl;
            }
            std::cout << std::endl;
        }
    }
    Kokkos::finalize();
    return EXIT_SUCCESS;
}

