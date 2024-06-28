
#include "SingleCell.h"
#include <map>
#include <utility>

/*
New Goals For the Code:
    1) Store the convention of each node (check)
    2) Map out Pi and Pj (check) 
    3) Now check if its below the line (Kinda check)
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
std::vector<std::pair<int, int>> clip_below(std::vector<std::pair<int,int>> const& edges, 
                                            std::array<int, 4> const& sign, 
                                            std::map<std::pair<int, int>, int> const& intersectPoints){

    std::vector<std::pair<int, int>> belowline;

    int i, j, loop = 0;                                                                                                                               
    for(const auto& edge : edges){
        i = edge.first;
        j = edge.second;
        std::cout << "Iterate: (" << i << ", " << j << ")" << std::endl; 

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

        loop++;
    }
    return belowline;
}


int main(int argc, const char * argv[]){
    int index = 0;
    //std::array<double, 2> Pi = 12, Pj = {0, 0.5}; //this used to only be an integer
    std::array<int, 4> signs;
    std::array<double, 2> V;
    
    // Cell nodes, Interface, and Map of Pi and Pj
    std::vector<point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::array<point, 2> interface = {{{1.0, 0.5}, {0.0, 0.5}}};
    std::map<std::pair<int, int>, int > intersectPoints; 
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,3}, {3,0}}; // intiallized with edges
   
    // store all points in a single list
    std::vector<point> allPoints(nodes.size() + interface.size());
    allPoints.insert(allPoints.end(), nodes.begin(), nodes.end());
    allPoints.insert(allPoints.end(), interface.begin(), interface.end());

    // 1) Loop around the cell coordinates ////////////////////////////////////

    /* This will be completed later, Dont worry about it for now */

    ////// 1a) Map out Pi and Pj
    intersectPoints[{1,2}] = 4;      // Placeholder = 4, since edges[4] = {1, 0.5}
    intersectPoints[{3,0}] = 5;      // Placeholder = 5, since edges[5] = {0, 0.5}

    // 2) Deduce the normal vector of the cutting line ////////////////////////
    std::array <double, 2> normal = normVec(interface);
    printNorm(normal);
    printf("\n");
   
    // 3) Deduce the sign of every node using the dot product /////////////////
    for(const auto& point : nodes){
        // Vector of Node
        V = pointVec(point, interface);
       
        // Dot Product of normal and node vector
        double dp = dotProduct(normal, V);
        printf("Dot Product of (%.1lf, %.1lf) and (%.1lf, %.1lf): %.2lf\n", V[0], V[1], normal[0], normal[1], dp);
       
        // Convection of placement with respect to the line
        signs[index] = (dp < 0) ? -1 : 1;       // Classy :)
        index++;
 
    }
    
    // Printing the orientation
    printf("\n");
    for(int i = 0; i < index; ++i){
        printf("Orientation %d: %d \n", i, signs[i]);
    }
    printf("\n");

    // 4) Clipping the values from below //////////////////////////////////////
    auto const belowLine = clip_below(edges, signs, intersectPoints);

    // Printing the Below values
    printf("\nNodes of the Line and Below the Line: \n");
    for(const auto &b : belowLine){ 
        std::cout << "(" << b.first << ", " << b.second << ")" << std::endl;
    }
    printf("\n");
}
