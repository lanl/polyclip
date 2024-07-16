#include "geometry.h"

// Finding the normal vector between 2 Points ///////////////////////////////////////////
std::array<double, 2> polyintersect::normVec(std::array<Point, 2> const& inter){
   // Direction vec
   double dx = inter[1].x - inter[0].x;     // x2 - x1
   double dy = inter[1].y - inter[0].y;     // y2 - y1
   
   // Normal vec
   std::array <double, 2> normal = {dy, -dx};
   return normal;
}

// Finding the dot product of the array and vector //////////////////////////////////////
double polyintersect::dotProduct(std::array<double, 2> const& v , std::array<double, 2> &n){
   double product = (v[0] * n[0]) + (v[1] * n[1]);
   return product;
}

// Print the normal vector //////////////////////////////////////////////////////////////
void polyintersect::printNorm(std::array<double, 2> const& normal){
   printf("Normal: ");
   for(int i = 0; i < 2; ++i){
       printf("%.1lf ", normal[i]);
   }
   printf("\n");
}

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array<double, 2> polyintersect::pointVec(const Point &p, std::array<Point, 2> const& inter){
   double dx = p.x - inter[0].x;
   double dy = p.y - inter[0].y;
   
   std::array <double, 2> Point = {dx, dy};
   return Point;
}

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
std::array<int, 6> polyintersect::orientation_clip_2_3(std::vector<Point> const allPoints, 
                                                       std::array<Point, 2> const interface, 
                                                       std::array<double, 2> V){
    
    // Deduce the normal vector of the cutting line 
    std::array <double, 2> normal = normVec(interface);
    std::array<int, 6> signs;
    int index = 0;
    double dp;

    for(const auto& Point : allPoints){
        // Vector of Node
        V = pointVec(Point, interface);

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

// Orientation of Every Node for Method 1 ///////
std::array<int, 4> polyintersect::orientation_clip_1(std::vector<Point> const nodes, 
                                                    std::array<Point, 2> const interface, 
                                                    std::array<double, 2> V){
    
    // Deduce the normal vector of the cutting line 
    std::array <double, 2> normal = normVec(interface);
    std::array<int, 4> signs;
    int index = 0;
    double dp;

    for(const auto& Point : nodes){
        // Vector of Node
        V = pointVec(Point, interface);

        // Dot Product of normal and node vector
        dp = dotProduct(normal, V);

        // Convection of placement with respect to the line
        signs[index] = (dp < 0) ? -1 : 1;       // Classy :)
        index++;
    }

    return signs;
}
/////////////////////////////////////////////////////////////////////////////////////////
// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void polyintersect::sorting(std::vector<Point> &nodes, Point center){
    std::sort(nodes.begin(), nodes.end(), [&center](const Point& a, const Point& b){        
        double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));  
        double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));  
        return a1 < a2;   
    });
}

// Find the Center Coordinate ///////////////////
polyintersect::Point polyintersect::center(std::vector<Point> &nodes){
    std::vector<Point> result;
    double sumX = 0, sumY = 0;

    // Add up all the coordinates /////
    for(const auto &p : nodes){
        sumX += p.x;
        sumY += p.y;
    }

    // Store middle coordinates ///////  
    return {(sumX /= nodes.size()), (sumY /= nodes.size())};
}