#pragma once

#include "mesh.h"
#include "geometry.h"
#include <map>

namespace polyclip {
KOKKOS_INLINE_FUNCTION
Segment intersect_cell_with_line_n_d(Kokkos::View<Point*> points,
                                     Kokkos::View<int***> cells,
                                     int c,
                                     Line const& line,
                                     Kokkos::View<int*> num_verts_per_cell) {
  int const n = num_verts_per_cell(c);
  Point pts[2];
  constexpr Point const DUMMY = { DBL_MAX, DBL_MAX };

  int k = 0;

  for (int i = 0; i < n and k < 2; ++i) {
    double x_min, y_min, x_max, y_max;
    int const a = cells(c, i, 0);
    int const b = cells(c, i, 1);

    // Interface Normal and Distance
    Point const n1 = line.n;
    double const d1 = line.d;

    // Current Edge normal vector and distanc
    Point const n2 = normVec(points(a), points(b));
    double const d2 = -dotProduct(points(a), n2);

    // Deduce bounds on coordinates of the edge we are currently viewing

    if (points(a).x > points(b).x) {
      x_max = points(a).x;
      x_min = points(b).x;
    } else {
      x_max = points(b).x;
      x_min = points(a).x;
    }
    if (points(a).y > points(b).y) {
      y_max = points(a).y;
      y_min = points(b).y;
    } else {
      y_max = points(b).y;
      y_min = points(a).y;
    }

    double const& a1 = n1.x;
    double const& b1 = n1.y;
    double const& a2 = n2.x;
    double const& b2 = n2.y;

    // Compute the determinate
    double const det = (a1 * b2) - (a2 * b1);

    if (fabs(det) < 1.e-15) {
      continue;
    } else {
      // Compute intersection points using Cramers rule

      double const xa = (-d1 * b2) - (-d2 * b1);
      double const ya = (a1 * -d2) - (a2 * -d1);
      double const x = xa / det;
      double const y = ya / det;

      if (x < x_min or x > x_max or y < y_min or y > y_max) {
        continue;
      }
      pts[k] = { x, y };
      k++;
    }
  }

  // Check if line intersects
  if (k == 0) {
    return { DUMMY, DUMMY }; //dummy value
  }
  return { pts[0], pts[1] };
}

// first define this predicate in 'intersect_n_d_gpu.h'
KOKKOS_INLINE_FUNCTION
bool intersects(Kokkos::View<Point*> points,
                Kokkos::View<int***> cells,
                int c,
                Segment const& segment,
                Kokkos::View<int*> num_verts_per_cell) {
  // bounding box
  int const n = num_verts_per_cell(c);
  Point p_min = points(cells(c, 0, 0)); // first point of 'c'
  Point p_max = points(cells(c, 0, 0));

  for (int i = 1; i < n; ++i) {
    int const a = cells(c, i, 0);
    if (p_min.x > points(a).x) {
      p_min.x = points(a).x;
    }
    if (p_min.y > points(a).y) {
      p_min.y = points(a).y;
    }
    if (p_max.x < points(a).x) {
      p_max.x = points(a).x;
    }
    if (p_max.y < points(a).y) {
      p_max.y = points(a).y;
    }
  }

  // now check 'segment'
  Point const& p = segment.a;
  Point const& q = segment.b;

  return !(p.x < p_min.x || p.y < p_min.y || p.x > p_max.x || p.y > p_max.y ||
           q.x < p_min.x || q.y < p_min.y || q.x > p_max.x || q.y > p_max.y);
}
} // namespace polyclip
