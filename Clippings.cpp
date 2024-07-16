#include "Clippings.h"
#include "Mesh.h"

/*
    Code Description:
        - This code contains 3 different clipping methods.
            1) Method 1: Clips using edges, compares the sign of every node,
               and stores as edges
                * But skips the the interface edge and uses maps.

            2) Method 2: Clips by Pointing to the next node based on the sign 
               comparison.
                * But it uses a map and a reverse map.

            3) Method 3: Finds the center Point of all the nodes, calculates 
               the degree of every node WTR to the center Point, and sorts based 
               on smallest to biggest degree. Uses the sorted value of all Points to 
               compare the signs.

*/


// Method 1: Clipping Using Edges ///////////////////////////////////////////////////////
std::vector<std::pair<int, int>> polyintersect::clip_below_1(int cell, Mesh const &mesh, 
                                                             std::array<Point, 2> const& interface,
                                                             bool print,
                                                             std::map<std::pair<int, int>, int> const& intersectPoints){

    std::array<double, 2> V;
    std::vector<std::pair<int, int>> belowline;
//    std::map<std::pair<int, int>, int > intersectPoints;
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,3}, {3,0}}; 
    
    // Coordinates of the Cell Currently On
    std::vector<Point> nodes;
    for(const auto &c : mesh.cells_[cell]){
        nodes.emplace_back(mesh.points_[c]);
    }

//    // Mapping the intersection Points and providing an ID
//    intersectPoints[{1,2}] = 4;      // Pi = 4, since allPoints[4] = {1, 0.5}
//    intersectPoints[{3,0}] = 5;      // Pj = 5, since allPoints[5] = {0, 0.5}
////    intersectPoints[{2,3}] = 5;

    for (auto [key, val] : intersectPoints) {
      std::cout << "map("<< key.first << ", "<< key.second << ") = "<< val << std::endl;
    }

    std::array<int, 4> const sign = orientation_clip_1(nodes, interface, V);

    int i, j;                                                                                                                               
    for(const auto& edge : edges){
        i = edge.first;
        j = edge.second;

        // Checking all possibilities 
        if(sign[i] == sign[j] && sign[i] < 0){ 
            belowline.emplace_back(i,j);    
        }
        else if(sign[i] < 0){
            belowline.emplace_back(i, intersectPoints.at({i,j}));
        }
        else if(sign[j] < 0){
            belowline.emplace_back(intersectPoints.at({i,j}), j);  // Fix the Flip
        }
    }

    if (print) {
        std::cout << "\nMethod 1: "<< std::endl;
        for(const auto &b : belowline) {        // Method 1
            std::cout << "Edges: (" << b.first << ", " << b.second << ")" << std::endl;
        }
    }

    return belowline;
}

// Method 2 : Clipping by Pointing to the Next Node /////////////////////////////////////
std::vector<int> polyintersect::clip_below_2(int cell, Mesh const &mesh, 
                                             std::array<Point, 2> const& interface, 
                                             bool print,
                                             std::map<std::pair<int, int>, int> const& intersectPoints){

//    std::map<std::pair<int, int>, int > intersectPoints;
    std::map<int, std::pair<int, int>> reverse_map;
    std::vector<int> belowline;
    std::array<double, 2> V{};
    int n = 0;

    // Store all Points in a single list
    std::vector<Point> allPoints;
    for(const auto &c : mesh.cells_[cell]){
        allPoints.emplace_back(mesh.points_[c]);
        n++;
    }
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());     

//    // Mappings
//    intersectPoints[{1,2}] = 4;
//    intersectPoints[{3,0}] = 5;
//    intersectPoints[{2,3}] = 5;
//    reverse_map[4] = {1,2};
//    reverse_map[5] = {3,0};
//    reverse_map[5] = {2,3};
    for (auto [key,val] : intersectPoints) {
      reverse_map[val] = key;
    }


    std::array<int, 6> const sign = orientation_clip_2_3(allPoints, interface, V);

    int i = 0, j = 1;                                                                                                                               
    do{
        belowline.emplace_back(i);

        // Checking all possibilities 
        if(sign[i] == sign[j] && sign[i] < 0 || sign[j] < 0){   // Below the Line
            i = j;
            j = (j + 1) % n;
        }
        else if(sign[i] < 0){                                   // Detecting intersecting Points
            i = intersectPoints.at({i,j});
            j = i + 1;
        }
        else if(sign[i] == sign[j] && sign[i] == 0){            // Detecting on the line
            i = j;  
            j = reverse_map.at(i).second; 
        }
    } while (j != 0);
    belowline.emplace_back(i);

    if (print) {
        std::cout << "\nMethod 2: "<< std::endl;
        for(const auto &b : belowline){        // Method 2
            std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
        } 
    }
    
    return belowline;
}

// Method 3: Clipping Using Degrees /////////////////////////////////////////////////////
std::vector<int> polyintersect::clip_below_3(int cell, Mesh const & mesh,
                                             std::array<Point, 2> const& interface,
                                             bool print) {
    // Initialize the point vetor
    std::array<double, 2> V{};

    // Store all Points in a single list
    std::vector<Point> allPoints;
    for(const auto &c : mesh.cells_[cell]){
        allPoints.emplace_back(mesh.points_[c]);
    }
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());                         

    // Find Center Point and Sort from Least to Greatest Degree
    Point centerPoint = center(allPoints);
    sorting(allPoints, centerPoint);
    
    // Store the Orientation of every node
    std::array<int, 6> const sign = orientation_clip_2_3(allPoints, interface, V);
    std::vector<int> belowline;

    // Clip below
    for(int p = 0;  p < allPoints.size(); p++){
        if(sign[p] <= 0){
            belowline.emplace_back(p);
        }
    }

    if (print) {
        std::cout << "\nMethod 3: "<< std::endl;
        for(const auto &b : belowline){        // method 3 
            std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
        } 
    }
    return belowline;
}



