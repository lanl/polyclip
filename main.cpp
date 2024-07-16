#include "Clippings.h"
#include "Mesh.h"
#include "intersect.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

    using namespace polyintersect;
    // Create One Cell
    int n_cells = 1;
    Mesh mesh(n_cells);

    std::map<std::pair<int, int>, int> mapping;

    // Cell Nodes and Interface Declared
//    std::array<Point, 2> interface = {{{1.0, 0.5}, {0.5, 1}}}; // ccw
    std::array<Point, 2> line = {{{2.0, 0.4}, {-1.0, 0.2}}}; // ccw
    auto const interface = intersect_cell_with_line(mesh, 0, line, mapping);

    // Clipping the values from below Using Different Methods 
    auto const belowLine1 = clip_below_1(0, mesh, interface, true, mapping);
    auto const belowLine2 = clip_below_2(0, mesh, interface, true, mapping);
    auto const belowLine3 = clip_below_3(0, mesh, interface, true);


    return EXIT_SUCCESS;
}