// C++ program for the above approach
#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
 
using namespace std;
 
struct Point {
    double x, y;
};
 
// Checking if a point is inside a polygon
bool point_in_polygon(const Point  &p, vector<Point> nodes)
{
    int num_vertices = nodes.size();
    double x = p.x, y = p.y;
    bool inside = false;
 
    // Store the first point in the polygon and initialize
    // the second point
    Point p1 = nodes[0], p2;
 
    // Loop through each edge in the polygon
    for (int i = 1; i <= num_vertices; i++) {
        // Get the next point in the polygon
        p2 = nodes[i % num_vertices];
 
        // Check if the point is above the minimum y
        // coordinate of the edge
        if (y > min(p1.y, p2.y)) {
            // Check if the point is below the maximum y
            // coordinate of the edge
            if (y <= max(p1.y, p2.y)) {
                // Check if the point is to the left of the
                // maximum x coordinate of the edge
                if (x <= max(p1.x, p2.x)) {
                    // Calculate the x-intersection of the
                    // line connecting the point to the edge

                    // THE MASTER MIND
                    double x_intersection
                        = (y - p1.y) * (p2.x - p1.x)
                              / (p2.y - p1.y)
                          + p1.x;
 
                    // Check if the point is on the same
                    // line as the edge or to the left of
                    // the x-intersection
                    if (p1.x == p2.x
                        || x <= x_intersection) {
                        // Flip the inside flag
                        inside = !inside;
                    }
                }
            }
        }
 
        // Store the current point as the first point for
        // the next iteration
        p1 = p2;
    }
 
    // Return the value of the inside flag
    return inside;
}


int main()
{
    // Define a point to test
    //Point point = {1.0, 0.5};
    std::array<Point, 2> interface = {{{1.0, 0.5}, {0.0, 0.5}}};
 
    // Define a polygon
    vector<Point> nodes = {
        {0,0}, {1,0}, {1,1}, {0,1}
    };
 
 for(const auto& point : interface){
        // Check if the point is inside the polygon
        if (point_in_polygon(point, nodes)) {
            cout << "Point is inside the polygon" << endl;
        }
        else {
            cout << "Point is outside the polygon" << endl;
        }
    }
    return 0;
}