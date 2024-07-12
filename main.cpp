#include "Clippings.h"

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]){
    using namespace polyintersect;

    std::array<double, 2> V;
    std::vector<int> belowLine3;

    // Cell Nodes and Interface Declared
    std::vector<point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    //std::array<point, 2> interface = {{{1.0, 0.5}, {0.0, 0.5}}};
    std::array<point, 2> interface = {{{1.0, 0.5}, {0.5, 1}}};

    // Store all points in a single list
    std::vector<point> allPoints;
    allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());

    std::vector<point> allPoints2;
    allPoints2.insert(allPoints2.end(), nodes.begin(), nodes.end());
    allPoints2.insert(allPoints2.end(), interface.begin(), interface.end());

    // 1) Finding the Orientation of each Node WRT the Interface //////////////
    std::array<int, 4> signs1 = orientation_clip_1(nodes, interface, V);
    std::array<int, 6> signs2 = orientation_clip_2_3(allPoints2, interface, V);

    // 2) Clipping the values from below Using Different Methods //////////////
    auto const belowLine1 = clip_below_1(signs1);
    auto const belowLine2 = clip_below_2(nodes, signs2);

    ///////// Method 3 /////////////////////
    // Find Center Point and Sort from Least to Greatest Degree
    point centerPoint = center(allPoints);
    sorting(allPoints, centerPoint);
    
    // Store the Orientation of every node
    std::array<int, 6> signs3 = orientation_clip_2_3(allPoints, interface, V);

    // Clip below
    for(int p = 0;  p < allPoints.size(); p++){
        if(signs3[p] <= 0){
            belowLine3.emplace_back(p);
        }
    }

    // 3) Printing the Below values for each Method ///////////////////////////
    printf("\n////////// Different Methods of Clipping Below the Line ////////// \n\nMethod 1: \n");
    for(const auto &b : belowLine1){        // Method 1
        std::cout << "Edges: (" << b.first << ", " << b.second << ")" << std::endl;
    } 
    printf("\nMethod 2: \n");

    for(const auto &b : belowLine2){        // Method 2
        std::cout << "Coordinates: (" << allPoints2[b].x << ", " << allPoints2[b].y << ")" << std::endl;
    } 
    printf("\nMethod 3: \n");

    for(const auto &b : belowLine3){        // method 3 
        std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
    } 
    printf("\n");

    // New Portion //////////////////////////////////////////////////
    int cells = 4;
    mesh m = createMesh(cells);


}