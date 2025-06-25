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

namespace polyclip {

// Below the line using Degrees /////////////////////////////////////////////////////////
KOKKOS_INLINE_FUNCTION
void clip_below_3(int cell,
                  Kokkos::View<Point*> points,
                  Kokkos::View<int***> cells,
                  Segment const& intersect_points,
                  Kokkos::View<int***> output,
                  Kokkos::View<int**> size_output,
                  Kokkos::View<int*> num_verts_per_cell,
                  Kokkos::View<int**> signs,
                  Kokkos::View<Point**> allPoints,
                  Line const& line) {
  int const n = num_verts_per_cell(cell) + 2;

  // Store all Points (vertices + intersect points) in a single list
  list_of_points(cell, points, cells, intersect_points, allPoints,
                 num_verts_per_cell);

  Point center_point = center(cell, n, allPoints);

  sorting(cell, n, allPoints, center_point);

  // Clip Below ///////////////////////////////////////////////////////////////////////////
  // Store the Orientation of every node

  orientation_clip(cell, allPoints, line.n, signs, n, intersect_points);

  int below = 0;
  int above = 0;
  for (int p = 0; p < n; p++) {
    if (signs(cell, p) <= 0) {
      output(cell, 0, below++) = p;
    }
  }

  // Clip Above ////////////////////////////////////////////////////////////////////////////
  Point flipped_normal = { -line.n.x, -line.n.y };
  orientation_clip(cell, allPoints, flipped_normal, signs, n, intersect_points);

  for (int p = 0; p < n; p++) {
    if (signs(cell, p) <= 0) {
      output(cell, 1, above++) = p;
    }
  }

  // keep track of number of vertices for the section of the cell
  // that is below the cutting plane
  size_output(cell, 0) = below; // The size of elements below the cutting plane
  size_output(cell, 1) = above; // The size of elements above the cutting plane
}
} // namespace polyclip
