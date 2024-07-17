#include "clippings.h"
#include "mesh.h"
#include "intersect.h"
#include <Kokkos_Core.hpp>

int main(int argc, const char * argv[]){
    using namespace polyintersect;
    bool horizontal = false;
    bool four = false;

    // 4 by 4 Case //////////////////////////////
    if(four){
        int n_cells = 4;
        Mesh mesh(n_cells);
        std::vector<std::array<Point, 2>> line(4);

        if(horizontal){
            // Interface //////////////////////// 
            line[0] = {{{2.0, 0.10}, {-1.0, 0.10}}};    // ccw
            line[1] = {{{2.0, 0.4}, {-1.0, 0.4}}};      // ccw
            line[2] = {{{2.0, 0.6}, {-1.0, 0.6}}};      // ccw
            line[3] = {{{2.0, 0.85}, {-1.0, 0.85}}};    // ccw 

            // Clipping below for Every Cell ////
            for(int c = 0; c < (n_cells * n_cells); c++){
                int const k = static_cast<int>(c / n_cells);
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, true);
            }
        }
        else{
            // Interface ////////////////////////
            line[0] = {{{.1, -1}, {.1, 2}}};    // ccw
            line[1] = {{{.4, -1}, {.4, 2}}};    // ccw
            line[2] = {{{.6, -1}, {.6, 2}}};    // ccw
            line[3] = {{{.8, -1}, {.8, 2}}};    // ccw

            // Clipping below for Every Cell ////
            for(int k = 0; k < line.size(); k++){
                for(int c = k; c < (n_cells * n_cells); (c += line.size())){
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, true);
                }
            }
        }
    }
    
    // 16 by 16 Case ////////////////////////////
    else{
        int n_cells = 16;
        Mesh mesh(n_cells);
        std::vector<std::array<Point, 2>> line(n_cells);

        if(horizontal){
            // Interface ////////////////////////
            line[0] = {{{2.0, 0.03}, {-1.0, 0.03}}};    // ccw
            line[1] = {{{2.0, 0.09}, {-1.0, 0.09}}};    // ccw
            line[2] = {{{2.0, 0.14}, {-1.0, 0.14}}};    // ccw
            line[3] = {{{2.0, 0.2}, {-1.0, 0.2}}};      // ccw
            line[4] = {{{2.0, 0.29}, {-1.0, 0.29}}};    // ccw
            line[5] = {{{2.0, 0.35}, {-1.0, 0.35}}};    // ccw
            line[6] = {{{2.0, 0.4}, {-1.0, 0.4}}};      // ccw
            line[7] = {{{2.0, 0.45}, {-1.0, 0.45}}};    // ccw

            line[8] = {{{2.0, 0.53}, {-1.0, 0.53}}};    // ccw
            line[9] = {{{2.0, 0.6}, {-1.0, 0.6}}};      // ccw
            line[10] = {{{2.0, 0.65}, {-1.0, 0.65}}};   // ccw
            line[11] = {{{2.0, 0.72}, {-1.0, 0.72}}};   // ccw
            line[12] = {{{2.0, 0.79}, {-1.0, 0.79}}};   // ccw
            line[13] = {{{2.0, 0.85}, {-1.0, 0.85}}};   // ccw
            line[14] = {{{2.0, 0.9}, {-1.0, 0.9}}};     // ccw
            line[15] = {{{2.0, 0.95}, {-1.0, 0.95}}};   // ccw

            // Clipping below for Every Cell ////
            for(int c = 0; c < (n_cells * n_cells); c++){
                int const k = static_cast<int>(c / n_cells);
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, true);
            }

        }
        else{
            // Interface ////////////////////////
            line[0] = {{{0.03, -1}, {0.03, 2}}};      // ccw
            line[1] = {{{0.09, -1}, {0.09, 2}}};      // ccw
            line[2] = {{{0.14, -1}, {0.14, 2}}};      // ccw
            line[3] = {{{0.2, -1}, {0.2, 2}}};        // ccw
            line[4] = {{{0.29, -1}, {0.29, 2}}};      // ccwß
            line[5] = {{{0.35, -1}, {0.35, 2}}};      // ccw
            line[6] = {{{0.4, -1}, {0.4, 2}}};        // ccw
            line[7] = {{{0.45, -1}, {0.45, 2}}};      // ccw

            line[8] = {{{0.53, -1}, {0.53, 2}}};      // ccw
            line[9] = {{{0.6, -1}, {0.6, 2}}};        // ccw
            line[10] = {{{0.65, -1}, {0.65, 2}}};     // ccw
            line[11] = {{{0.72, -1}, {0.72, 2}}};     // ccw
            line[12] = {{{0.79, -1}, {0.79, 2}}};     // ccw
            line[13] = {{{0.85, -1}, {0.85, 2}}};     // ccw
            line[14] = {{{0.9, -1}, {0.9, 2}}};       // ccw
            line[15] = {{{0.95, -1}, {0.95, 2}}};     // ccw


            // Clipping below for Every Cell ////
            for(int k = 0; k < line.size(); k++){
                for(int c = k; c < (n_cells * n_cells); (c += line.size())){
                auto const interface = intersect_cell_with_line(mesh, c, line[k]);
                auto const belowLine = clip_below_3(c, mesh, interface, true);
                }
            }
        }

    }

    return EXIT_SUCCESS;
}
