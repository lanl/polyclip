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

#ifdef USE_SINGLE_PRECISION
using real = float;
#else
using real = double;
#endif

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
#ifdef USE_SINGLE_PRECISION
  real x = 0.f;
  real y = 0.f;
#else
  real x = 0.0;
  real y = 0.0;
#endif
};

struct Segment {
  Point a;
  Point b;
};

struct Line {
  Point n;    // normal

#ifdef USE_SINGLE_PRECISION
  real d = 0.f; // distance
#else
  real d = 0.0;
#endif
};

struct Edge {
  int a;
  int b;
};

// Finding the normal vector between 2 Points ///////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point normVec(Point a, Point b) {
  // Direction vec
  real dx = b.x - a.x; // x2 - x1
  real dy = b.y - a.y; // y2 - y1

  // Normal vec
  return { dy, -dx };
}

// Finding the dot product of the direction vector and normal of the line //////////////
KOKKOS_INLINE_FUNCTION
real dotProduct(Point const& v, Point const& n) {
  real const product = (v.x * n.x) + (v.y * n.y);
#ifdef USE_SINGLE_PRECISION
  return std::abs(product) < 1.e-10 ? 0.f : product;
#else
  return std::abs(product) < 1.e-15 ? 0.0 : product;
#endif
}

// Point Vector /////////////////////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point pointVec(Point const& p, Point const& middle) {
  real dx = p.x - middle.x;
  real dy = p.y - middle.y;

  // Direction Vector
  return { dx, dy };
}

// Middile Point of the Interface ////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
Point middle_point(Segment const& points) {
#ifdef USE_SINGLE_PRECISION
  real mx = (points.a.x + points.b.x) / 2.0f;
  real my = (points.a.y + points.b.y) / 2.0f;
#else
  real mx = (points.a.x + points.b.x) / 2.0;
  real my = (points.a.y + points.b.y) / 2.0;
#endif
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
  real dp;

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
  real sumX = 0, sumY = 0;
#ifdef USE_LOOP_UNROLLING
  // Add up all the coordinates /////

//#pragma unroll 4
#pragma unroll 8  
//#pragma unroll 5
  for (int p = 0; p < n; p++) { //(const auto &p: nodes) {
    sumX += allPoints(c, p).x;
    sumY += allPoints(c, p).y;
  }
#else
  // Add up all the coordinates /////
  for (int p = 0; p < n; p++) { //(const auto &p: nodes) {
    sumX += allPoints(c, p).x;
    sumY += allPoints(c, p).y;
  }
#endif
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

/*#ifdef USE_LOOP_UNROLLING
//#pragma unroll 2
//#pragma unroll 4
#pragma unroll 3
  for (int i = 0; i < m; i++) {
    int index = cells(cell, i, 0);
    allPoints(cell, i) = points(index);
  }
#else*/
  for (int i = 0; i < m; i++) {
    int index = cells(cell, i, 0);
    allPoints(cell, i) = points(index);
  }
//#endif
  allPoints(cell, m) = intersect_points.a;
  allPoints(cell, (m + 1)) = intersect_points.b;
}

// Compare Points ////////////////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
bool compare_points(const Point p1, const Point p2, Point center_point) {
#ifdef USE_SINGLE_PRECISION
  constexpr real pi = 3.14159265f;
  real degree = 180.0f;
#else
  constexpr real pi = M_PI;
  real degree = 180.0;
#endif 

#ifdef USE_SFU
  real a1 =
    (atan2(p1.y - center_point.y, p1.x - center_point.x) * (degree / pi));
  real a2 =
    (atan2(p2.y - center_point.y, p2.x - center_point.x) * (degree / pi));
  return a1 < a2;
#else
  real a1 =
    (std::atan2(p1.y - center_point.y, p1.x - center_point.x) * (degree / pi));
  real a2 =
    (std::atan2(p2.y - center_point.y, p2.x - center_point.x) * (degree / pi));
  return a1 < a2;
#endif
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
