/*
 * (c) 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad National
 * Security, LLC for the U.S. Department of Energy/National Nuclear Security
 * Administration. All rights in the program are reserved by Triad National
 * Security, LLC, and the U.S. Department of Energy/National Nuclear Security
 * Administration. The Government is granted for itself and others acting on its
 * behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
 * to reproduce, prepare. derivative works, distribute copies to the public,
 * perform publicly and display publicly, and to permit others to do so.
 */
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

namespace polyclip {

// x and y values ///////////////////////////////////////////////////////////////////////
struct Point {
  double x = 0.0;
  double y = 0.0;
};

struct Segment {
  Point a;
  Point b;
};

struct Line {
  Point n;  // normal
  double d; // distance
};

struct Edge {
  int a;
  int b;
};

// Finding the normal vector between 2 Points ///////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point normVec(Point a, Point b) {
  // Direction vec
  double dx = b.x - a.x; // x2 - x1
  double dy = b.y - a.y; // y2 - y1

  // Normal vec
  return { dy, -dx };
}

// Finding the dot product of the direction vector and normal of the line //////////////
KOKKOS_INLINE_FUNCTION
double dotProduct(Point const& v, Point const& n) {
  double const product = (v.x * n.x) + (v.y * n.y);

  double const epsilon = 1.e-15;
  return std::abs(product) < epsilon ? 0.0 : product;
}

// Point Vector /////////////////////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point pointVec(Point const& p, Point const& middle) {
  double dx = p.x - middle.x;
  double dy = p.y - middle.y;

  // Direction Vector
  return { dx, dy };
}

// Middile Point of the Interface ////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point middle_point(Segment const& points) {
  double mx = (points.a.x + points.b.x) / 2;
  double my = (points.a.y + points.b.y) / 2;

  return { mx, my };
}

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
void orientation_clip(int c,
                      Kokkos::View<Point**> allPoints,
                      Point normal,
                      Kokkos::View<int**> signs,
                      int const n,
                      Segment intersect_points) {
  // Deduce the normal vector, middle point, and distance of the clipping line
  Point middle =
    middle_point(intersect_points); // 2) Calculate the middle point of the line
  double dp;

  for (int p = 0; p < n; p++) {
    Point const V = pointVec(allPoints(c, p), middle);

    // Dot Product of normal and node vector
    dp = dotProduct(V, normal);

    // Convection of placement with respect to the line
    if (dp < 0) { // Below the line
      signs(c, p) = -1;
    } else if (dp > 0) { // Above the line
      signs(c, p) = 1;
    } else { // On the line
      signs(c, p) = 0;
    }
  }
}

// Find the Center Coordinate ///////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point center(int c, int n, Kokkos::View<Point**> allPoints) {
  double sumX = 0, sumY = 0;

  // Add up all the coordinates /////
  for (int p = 0; p < n; p++) { //(const auto &p: nodes) {
    sumX += allPoints(c, p).x;
    sumY += allPoints(c, p).y;
  }

  // Store middle coordinates ///////
  return { sumX / n, sumY / n };
}

// get list of points  ////////////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
void list_of_points(int cell,
                    Kokkos::View<Point*> points,
                    Kokkos::View<int***> cells,
                    Segment const& intersect_points,
                    Kokkos::View<Point**> allPoints,
                    Kokkos::View<int*> num_verts_per_cell) {
  int const m = num_verts_per_cell(cell);
  for (int i = 0; i < m; i++) {
    int index = cells(cell, i, 0);
    allPoints(cell, i) = points(index);
  }
  allPoints(cell, m) = intersect_points.a;
  allPoints(cell, (m + 1)) = intersect_points.b;
}

// Compare Points ////////////////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
bool compare_points(const Point p1, const Point p2, Point center_point) {
  double a1 =
    (std::atan2(p1.y - center_point.y, p1.x - center_point.x) * (180 / M_PI));
  double a2 =
    (std::atan2(p2.y - center_point.y, p2.x - center_point.x) * (180 / M_PI));
  return a1 < a2;
}

// Sort all points based on degrees ///////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
void sorting(int c,
             int n,
             Kokkos::View<Point**> allPoints,
             Point center_point) {
  for (int i = 1; i < n; ++i) {
    Point current_point = allPoints(c, i);
    int insert_index = i;
    for (int j = (i - 1); j >= 0; j--) {
      if (compare_points(current_point, allPoints(c, j), center_point)) {
        allPoints(c, (j + 1)) = allPoints(c, j);
        insert_index = j;
      } else {
        break;
      }
    }
    allPoints(c, insert_index) = current_point;
  }
}

} // namespace polyclip
