#include "Clippings.h"
#include "Mesh.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){

    using namespace polyintersect;
    // Cell Nodes and Interface Declared
    std::vector<Point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::array<Point, 2> interface = {{{1.0, 0.5}, {0.5, 1}}};

    // // Store all points in a single list
    // std::vector<point> allPoints;
    // allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
    // allPoints.insert(allPoints.end(), interface.begin(), interface.end());

    // std::vector<point> allPoints2;
    // allPoints2.insert(allPoints2.end(), nodes.begin(), nodes.end());
    // allPoints2.insert(allPoints2.end(), interface.begin(), interface.end());

    // // 1) Finding the Orientation of each Node WRT the Interface //////////////
    // std::array<int, 4> signs1 = orientation_clip_1(nodes, interface, V);
    // std::array<int, 6> signs2 = orientation_clip_2_3(allPoints2, interface, V);

    // 2) Clipping the values from below Using Different Methods //////////////
    auto const belowLine1 = clip_below_1(nodes, interface, true);
    auto const belowLine2 = clip_below_2(nodes, interface, true);
    auto const belowLine3 = clip_below_3(nodes, interface, true);

    // New Portion //////////////////////////////////////////////////
    int cells = 4;
    Mesh m(cells);

    return EXIT_SUCCESS;
}