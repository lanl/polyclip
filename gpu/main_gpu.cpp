#include "clippings_gpu.h"
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

        // Print elapsed time 
        std::cout << "Duration: " << timer::elapsed(start) << " ms" << std::endl;
        std::cout << "Max Threads: " << max_threads << std::endl << std::endl; 

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
}
