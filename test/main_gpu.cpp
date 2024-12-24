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
        mesh.add_all_points(0, {0.0, 0.0}, mesh.points_);
        mesh.add_all_points(1, {0.5, 0.0}, mesh.points_);
        mesh.add_all_points(2, {0.5, 0.25}, mesh.points_);
        mesh.add_all_points(3, {0.25, 0.375}, mesh.points_);
        mesh.add_all_points(4, {0.0, 0.25}, mesh.points_);
        mesh.add_all_points(5, {0.875, 0.25}, mesh.points_);
        mesh.add_all_points(6, {1.0, 0.375}, mesh.points_);
        mesh.add_all_points(7, {0.875, 0.625}, mesh.points_);
        mesh.add_all_points(8, {0.5, 0.625}, mesh.points_);
        mesh.add_all_points(9, {0.5, 0.875}, mesh.points_);
        mesh.add_all_points(10, {0.875, 0.875}, mesh.points_);

        // Pentagon
        mesh.add_edge(0, 0, {0, 1}, mesh.cells_);
        mesh.add_edge(0, 1, {1, 2}, mesh.cells_);
        mesh.add_edge(0, 2, {2, 3}, mesh.cells_);
        mesh.add_edge(0, 3, {3, 4}, mesh.cells_);
        mesh.add_edge(0, 4, {4, 0}, mesh.cells_);

        // Triangle
        mesh.add_edge(1, 0, {1, 5}, mesh.cells_);
        mesh.add_edge(1, 1, {5, 2}, mesh.cells_);
        mesh.add_edge(1, 2, {2, 1}, mesh.cells_);

        // Hexagon
        mesh.add_edge(2, 0, {2, 5}, mesh.cells_);
        mesh.add_edge(2, 1, {5, 6}, mesh.cells_);
        mesh.add_edge(2, 2, {6, 7}, mesh.cells_);
        mesh.add_edge(2, 3, {7, 8}, mesh.cells_);
        mesh.add_edge(2, 4, {8, 3}, mesh.cells_);   
        mesh.add_edge(2, 5, {3, 2}, mesh.cells_);   

        // Square
        mesh.add_edge(3, 0, {8, 7}, mesh.cells_);
        mesh.add_edge(3, 1, {7, 10}, mesh.cells_);
        mesh.add_edge(3, 2, {10, 9}, mesh.cells_);
        mesh.add_edge(3, 3, {9, 8}, mesh.cells_);

	Kokkos::deep_copy(mesh.mirror_points_, mesh.points_);
        Kokkos::deep_copy(mesh.mirror_cells_, mesh.cells_);

        
        // Print Cells 
        for(int j = 0; j < total_cells; j++){   // Cell
            std::cout << "----- Cell " << j << " -----" << std::endl;
            for (int i = 0; i < max_edges_per_cell; i++) {      // Edge       
                std::cout << "Edge " << i << ": (" << mesh.mirror_cells_(j , i, 0) << ", ";
                std::cout << mesh.mirror_cells_(j, i, 1) << ") " << std::endl;

                if(j == 0 && i == 4 || j == 1 && i == 2 || j == 3 && i == 3){   
                    break;
                }
            }
	    std::cout <<  std::endl;
        }
        std::cout << std::endl;

        // Print Point Coordinates
        for (int j = 0; j < total_points; j++) {             // All Pointd
                std::cout << "Point " << j << ": (" << mesh.mirror_points_(j).x << ", " << mesh.mirror_points_(j).y << ")" << std::endl;
            }
     }
    Kokkos::finalize();
}


/*#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "intersect_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

int main(int argc, char * argv[]) {

    using namespace polyintersect;
    
    // Interface        
    int n_cells = std::atoi(argv[1]);
    double lengthPerAxis = std::atoi(argv[2]);
    double const h = lengthPerAxis / n_cells;

    Kokkos::initialize(argc, argv);
    {
        // aspect ratio: 4:1
        // 4 cells for lenghtperaxis = 1
        Mesh_Kokkos mesh(n_cells, lengthPerAxis);

        Kokkos::View<Line*> line("line", n_cells);
        Kokkos::View<Line*> interface("interface", n_cells * n_cells);
        Kokkos::View<int**> belowline("belowline", n_cells * n_cells, 4);
        Kokkos::View<int*> size("size", n_cells * n_cells);
        Kokkos::View<Point**> allPoints("allpoints", n_cells * n_cells, 6);

        // Max Threads
        int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;

        // Horizontal ///////////////////////////////
        // Start timer
        auto start = timer::now();
        
        Kokkos::parallel_for(n_cells, KOKKOS_LAMBDA(int i) {
            double const val = h * (0.5 + i);
            line(i) = {{lengthPerAxis + 2, val}, {-1.0, val}};
        });

        // Clipping below for Every Cell 
        Kokkos::parallel_for(n_cells * n_cells, KOKKOS_LAMBDA(int c) {            
            int const k = static_cast<int>(c / n_cells);
            interface(c) = intersect_cell_with_line(mesh.points_, mesh.cells_, c, line(k));
            clip_below_3(c, mesh.points_, mesh.cells_, interface(c),
                         belowline, size, allPoints);
        });

        auto const end = timer::elapsed(start);

        // CPU Copy 
        auto mirror_belowline = Kokkos::create_mirror_view(belowline);
        auto mirror_allPoints = Kokkos::create_mirror_view(allPoints);
        auto mirror_interface = Kokkos::create_mirror_view(interface);
        auto mirror_line = Kokkos::create_mirror_view(line);

        Kokkos::deep_copy(mesh.mirror_points_, mesh.points_);
        Kokkos::deep_copy(mesh.mirror_cells_, mesh.cells_);
        Kokkos::deep_copy(mirror_belowline, belowline);
        Kokkos::deep_copy(mirror_allPoints, allPoints);
        Kokkos::deep_copy(mirror_interface, interface);
        Kokkos::deep_copy(mirror_line, line);

        auto const end_including_copy = timer::elapsed(start);

        // Print elapsed time 
        std::cout << "Duration: " << end << " µs" << std::endl;
        std::cout << "Deep copy: " << end_including_copy << " µs" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 

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
                int const j = mirror_belowline(c, i);
                auto const p = mirror_allPoints(c, j);
                std::cout << "below line at cell " << c << ": (" << p.x << ", "<< p.y << ") "<< std::endl; 
            }
            std::cout << std::endl;
        }

    }

    Kokkos::finalize();
    return EXIT_SUCCESS;
}*/
