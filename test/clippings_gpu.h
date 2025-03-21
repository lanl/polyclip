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


    Kokkos::Profiling::pushRegion("CLIPPING BELOW - LIST OF POINT GENERATION");
    list_of_points(cell, points, cells, intersect_points, allPoints, num_verts_per_cell);
    Kokkos::Profiling::popRegion();


    Kokkos::Profiling::pushRegion("CLIPPING BELOW - CENTER POINT");
    Point center_point = center(cell, n, allPoints);
    Kokkos::Profiling::popRegion();

    Kokkos::Profiling::pushRegion("CLIPPING BELOW - SORTING POINTS");
    sorting(cell, n, allPoints, center_point);
    Kokkos::Profiling::popRegion();

    // Clip Below ///////////////////////////////////////////////////////////////////////////
    // Store the Orientation of every node

    Kokkos::Profiling::pushRegion("CLIPPING BELOW - ORIENTATION CLIP #1");
    orientation_clip(cell, allPoints, line.n, signs, n, intersect_points);
    Kokkos::Profiling::popRegion();

    int below = 0;
    int above = 0;
    for (int p = 0; p < n; p++) {
        if (signs(cell, p) <= 0) {               
            output(cell, 0, below++) = p;
        }
    }

    // Clip Above ////////////////////////////////////////////////////////////////////////////
    Point flipped_normal = {-line.n.x, -line.n.y};
    Kokkos::Profiling::pushRegion("CLIPPING BELOW - ORIENTATION CLIP #2");
    orientation_clip(cell, allPoints, flipped_normal, signs, n, intersect_points);
    Kokkos::Profiling::popRegion();

    Kokkos::Profiling::pushRegion("CLIPPING BELOW - OUTPUT");
    for (int p = 0; p < n; p++) {
        if (signs(cell, p) <= 0) {
            output(cell, 1, above++) = p;
        }
    }
    Kokkos::Profiling::popRegion();

    // keep track of number of vertices for the section of the cell
    // that is below the cutting plane
    size_output(cell, 0) = below;	// The size of elements below the cutting plane
    size_output(cell, 1) = above;	// The size of elements above the cutting plane
  }
}
