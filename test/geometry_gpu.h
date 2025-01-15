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
	- Creates a list of all the cells vertices and the intersecting points 
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
                          Kokkos::View<Point**> allPoints,
                          Line interface, 
                          Kokkos::View<int**> signs,
                          int n){

        // Deduce the normal vector of the cutting line
        auto normal = normVec(interface);
       // int index = 0;
        double dp;

        for(int p = 0; p < n; p++){ 
            // Vector of Node
            auto const V = pointVec(allPoints(c, p), interface);

            // Dot Product of normal and node vector
            dp = dotProduct(normal, V);

            // Convection of placement with respect to the line
            if (dp < 0) {           // Below the line
              signs(c, p) = -1;
            } else if (dp > 0) {    // Above the line
              signs(c, p) = 1;
            } else {                // On the line
              signs(c, p) = 0;
            }
           // index++;
        }
    }

    // Fake Intersect ///////////////////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Line fake_intersect_cell(int c){
        switch(c){
            case 0:	// Cell 0
                return {{.5, 0.125}, {0, 0.125}}; 
            case 1:	// Cell 1
                return {{0.75, 0.125}, {0.5, 0.125}};
            case 2:	// Cell 2
                return {{0.9375, 0.5}, {0.375, 0.5}};
            case 3:	// Cell 3
                return {{0.875, 0.75}, {0.5, 0.75}};
            default: 
                return {{-1.0, -1.0}, {-1.0, -1.0}};
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
                        Kokkos::View<int***> cells,
                        Line const &interface,
                        Kokkos::View<Point**> allPoints, 
                        Kokkos::View<int*> num_verts_per_cell) {

        int const m = num_verts_per_cell(cell);                    
        for (int i = 0; i < m; i++) {
            int index = cells(cell, i, 0);
            allPoints(cell, i) = points(index);
        }
        allPoints(cell, m) = interface.a;
        allPoints(cell, m + 1) = interface.b;
    }
}

