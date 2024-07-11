#include "SingleCell.h"

/*
    Code Description:
        - Method 3: Finds the center point of all the nodes, calculates 
          the degree of every node WTR to the center point, and sorts based 
          on smallest to biggest degree. Uses the sorted value of all points to 
          compare the signs.
*/

// Print Current and Updated Sort ///////////////////////////////////////////////////////
void printPoints(const std::vector<point> &nodes){
    std::cout << "Sorted Points: " ;
    for(const auto &p : nodes){
        std::cout << "(" << p.x << ", " << p.y << ") ";  
    }
    std::cout << std::endl;
}

// // Sort in Counter Clockwise manner /////////////////////////////////////////////////////
// void sorting(std::vector<point> &nodes, point center){
//     std::vector<int> degree(nodes.size());
//     int index = 0;

//     for(int i = 0; i < nodes.size(); i++){
//         degree[i] = (std::atan2(nodes[i].y - center.y, nodes[i].x - center.x) * (180 / M_PI));
//     }

//     std::cout << "Degrees: ";
//     for(int i = 0; i < degree.size(); i++){
//         std::cout << degree[i] << "\t";
//     }
//     std::cout << std::endl << std::endl;

//     std::sort(nodes.begin(), nodes.end(), [&degree, &nodes](const point& a, const point& b){
//         std::cout << "Addresses: " << &a << " and " << &nodes[0] << std::endl;
//         std::cout << "Addresses: " << &b << " and " << &nodes[0] << std::endl;

//         int a1 = (&a - &nodes[0]);
//         int a2 = (&b - &nodes[0]);

//         std::cout << "Index: " << a1 << " and " << a2 << std::endl;
//         std::cout << "Degrees: " << degree[a1] << " < " << degree[a2] << std::endl;

//         bool value = degree[a1] < degree[a2];
//         std::cout << "Result: " << value << std::endl << std::endl;

//         return value;   
//     });

//     std::cout << std::endl;
// }


// // Sort in Counter Clockwise manner /////////////////////////////////////////////////////
// void sorting(std::vector<point> &nodes, point center){
//     std::vector<int> degree(nodes.size());
//     int index = 0;

//     // Calculate the degree WRT the center point //////////
//     for(int i = 0; i < nodes.size(); i++){
//         degree[i] = (std::atan2(nodes[i].y - center.y, nodes[i].x - center.x) * (180 / M_PI));
//     }

//     // The indices of nodes ///////////////////////////////
//     std::vector<int> indices(nodes.size());
//     for(int i = 0; i < nodes.size(); i++){
//         indices[i] = i;
//     }

//     // Store indices based on smallest to biggest degree //
//     std::sort(indices.begin(), indices.end(), [&degree](size_t a, size_t b){
//         return degree[a] < degree[b];   
//     });

//     // Rearrange the nodes based on the degree indices //// 
//     std::vector<point> sortedNodes(nodes.size());
//     for(int i = 0; i < nodes.size(); i++){
//         sortedNodes[i] = nodes[indices[i]];
//     }

//     // Update Nodes ///////////////////////////////////////
//     nodes = std::move(sortedNodes);
// }

// Original Sorting Method //////////////////////////////////////////////////////////////
void sorting(std::vector<point> &nodes, point center){
    std::sort(nodes.begin(), nodes.end(), [&center](const point& a, const point& b){        
        double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));  
        double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));  
        return a1 < a2;   
    });
}

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

int main(){
    // Initialize nodes and all points (nodes + interface)
    std::vector<point> nodes = {{1,1}, {0,0}, {1,0}, {0,1}};
    std::vector<point> allPoints = {{0,1}, {1,0.5}, {0,0}, {1,0}, {1,1}, {0, 0.5}};
    std::array<int, 6> signs = {{-1, -1, 0, 1, 1, 0}};
    std::vector<int> belowLine;


    // 1) Find the center of the cell /////////////////////////////////////////
    std::cout << std::endl;
    point centerPoint = center(nodes);
    std::cout << "Center Point: (" << centerPoint.x << ", " << centerPoint.y << ") " << std::endl; 
    std::cout << std::endl;

    // 2) Sorting nodes ///////////////////////////////////////////////////////
    sorting(nodes, centerPoint);
    printPoints(nodes);
    std::cout << std::endl;

    // 2a) Sorting all points /////////////////////////////////////////////////
    point centerAllPoint = center(allPoints);
    std::cout << "Center Point: (" << centerPoint.x << ", " << centerPoint.y << ") " << std::endl << std::endl; 

    sorting(allPoints, centerAllPoint);
    //std::cout << std::endl;
    printPoints(allPoints);

    // 3) Clip the values below ///////////////////////////////////////////////
    std::cout << std::endl;
    for(int p = 0;  p < allPoints.size(); p++){
        if(signs[p] <= 0){
            belowLine.emplace_back(p);
        }
    }

    std::cout << "Clipped Below: ";
    for(const auto &b : belowLine){ 
        std::cout << "(" << allPoints[b].x << ", " << allPoints[b].y << ") ";
    } 
    printf("\n\n");

}