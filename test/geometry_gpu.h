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
    
    struct Edge {
	int a;
	int b;
    }; 

    // Finding the normal vector between 2 Points ///////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Point normVec(Line const &line){
        // Direction vec
        double dx = line.b.x - line.a.x;     // x2 - x1
        double dy = line.b.y - line.a.y;     // y2 - y1

        // Normal vec
        return {dy, -dx};
    }

    // Finding the dot product of the array and vector //////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    double dotProduct(Point const &v, Point const &n){
        double product = (v.x * n.x) + (v.y * n.y);
        return product;
    }

    // Point Vector /////////////////////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Point pointVec(Point const &p, Line const& line){
        double dx = p.x - line.a.x;
        double dy = p.y - line.a.y;

        return {dx, dy};
    }

    // Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    void orientation_clip(int c, 
                          Kokkos::View<Point**> list,
                          Line line, 
                          int signs[6]) {

        // Deduce the normal vector of the cutting line
        auto normal = normVec(line);
        int index = 0;
        double dp;

        for(int p = 0; p < 6; p++){ //(const auto &p: allPoints) {
            // Vector of Node
            auto const V = pointVec(list(c, p), line);

            // Dot Product of normal and node vector
            dp = dotProduct(normal, V);

            // Convection of placement with respect to the line
            if (dp < 0) {           // Below the line
              signs[index] = -1;
            } else if (dp > 0) {    // Above the line
              signs[index] = 1;
            } else {                // On the line
              signs[index] = 0;
            }
            index++;
        }
    }

    // Find the Center Coordinate ///////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Point center(Kokkos::View<Point*> nodes){
        double sumX = 0, sumY = 0;

        // Add up all the coordinates /////
        for(int p = 0; p < 6; p++){ //(const auto &p: nodes) {
            sumX += nodes(p).x;
            sumY += nodes(p).y;
        }
        double const n = nodes.size();

        // Store middle coordinates ///////
        return {sumX / n, sumY / n};
    }

    // get list of points  ////////////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    void list_of_points(int cell,
                        Kokkos::View<Point*> points,
                        Kokkos::View<int**> cells,
                        Line const &line,
                        Kokkos::View<Point**> list) {

        for (int i = 0; i < 4; i++) {
            int index = cells(cell, i);
            list(cell, i) = points(index);
        }
        list(cell, 4) = line.a;
        list(cell, 5) = line.b;
    }
}
