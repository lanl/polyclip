#include "clippings.h"
#include "mesh.h"
#include "intersect.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

    using namespace polyintersect;
    // 4 by 4 Mesh Using Horizonatal interface /////////////////////////////////////
    int n_cells = 4;
    Mesh mesh(n_cells);

    std::map<std::pair<int, int>, int> mapping;
    std::cout << std::endl << "////////////////// Horizontal ///////////////////" << std::endl;

    // Interface
    std::array<Point, 2> line1 = {{{2.0, 0.10}, {-1.0, 0.10}}}; // ccw
    std::array<Point, 2> line2 = {{{2.0, 0.4}, {-1.0, 0.4}}}; // ccw
    std::array<Point, 2> line3 = {{{2.0, 0.6}, {-1.0, 0.6}}}; // ccw
    std::array<Point, 2> line4 = {{{2.0, 0.85}, {-1.0, 0.85}}}; // ccw

    for(int i = 0; i < 4; i++){
        auto const interface1 = intersect_cell_with_line(mesh, i, line1, mapping);
        auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
    }
    for(int i = 4; i < 8; i++){
        auto const interface2 = intersect_cell_with_line(mesh, i, line2, mapping);
        auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
    }
    for(int i = 8; i < 12; i++){
        auto const interface3 = intersect_cell_with_line(mesh, i, line3, mapping);
        auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
    }
    for(int i = 12; i < 16; i++){
        auto const interface4 = intersect_cell_with_line(mesh, i, line4, mapping);
        auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
    }
    ////////////////////////////////////////////////////////////////////////////////

    // 4 by 4 Mesh Using Vertical interface ////////////////////////////////////////
    std::cout << std::endl << "////////////////// Vertical ///////////////////" << std::endl;
    // Interface
    line1 = {{{.1, -1}, {.1, 2}}}; // ccw
    line2 = {{{.4, -1}, {.4, 2}}}; // ccw
    line3 = {{{.6, -1}, {.6, 2}}}; // ccw
    line4 = {{{.8, -1}, {.8, 2}}}; // ccw

    for(int i = 0; i < 13; i++){
        if(i % 4 == 0){
            auto const interface1 = intersect_cell_with_line(mesh, i, line1, mapping);
            auto const belowLine3 = clip_below_3(i, mesh, interface1, true);
        }
    }
    for(int i = 1; i < 14; i++){
        if(i % 4 == 1){
            auto const interface2 = intersect_cell_with_line(mesh, i, line2, mapping);
            auto const belowLine3 = clip_below_3(i, mesh, interface2, true);
        }
    }
    for(int i = 1; i < 15; i++){
        if(i % 4 == 2){
            auto const interface3 = intersect_cell_with_line(mesh, i, line3, mapping);
            auto const belowLine3 = clip_below_3(i, mesh, interface3, true);
        }
    }
    for(int i = 3; i < 16; i++){
        if(i % 4 == 3){
            auto const interface4 = intersect_cell_with_line(mesh, i, line4, mapping);
            auto const belowLine3 = clip_below_3(i, mesh, interface4, true);
        }
    }
    ////////////////////////////////////////////////////////////////////////////////

    



    return EXIT_SUCCESS;
}