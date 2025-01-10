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
        mesh.add_all_points(0, {0.0, 0.0});
        mesh.add_all_points(1, {0.5, 0.0});
        mesh.add_all_points(2, {0.5, 0.25});
        mesh.add_all_points(3, {0.25, 0.375});
        mesh.add_all_points(4, {0.0, 0.25});
        mesh.add_all_points(5, {0.875, 0.25});
        mesh.add_all_points(6, {1.0, 0.375});
        mesh.add_all_points(7, {0.875, 0.625});
        mesh.add_all_points(8, {0.5, 0.625});
        mesh.add_all_points(9, {0.5, 0.875});
        mesh.add_all_points(10, {0.875, 0.875});

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
        mesh.add_edge(2, 0, {2, 5});
        mesh.add_edge(2, 1, {5, 6});
        mesh.add_edge(2, 2, {6, 7});
        mesh.add_edge(2, 3, {7, 8});
        mesh.add_edge(2, 4, {8, 3});   
        mesh.add_edge(2, 5, {3, 2});   

        // Square
        mesh.add_edge(3, 0, {8, 7});
        mesh.add_edge(3, 1, {7, 10});
        mesh.add_edge(3, 2, {10, 9});
        mesh.add_edge(3, 3, {9, 8});
 
        // CPU to GPU
        mesh.send_to_gpu();

        //Kokkos::View<Line*> line("line", total _cells);
        Kokkos::View<Line*> interface("interface", total_cells);
        Kokkos::View<int**> output("output", total_cells, max_edges_per_cell);
        Kokkos::View<int*> size_output("sizeoutput", total_cells);                                           
        Kokkos::View<Point**> allPoints("allpoints", total_cells, (max_edges_per_cell + 2));    // Cell points + intersect points

        // Max Threads
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;

        // Horizontal ///////////////////////////////
        auto start = timer::now();  // Timer 
        

        // Clipping below for Every Cell 
        Kokkos::parallel_for(total_cells, KOKKOS_LAMBDA(int c) {            
            interface(c) = fake_intersect_cell(c);
            clip_below_3(c, mesh.device_points_, mesh.device_cells_, interface(c),
                         output, size_output, mesh.num_verts_per_cell_, mesh.signs_, allPoints);
        });

        auto const end = timer::elapsed(start);

        // CPU Copy 
        auto mirror_output = Kokkos::create_mirror_view(output);
        auto mirror_allPoints = Kokkos::create_mirror_view(allPoints);
        auto mirror_interface = Kokkos::create_mirror_view(interface);
       // auto mirror_line = Kokkos::create_mirror_view(line);

        Kokkos::deep_copy(mesh.mirror_points_, mesh.device_points_);
        Kokkos::deep_copy(mesh.mirror_cells_, mesh.device_cells_);
        Kokkos::deep_copy(mirror_output, output);
        Kokkos::deep_copy(mirror_allPoints, allPoints);
        Kokkos::deep_copy(mirror_interface, interface);
        //Kokkos::deep_copy(mirror_line, line);

        auto const end_including_copy = timer::elapsed(start);

        // Print elapsed time 
        std::cout << "Duration: " << end << " µs" << std::endl;
        std::cout << "Deep copy: " << end_including_copy << " µs" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 
/*
    #ifdef PRINT_EVERYTHING
        // print interfaces
        int n_nodes = n_cells + 1;

        for (int j = 0; j < n_cells; ++j) {
            auto const pa = mirror_line(j).a;
            auto const pb = mirror_line(j).b;
            std::cout << "line: "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
        }

        for (int j = 0; j < n_cells * n_cells; ++j) {
            auto const pa = mirror_interface(j).a;
            auto const pb = mirror_interface(j).b;
            std::cout << "intersection points: "<< j << ": ("<< pa.x << ", "<< pa.y << "), ("<< pb.x << ", "<< pb.y << ")" << std::endl;
        }

        // Print Cells 
        for(int j = 0; j < n_cells; j++){               // Cell Rows
            for (int i = 0; i < n_cells; i++) {         // Cell Columns
                int k = i + (j * n_cells);              // Identify the individual cell elements
                std::cout << "Cell " << k << ": " << mesh.mirror_cells_(k , 0) << ", ";
                std::cout << mesh.mirror_cells_(k , 1) << ", " << mesh.mirror_cells_(k , 2) << ", " << mesh.mirror_cells_(k , 3) << std::endl;
            }
        }
        std::cout << std::endl;

        // Print Point Coordinates
        for (int j = 0; j < n_nodes; j++) {             // Rows
            for (int i = 0; i < n_nodes; i++) {         // Columns
                int k = i + (j * n_nodes);              // Identify the elements
                std::cout << "Point " << k << ": (" << mesh.mirror_points_(k).x << ", " << mesh.mirror_points_(k).y << ")" << std::endl;
            }
        std::cout << std::endl << std::endl;
        }
        std::cout << std::endl;
    #endif
    
        // Print Below the Line
        for(int c = 0; c < (n_cells * n_cells); c++){
            for(int i = 0; i < 4; i++){
                int const j = mirror_output(c, i);
                auto const p = mirror_allPoints(c, j);
                std::cout << "below line at cell " << c << ": (" << p.x << ", "<< p.y << ") "<< std::endl; 
            }
            std::cout << std::endl;
        }
*/
    }
    Kokkos::finalize();
    return EXIT_SUCCESS;
}

