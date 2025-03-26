#pragma once
#include "mesh_gpu.h"

namespace polyintersect {

// Below the line using Degrees /////////////////////////////////////////////////////////
  KOKKOS_INLINE_FUNCTION
  void clip_below_3(int cell, 
                  Kokkos::View<Point*> points,
                  Kokkos::View<int***> cells,
                  Segment const &intersect_points,
                  Kokkos::View<int***> output,
                  Kokkos::View<int**> size_output,
                  Kokkos::View<int*> num_verts_per_cell,
                  Kokkos::View<int**> signs,
                  Kokkos::View<Point**> allPoints,
		  Line const &line) {


      int const n = num_verts_per_cell(cell) + 2;

    // Store all Points (vertices + intersect points) in a single list


    list_of_points(cell, points, cells, intersect_points, allPoints, num_verts_per_cell);


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
    Point flipped_normal = {-line.n.x, -line.n.y};
    orientation_clip(cell, allPoints, flipped_normal, signs, n, intersect_points);

    for (int p = 0; p < n; p++) {
        if (signs(cell, p) <= 0) {
            output(cell, 1, above++) = p;
        }
    }

    // keep track of number of vertices for the section of the cell
    // that is below the cutting plane
    size_output(cell, 0) = below;	// The size of elements below the cutting plane
    size_output(cell, 1) = above;	// The size of elements above the cutting plane
  }
}
