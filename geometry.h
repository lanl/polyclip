#pragma once
#include "Mesh.h"

namespace polyintersect {

// Finding the normal vector between 2 points ///////////////////////////////////////////
std::array<double, 2> normVec(std::array<point, 2> const& inter);

// Finding the dot product of the array and vector //////////////////////////////////////
double dotProduct(std::array<double, 2> const& v , std::array<double, 2> &n);

// Print the normal vector //////////////////////////////////////////////////////////////
void printNorm(std::array<double, 2> const& normal);

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array<double, 2> pointVec(const point &p, std::array<point, 2> const& inter);

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
std::array<int, 6> orientation_clip_2_3(std::vector<point> const allPoints, 
                                        std::array<point, 2> const interface, 
                                        std::array<double, 2> V);

// Orientation of Every Node for Method 1 ///////
std::array<int, 4> orientation_clip_1(std::vector<point> const nodes, 
                                      std::array<point, 2> const interface, 
                                      std::array<double, 2> V);
/////////////////////////////////////////////////////////////////////////////////////////

// Find the Center Coordinate ///////////////////
point center(std::vector<point> &nodes);

// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void sorting(std::vector<point> &nodes, point center);

}