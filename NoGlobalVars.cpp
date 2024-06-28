
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
std::array <double, 2> normVec(std::array <std::array<double, 2>,2> &inter){
   // Direction vec
   double dx = inter[1][0] - inter[0][0];     // x2 - x1
   double dy = inter[1][1] - inter[0][1];     // y2 - y1
   
   // Normal vec
   std::array <double, 2> normal = {dy, -dx};
   return normal;
}

// Finding the dot product of the array and vector //////////////////////////////////////
double dotProduct(std::array <double, 2> v , std::array<double,2> &n){
   double product = (v[0] * n[0]) + (v[1] * n[1]);
   return product;
}

// Print the normal vector //////////////////////////////////////////////////////////////
void printNorm(std::array <double, 2> normal){
   printf("Normal: ");
   for(int i = 0; i < 2; ++i){
       printf("%.1lf ", normal[i]);
   }
   printf("\n");
}

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array <double, 2> pointVec(const point &p, std::array <std::array<double,2>,2> inter){
   double dx = p.x - inter[0][0];
   double dy = p.y - inter[0][1];
   
   std::array <double, 2> point = {dx, dy};
   return point;
}

// Below the Line ////////////////////////////////////////////////////////////////////////
void below(std::vector<std::pair<double,double>> &edges, std::array<int,4> sign, std::map<std::pair<int, int>, int>  location, std::vector<std::pair<double,double>> &belowline){
    int i, j, loop = 0;                                                                                                                               
    for(const auto& edge : edges){
        i = edge.first;
        j = edge.second;
        std::cout << "Iterate: (" << i << ", " << j << ")" << std::endl; 

        // Will stop before looping through the interface id's
        if(loop == 4){
            break;
        }

        // Checking all possibilities 
        if(sign[i] == sign[j] && sign[i] < 0){ 
            belowline.emplace_back(i,j);    
        }
        else if(sign[i] < 0){
            belowline.emplace_back(i, location[{i,j}]);
        }
        else if(sign[j] < 0){
            belowline.emplace_back(location[{i,j}], j);  // Fix the Flip
        }

        loop++;
    }
}


int main(int argc, const char * argv[]){
    int index = 0;
    //std::array<double, 2> Pi = 12, Pj = {0, 0.5}; //this used to only be an integer
    std::array<int, 4> signs;
    std::array<double, 2> V;
    
    // Cell nodes, Interface, and Map of Pi and Pj
    std::vector<point> nodes = {{0,0}, {1,0}, {1,1}, {0,1}};
    std::array<std::array<double,2>, 2> interface = {{{1, 0.5}, {0, 0.5}}}; // value points were actually in here
    std::map<std::pair<int, int>, int > locationP; 
    std::vector<std::pair<double,double>> edges = {{0,1}, {1,2}, {2,3}, {3,0}, {1,0.5}, {0,0.5}}, belowLine; // intiallized with edges
   
    // 1) Loop around the cell coordinates ////////////////////////////////////

    /* This will be completed later, Dont worry about it for now */

    ////// 1a) Map out Pi and Pj
    locationP[{1,2}] = 4;      // Placeholder = 4, since edges[4] = {1, 0.5}
    locationP[{3,0}] = 5;      // Placeholder = 5, since edges[5] = {0, 0.5}

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
    below(edges,signs,locationP,belowLine);

    // Printing the Below values
    printf("\nNodes of the Line and Below the Line: \n");
    for(const auto &b : belowLine){ 
        std::cout << "(" << b.first << ", " << b.second << ")" << std::endl;
    }
    printf("\n");
}
