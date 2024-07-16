#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>

/*
    geometry.h Description:
        - Calculates the normal vector of the interface.
        - Calculates the direction vector with respect to (WRT) the normal vector.
        - Calculates the dot product of the normal and direction vector.
            - This will indicate the sign of the node. 
*/

namespace polyintersect {

// x and y values ///////////////////////////////////////////////////////////////////////
struct Point {
   double x = 0.0;
   double y = 0.0;
};

// Finding the normal vector between 2 Points ///////////////////////////////////////////
std::array<double, 2> normVec(std::array<Point, 2> const& inter);

// Finding the dot product of the array and vector //////////////////////////////////////
double dotProduct(std::array<double, 2> const& v , std::array<double, 2> &n);

// Print the normal vector //////////////////////////////////////////////////////////////
void printNorm(std::array<double, 2> const& normal);

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array<double, 2> pointVec(const Point &p, std::array<Point, 2> const& inter);

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
std::array<int, 6> orientation_clip_2_3(std::vector<Point> const allPoints, 
                                        std::array<Point, 2> const interface, 
                                        std::array<double, 2> V);

// Orientation of Every Node for Method 1 ///////////////////////////////////////////////
std::array<int, 4> orientation_clip_1(std::vector<Point> const nodes, 
                                      std::array<Point, 2> const interface, 
                                      std::array<double, 2> V);

// Find the Center Coordinate ///////////////////////////////////////////////////////////
Point center(std::vector<Point> &nodes);

// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void sorting(std::vector<Point> &nodes, Point center);

}