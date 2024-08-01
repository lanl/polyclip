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

  using Line = std::array<Point, 2>;

// Finding the normal vector between 2 Points ///////////////////////////////////////////
  std::array<double, 2> normVec(Line const& line);

// Finding the dot product of the array and vector //////////////////////////////////////
  double dotProduct(std::array<double, 2> const &v, std::array<double, 2> const &n);

// Print the normal vector //////////////////////////////////////////////////////////////
  void printNorm(std::array<double, 2> const &normal);

// Point Vector /////////////////////////////////////////////////////////////////////////
  std::array<double, 2> pointVec(const Point& p, Line const& line);

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
  std::array<int, 6> orientation_clip(std::vector<Point> const& allPoints, Line const& line);

// Find the Center Coordinate ///////////////////////////////////////////////////////////
  Point center(std::vector<Point> const& nodes);

// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
  void sorting(std::vector<Point> &nodes, Point const& center);
}