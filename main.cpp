#include "Clippings.h"
#include "Mesh.h"
#include "intersect.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

    using namespace polyintersect;
    // Create One Cell
    int cells = 1;
    Mesh m(cells);

    std::map<std::pair<int, int>, int> mapping;

    // Cell Nodes and Interface Declared
    std::vector<Point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
//    std::array<Point, 2> interface = {{{1.0, 0.5}, {0.5, 1}}}; // ccw
    std::array<Point, 2> line = {{{2.0, 0.4}, {-1.0, 0.4}}};
    auto const interface = intersect_cell_with_line(m, 0, line, mapping);

    // Clipping the values from below Using Different Methods 
    auto const belowLine1 = clip_below_1(0, m, interface, true, mapping);
    auto const belowLine2 = clip_below_2(0, m, interface, true, mapping);
    auto const belowLine3 = clip_below_3(0, m, interface, true);


    return EXIT_SUCCESS;
}