#include "Mesh.h"

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

            2) Method 2: Clips by pointing to the next node based on the sign 
               comparison.
                * But it uses a map and a reverse map.

            3) Method 3: Finds the center point of all the nodes, calculates 
               the degree of every node WTR to the center point, and sorts based 
               on smallest to biggest degree. Uses the sorted value of all points to 
               compare the signs.

*/

// Finding the normal vector between 2 points ///////////////////////////////////////////
std::array<double, 2> normVec(std::array<point, 2> const& inter){
   // Direction vec
   double dx = inter[1].x - inter[0].x;     // x2 - x1
   double dy = inter[1].y - inter[0].y;     // y2 - y1
   
   // Normal vec
   std::array <double, 2> normal = {dy, -dx};
   return normal;
}

// Finding the dot product of the array and vector //////////////////////////////////////
double dotProduct(std::array<double, 2> const& v , std::array<double, 2> &n){
   double product = (v[0] * n[0]) + (v[1] * n[1]);
   return product;
}

// Print the normal vector //////////////////////////////////////////////////////////////
void printNorm(std::array<double, 2> const& normal){
   printf("Normal: ");
   for(int i = 0; i < 2; ++i){
       printf("%.1lf ", normal[i]);
   }
   printf("\n");
}

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array<double, 2> pointVec(const point &p, std::array<point, 2> const& inter){
   double dx = p.x - inter[0].x;
   double dy = p.y - inter[0].y;
   
   std::array <double, 2> point = {dx, dy};
   return point;
}

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
std::array<int, 6> orientation_clip_2_3(std::vector<point> const allPoints, std::array<point, 2> const interface, 
                                      std::array<double, 2> V){
    
    // Deduce the normal vector of the cutting line 
    std::array <double, 2> normal = normVec(interface);
    std::array<int, 6> signs;
    int index = 0;
    double dp;

    for(const auto& point : allPoints){
        // Vector of Node
        V = pointVec(point, interface);

        // Dot Product of normal and node vector
        dp = dotProduct(normal, V);

        // Convection of placement with respect to the line
        if(dp < 0){         // Below the line
            signs[index] = -1;
        }
        else if(dp > 0){    // Above the line
            signs[index] = 1;
        }
        else{               // On the line
            signs[index] = 0;
        }
        index++;
    }

    return signs;
}


// Below the Line Using Edges ///////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> clip_below_1(std::array<int, 4> const& sign){

    std::vector<std::pair<int, int>> belowline;
    std::map<std::pair<int, int>, int > intersectPoints; 
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,3}, {3,0}}; 

    // Mapping the intersection points and providing an ID
    intersectPoints[{1,2}] = 4;      // Pi = 4, since allPoints[4] = {1, 0.5}
    //intersectPoints[{3,0}] = 5;      // Pj = 5, since allPoints[5] = {0, 0.5}
    intersectPoints[{2,3}] = 5;

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
    return belowline;
}

// Orientation of Every Node for Method 1 ///////
std::array<int, 4> orientation_clip_1(std::vector<point> const nodes, std::array<point, 2> const interface, 
                                      std::array<double, 2> V){
    
    // Deduce the normal vector of the cutting line 
    std::array <double, 2> normal = normVec(interface);
    std::array<int, 4> signs;
    int index = 0;
    double dp;

    for(const auto& point : nodes){
        // Vector of Node
        V = pointVec(point, interface);

        // Dot Product of normal and node vector
        dp = dotProduct(normal, V);

        // Convection of placement with respect to the line
        signs[index] = (dp < 0) ? -1 : 1;       // Classy :)
        index++;
    }

    return signs;
}
/////////////////////////////////////////////////////////////////////////////////////////

// Below the Line Pointing to the Next Node /////////////////////////////////////////////
std::vector<int> clip_below_2(std::vector<point> const& nodes, 
                            std::array<int, 6> const& sign){

    std::map<std::pair<int, int>, int > intersectPoints; 
    std::map<int, std::pair<int, int>> reverse_map;
    std::vector<int> belowline;
    int n = nodes.size();

    // Mappings
    intersectPoints[{1,2}] = 4;     
    //intersectPoints[{3,0}] = 5; 
    intersectPoints[{2,3}] = 5;     
    reverse_map[4] = {1,2};
    //reverse_map[5] = {3,0};
    reverse_map[5] = {2,3};

    int i = 0, j = 1;                                                                                                                               
    do{
        belowline.emplace_back(i);

        // Checking all possibilities 
        if(sign[i] == sign[j] && sign[i] < 0 || sign[j] < 0){   // Below the Line
            i = j;
            j = (j + 1) % n;
        }
        else if(sign[i] < 0){                                   // Detecting intersecting points
            i = intersectPoints.at({i,j});
            j = i + 1;
        }
        else if(sign[i] == sign[j] && sign[i] == 0){            // Detecting on the line
            i = j;  
            j = reverse_map.at(i).second; 
        }

    }
    while(j != 0);
    belowline.emplace_back(i);

    return belowline;
}
/////////////////////////////////////////////////////////////////////////////////////////


// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void sorting(std::vector<point> &nodes, point center){
    std::sort(nodes.begin(), nodes.end(), [&center](const point& a, const point& b){        
        double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));  
        double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));  
        return a1 < a2;   
    });
}

// Find the Center Coordinate ///////////////////
point center(std::vector<point> &nodes){
    std::vector<point> result;
    double sumX = 0, sumY = 0;

    // Add up all the coordinates /////
    for(const auto &p : nodes){
        sumX += p.x;
        sumY += p.y;
    }

    // Store middle coordinates ///////  
    return {(sumX /= nodes.size()), (sumY /= nodes.size())};
}

