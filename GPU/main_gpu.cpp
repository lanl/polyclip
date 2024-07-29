#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "intersect_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "timer.h"

int main(int argc, char * argv[]) {
    
    Kokkos::initialize(argc, argv);
    {
        using namespace polyintersect;
        bool horizontal = true;
        // aspect ratio: 4:1
        // 4 cells for lenghtperaxis = 1
        int n_cells = std::atoi(argv[1]);
        double lengthPerAxis = std::atoi(argv[2]);
        Mesh_Kokkos mesh(n_cells, lengthPerAxis);

        int n_nodes = n_cells + 1; 
        Kokkos::View<Line*> line;
        //Kokkos::resize(line, n_cells);

        // Horizontal ///////////////////////////////
        // Start timer
        auto start = timer::now();
        
        // Interface        
        double const h = lengthPerAxis / n_cells;

        Kokkos::parallel_for(n_cells, KOKKOS_LAMBDA(int i) {
            double const val = h * (0.5 + i);
            line(i).a = {lengthPerAxis + 2, val};
            line(i).b = {-1.0, val};
        });

        // Clipping below for Every Cell 
        Kokkos::parallel_for(n_cells * n_cells, KOKKOS_LAMBDA(int c) {            
            int const k = static_cast<int>(c / n_cells);
            //auto const interface = intersect_cell_with_line(mesh, c, line[k]);
            //auto const belowLine = clip_below_3(c, mesh, interface);
        });

        // Print elapsed time 
        std::cout << "Duration: " << timer::elapsed(start) << " ms" << std::endl;
    }

    Kokkos::finalize();
    return EXIT_SUCCESS;
}
