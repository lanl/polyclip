#include "SingleCell.h"


/*
Goal for this Code:
    1) Find the middle coordinate of all the nodes 
    2) Sort by using the node and center diffrence degree
    3) Clip the coordinate values below
*/

// Print Current and Updated Sort ///////////////////////////////////////////////////////
void printPoints(const std::vector<point> &nodes){
    for(const auto &p : nodes){
        std::cout << "(" << p.x << ", " << p.y << ") ";  
    }
    std::cout << std::endl;
}

// Sort in Counter Clockwise manner /////////////////////////////////////////////////////
void sorting(std::vector<point> &nodes, point center){
    std::sort(nodes.begin(), nodes.end(), [&center](const point& a, const point& b){
        double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI)) ;  
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
    point centerPoint = center(nodes);
    std::cout << "(" << centerPoint.x << ", " << centerPoint.y << ") " << std::endl; 
    std::cout << std::endl;

    // 2) Sorting nodes ///////////////////////////////////////////////////////
    sorting(nodes, centerPoint);
    printPoints(nodes);
    std::cout << std::endl;

    // 2a) Sorting all points /////////////////////////////////////////////////
    point centerAllPoint = center(allPoints);
    std::cout << "(" << centerPoint.x << ", " << centerPoint.y << ") " << std::endl; 
    sorting(allPoints, centerAllPoint);
    printPoints(allPoints);

    // 3) Clip the values below ///////////////////////////////////////////////
    std::cout << std::endl;
    for(int p = 0;  p < allPoints.size(); p++){
        if(signs[p] <= 0){
            belowLine.emplace_back(p);
        }
    }

    for(const auto &b : belowLine){ 
        std::cout << "(" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
    } 
    printf("\n");

}