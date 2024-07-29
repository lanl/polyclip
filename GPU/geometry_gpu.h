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
#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>

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

    struct Line {
        Point a;
        Point b;
    };

    // Finding the normal vector between 2 Points ///////////////////////////////////////////
    Point normVec(Line const &line);

    // Finding the dot product of the array and vector //////////////////////////////////////
    double dotProduct(Point const &v, Point const &n);

    // Point Vector /////////////////////////////////////////////////////////////////////////
    Point pointVec(Point const &p, Line const& line);

    // Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
    void orientation_clip(Kokkos::View<Point[6]> const &allPoints,
                          Line line, int signs[6]);

    // Find the Center Coordinate ///////////////////////////////////////////////////////////
    Point center(Kokkos::View<Point[6]> const &nodes);

    // Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
    void sorting(Kokkos::View<Point[6]> &nodes, Point const &center);
}