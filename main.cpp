#include "Clippings.h"
#include "Mesh.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

    using namespace polyintersect;
    // Create One Cell
    int cells = 1;
    Mesh m(cells);

    // Cell Nodes and Interface Declared
    std::vector<Point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::array<Point, 2> interface = {{{1.0, 0.5}, {0.5, 1}}};

    // Clipping the values from below Using Different Methods 
    auto const belowLine1 = clip_below_1(0, m, interface, true);
    auto const belowLine2 = clip_below_2(0, m, interface, true);
    auto const belowLine3 = clip_below_3(0, m, interface, true);


    return EXIT_SUCCESS;
}