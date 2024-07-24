#include "clippings.h"
#include "mesh.h"
#include "intersect.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include "timer.h"

int main(int argc, const char * argv[]){
    using namespace polyintersect;
    bool horizontal = true;
    int n_cells = std::atoi(argv[1]);
    double lengthPerAxis = std::atoi(argv[2]);
    // int n_cells = 120;            //Update: 120
    // double lengthPerAxis = 30;   //update: 30

    Mesh mesh(n_cells, lengthPerAxis);
    int n_nodes = n_cells + 1; 
    std::vector<std::array<Point, 2>> line(n_cells);

    // Horizontal ///////////////////////////////
    if(horizontal){
        // Start timer
        auto start = timer::now();
        
        // Parallel Region
        #pragma omp parallel
        {
            // Interface        
            double const h = lengthPerAxis / n_cells;

            //#pragma omp for
            for(int i = 0; i < line.size(); i++){   //UPDATE 
                double const val = h * (0.5 + i);
                line[i] = {{{lengthPerAxis + 2, val}, {-1.0, val}}};
            }

            // Clipping below for Every Cell 
            #pragma omp for
            for(int c = 0; c < (n_cells * n_cells); c++){
                int const k = static_cast<int>(c / n_cells);
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, false);
            }
        }

        // Print elapsed time
        std::cout << "Duration: " << timer::elapsed(start) << " ms." << std::endl;
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

    return EXIT_SUCCESS;
}
