
#include "SingleCell.h"

/*
New Goals For the Code: 
    1) Check to see when the signs are different
        - if they are different then set i = Pi and j = Pj
    2) Check the signs are equal to 0 (on the line)
        - if they are then i = Pj (j) and j = reverse map
    3) Below line prints the coordinates using their ID's

    NEW:
        - I included the a new sorting algorithm 
        - Used the same algorithm as Sorting.cpp
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

// Below the Line ////////////////////////////////////////////////////////////////////////
std::vector<int> clip_below(std::vector<point> const& nodes, 
                            std::array<int, 6> const& sign, 
                            std::map<std::pair<int, int>, int> const& intersectPoints,
                            std::map<int, std::pair<int, int>> const& reverse_map){

    std::vector<int> belowline;
    int n = nodes.size(), k = intersectPoints.size();
    std::cout << "SIZE: " << k << std::endl;

    int i = 0, j = 1;                                                                                                                               
    do{
        belowline.emplace_back(i);
        //std::cout << "Iterate: (" << i << ", " << j << ")" << std::endl; 

        // Checking all possibilities 
        if(sign[i] == sign[j] && sign[i] < 0){              // Below the Line
            i = j;
            j = (j + 1) % n;
        }
        else if(sign[i] < 0){                               // Detecting intersecting points
            i = intersectPoints.at({i,j});
            j = i + 1;
        }
        else if(sign[i] == sign[j] && sign[i] == 0){        // Detecting on the line
            i = j;  
            j = reverse_map.at(i).second; 
        }

    }
    while(j != 0);
    belowline.emplace_back(i);

    return belowline;
}

// NEW PORTION //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print Current and Updated Sort ///////////////////////////////////////////////////////
void printPoints(const std::vector<point> &nodes){
    std::cout << "Sorted Values: ";
    for(const auto &p : nodes){
        std::cout << "(" << p.x << ", " << p.y << ") ";  
    }
    std::cout << std::endl;
}

// Sort in Counter Clockwise manner /////////////////////////////////////////////////////
void sorting(std::vector<point> &nodes, point center){
    std::sort(nodes.begin(), nodes.end(), [&center](const point& a, const point& b){
        double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));  
        double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));  

        return a1 < a2;   
    });
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find the Center Coordinate ///////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, const char * argv[]){
    int index = 0;
    std::array<int, 6> signs;
    std::array<double, 2> V;
    
    // Cell nodes, Interface, and Map of Pi and Pj
    std::vector<point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::array<point, 2> interface = {{{1.0, 0.5}, {0.0, 0.5}}};
    std::map<std::pair<int, int>, int> intersectPoints; 
    std::vector<int> belowLine;
    //std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,3}, {3,0}}; // intiallized with edges
   
    // Store all points in a single list
    std::vector<point> allPoints;
    allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());

    // Reverse mape of the intersecting points
    std::map<int, std::pair<int, int>> reverse_map;
    reverse_map[4] = {1,2};
    reverse_map[5] = {3,0};

    // 1) Loop around the cell coordinates ////////////////////////////////////

    ////// 1a) Map out Pi and Pj
    intersectPoints[{1,2}] = 4;      // Pi = 4, since allPoints[4] = {1, 0.5}
    intersectPoints[{3,0}] = 5;      // Pj = 5, since allPoints[5] = {0, 0.5}

    // 2) Deduce the normal vector of the cutting line ////////////////////////
    std::array <double, 2> normal = normVec(interface);
    printNorm(normal);
    printf("\n");

    // NEW PORTION //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Find the center of the cell
    point centerPoint = center(allPoints);
    std::cout << "Center Coordinates: (" << centerPoint.x << ", " << centerPoint.y << ") " << std::endl; 
    std::cout << std::endl;

    // Sort all points
    sorting(allPoints, centerPoint);
    printPoints(allPoints);
    std::cout << std::endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    // 3) Deduce the sign of every node using the dot product /////////////////
    for(const auto& point : allPoints){
        // Vector of Node
        V = pointVec(point, interface);
       
        // Dot Product of normal and node vector
        double dp = dotProduct(normal, V);
        printf("Dot Product of (%.1lf, %.1lf) and (%.1lf, %.1lf): %.2lf\n", V[0], V[1], normal[0], normal[1], dp);
       
        // Convection of placement with respect to the line
        //signs[index] = (dp < 0) ? -1 : 1;       // Classy :)
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
    
    // Printing the orientation
    printf("\n");
    for(int i = 0; i < index; ++i){
        printf("Orientation %d: %d \n", i+1, signs[i]);
    }

    // 4) Clipping the values from below //////////////////////////////////////
    //auto const belowLine = clip_below(nodes, signs, intersectPoints, reverse_map); // NO NEED FOR THIS FUNCTION

    // NEW PORTION //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Storing the element number of the signs below and on the line
    std::cout << std::endl;
    for(int p = 0;  p < allPoints.size(); p++){
        if(signs[p] <= 0){
            belowLine.emplace_back(p);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Printing the Below values
    printf("\nNodes of the Line and Below the Line: \n");
    for(const auto &b : belowLine){ 
        std::cout << "(" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
    } 
    printf("\n");


}
