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

#include "mesh.h"
#include "geometry.h"
#include <map>

namespace polyintersect {
KOKKOS_INLINE_FUNCTION
Line intersect_cell_with_line(Kokkos::View<Point*> points,
                              Kokkos::View<int***> cells,
                              int c,
                              Line const& line,
                              Kokkos::View<int*> num_verts_per_cell) {
  int const n = num_verts_per_cell(c);
  Point pts[2];

  int k = 0;
  for (int i = 0; i < n; ++i) {
    int const a = cells(c, i, 0);
    int const b = cells(c, i, 1);

    real x_min, y_min, x_max, y_max;

    // deduce bounds on coordinates of the edge we are currently viewing
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

    real const& xa = points(a).x;
    real const& ya = points(a).y;
    real const& xb = points(b).x;
    real const& yb = points(b).y;

    real const& xp = line.a.x;
    real const& yp = line.a.y;
    real const& xq = line.b.x;
    real const& yq = line.b.y;

    // (ab) is represented as: a1x + b1y = c1
    // (pq) is represented as: a2x + b2y = c2
    real const a1 = yb - ya;
    real const b1 = xa - xb;
    real const a2 = yq - yp;
    real const b2 = xp - xq;
    real const det = a1 * b2 - a2 * b1;

    // fabs
    if (fabs(det) < 1.e-15) {
      continue;
    } else {
      real const c1 = a1 * xa + b1 * ya;
      real const c2 = a2 * xp + b2 * yp;
      real const x = (b2 * c1 - b1 * c2) / det;
      real const y = (a1 * c2 - a2 * c1) / det;

      if (x < x_min or x > x_max or y < y_min or y > y_max) {
        continue;
      }
      pts[k] = { x, y };
      k++;
    }
  }
  return { pts[0], pts[1] };
}
} // namespace polyintersect
