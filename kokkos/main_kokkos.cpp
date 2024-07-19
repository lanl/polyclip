#include "clippings.h"
#include "clippings_kokkos.h"
#include "mesh.h"
#include "mesh_kokkos.h"
// #include "intersect.h"
#include "intersect_kokkos.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include "timer.h"

int main(int argc, char * argv[]) {
    
    Kokkos::initialize(argc, argv);
    {
        using namespace polyintersect;
        bool horizontal = true;

        int n_cells = 4;
        double lengthPerAxis = 1.0;
        //Mesh mesh(n_cells, lengthPerAxis);
        Mesh_Kokkos mesh(n_cells, lengthPerAxis);

        int n_nodes = n_cells + 1; 
        Kokkos::View<std::array<Point, 2>*> line;
        Kokkos::resize(line, n_cells);

        // Horizontal ///////////////////////////////
        // Start timer
        std::vector<double> duration(1, 0.);
        auto start = timer::now();
        
        // Interface        
        double const h = lengthPerAxis / n_cells;

        Kokkos::parallel_for(n_cells, KOKKOS_LAMBDA(int i) {
            double const val = h * (0.5 + i);
            line(i) = {{{7.0, val}, {-1.0, val}}};
        });

        // Clipping below for Every Cell 
        Kokkos::parallel_for(n_cells * n_cells, KOKKOS_LAMBDA(int c) {            
            int const k = static_cast<int>(c / n_cells);
            auto const interface = intersect_cell_with_line(mesh, c, line[k]);
            // auto const belowLine = clip_below_3(c, mesh, interface, true);
        });
        
        // Stop timer 
        duration[0] = timer::elapsed(start);

        // Print elapsed time
        std::cout << "Duration: " << duration[0] << " ms." << std::endl;
    }

    Kokkos::finalize();
    return EXIT_SUCCESS;
}
