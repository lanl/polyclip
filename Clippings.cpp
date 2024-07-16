#include "Clippings.h"

/*
    Code Description:
        - Calculates the normal vector of the interface.
        - Calculates the direction vector with respect to (WRT) the normal vector.
        - Calculates the dot product of the normal and direction vector.
            - This will indicate the sign of the node. 
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


// Below the Line Using Edges ///////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> polyintersect::clip_below_1(std::vector<Point> const& nodes, 
                                                             std::array<Point, 2> const& interface, 
                                                             bool print){

    std::array<double, 2> V;
    std::vector<std::pair<int, int>> belowline;
    std::map<std::pair<int, int>, int > intersectPoints; 
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,3}, {3,0}}; 

    // Mapping the intersection Points and providing an ID
    intersectPoints[{1,2}] = 4;      // Pi = 4, since allPoints[4] = {1, 0.5}
    //intersectPoints[{3,0}] = 5;      // Pj = 5, since allPoints[5] = {0, 0.5}
    intersectPoints[{2,3}] = 5;

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



// Below the Line Pointing to the Next Node /////////////////////////////////////////////
std::vector<int> polyintersect::clip_below_2(std::vector<Point> const& nodes, 
                                             std::array<Point, 2> const& interface, 
                                             bool print){

    std::map<std::pair<int, int>, int > intersectPoints; 
    std::map<int, std::pair<int, int>> reverse_map;
    std::vector<int> belowline;
    std::array<double, 2> V;
    int n = nodes.size();

    std::vector<Point> allPoints;
    allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());      

    // Mappings
    intersectPoints[{1,2}] = 4;     
    //intersectPoints[{3,0}] = 5; 
    intersectPoints[{2,3}] = 5;     
    reverse_map[4] = {1,2};
    //reverse_map[5] = {3,0};
    reverse_map[5] = {2,3};

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
/////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> polyintersect::clip_below_3(std::vector<Point> const& nodes, 
                                             std::array<Point, 2> const& interface,
                                             bool print) {

    std::array<double, 2> V;
    // Store all Points in a single list
    std::vector<Point> allPoints;
    allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
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



