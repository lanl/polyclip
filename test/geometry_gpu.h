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
    float dotProduct(Point const &v, Point const &n){
        //double product = (v.x * n.x) + (v.y * n.y);
	float dpx = (v.x * n.x);
	float dpy = (v.y * n.y);
	float product = dpx + dpy;

	// Dot Product 
        return product;
    }

    // Point Vector /////////////////////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Point pointVec(Point const &p, Point const& middle){
        double dx = p.x - middle.x;
        double dy = p.y - middle.y;
	
	// Direction Vector
        return {dx, dy};
    }

    // Middile Point of the Interface ////////////////////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    Point middle_point(Line const& line){
        double mx = (line.a.x + line.b.x) / 2;
        double my = (line.a.y + line.b.y) / 2;

        return {mx, my};
    }

    // Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
    KOKKOS_INLINE_FUNCTION
    void orientation_clip(int c, 
                          Kokkos::View<Point**> allPoints,
                          Line line, 
                          Kokkos::View<int**> signs,
                          int const n){

        // Deduce the normal vector of the cutting line
        Point normal = normVec(line);
        //int index = 0;
        Point middle = middle_point(line);
        float dp;

        for(int p = 0; p < n; p++){ 
            // Vector of Node
            Point const V = pointVec(allPoints(c, p), middle);

            // Dot Product of normal and node vector
            dp = dotProduct(V, normal);

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
                        Line const &line,
                        Kokkos::View<Point**> allPoints, 
                        Kokkos::View<int*> num_verts_per_cell) {

        int const m = num_verts_per_cell(cell);                    
        for (int i = 0; i < m; i++) {
            int index = cells(cell, i, 0);
            allPoints(cell, i) = points(index);
        }
        allPoints(cell, m) = line.a;
        allPoints(cell, (m + 1)) = line.b;
    }
}

