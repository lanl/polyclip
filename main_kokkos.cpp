#include "clippings.h"
#include "clippings_kokkos.h"
#include "mesh.h"
#include "mesh_kokkos.h"
#include "intersect.h"
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

/*        int n_nodes = n_cells + 1; 
        std::vector<std::array<Point, 2>> line(n_cells);


        // Horizontal ///////////////////////////////
        if(horizontal){
            // Start timer
            std::vector<double> duration(1, 0.);
            auto start = timer::now();
            
            // Parallel Region
            #pragma omp parallel
            {
                // Interface        
                double const h = lengthPerAxis / n_cells;

                #pragma omp for
                for(int i = 0; i < line.size(); i++){
                    double const val = h * (0.5 + i);
                    line[i] = {{{7.0, val}, {-1.0, val}}};
                }

                // Clipping below for Every Cell 
                #pragma omp for
                for(int c = 0; c < (n_cells * n_cells); c++){
                    int const k = static_cast<int>(c / n_cells);
                    auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                    auto const belowLine = clip_below_3(c, mesh, interface, true);
                }
            }
            // Stop timer 
            duration[0] = timer::elapsed(start);

            // Print elapsed time
            std::cout << "Duration: " << duration[0] << " ms." << std::endl;
        }

        // Vertical /////////////////////////////////
        else{
            // Interface ////////////////////////
            for(int i = 0; i < line.size(); i++){
                int x1 = i;
                int x2 = x1 + 1;
                double val = (mesh.points_[x1].x + mesh.points_[x2].x) / 2;

                line[i] = {{{val, -1.0}, {val, 2.0}}};
            }

            // Clipping below for Every Cell ////
            for(int k = 0; k < line.size(); k++){
                for(int c = k; c < (n_cells * n_cells); (c += line.size())){
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, true);
                }
            }
        }
    }
*/
    }
    Kokkos::finalize();
    return EXIT_SUCCESS;
}
