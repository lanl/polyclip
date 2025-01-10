#pragma once
#include "mesh_gpu.h"

namespace polyintersect {

// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Below the line using Degrees /////////////////////////////////////////////////////////
  KOKKOS_INLINE_FUNCTION
  void clip_below_3(int cell, 
                  Kokkos::View<Point*> points,
                  Kokkos::View<int***> cells,
                  Line const &interface,
                  Kokkos::View<int**> output,
                  Kokkos::View<int*> size_output,
                  Kokkos::View<int*> num_verts_per_cell,
                  Kokkos::View<int**> signs,
                  Kokkos::View<Point**> allPoints) {

    // Store all Points in a single list
    list_of_points(cell, points, cells, interface, allPoints, num_verts_per_cell);

    // Store the Orientation of every node
    int const n = num_verts_per_cell(cell) + 2;
    orientation_clip(cell, allPoints, interface, signs, n);

    // Clip below
    int count = 0;
    for (int p = 0; p < n; p++) {
        if (signs(cell, p) <= 0) {                // NEW PORTION
            output(cell, count++) = p;
        }
    }
    // keep track of number of vertices for the section of the cell
    // that is below the cutting plane
    size_output(cell) = count;
  }
}



/*#pragma once
#include "mesh_gpu.h"

namespace polyintersect {

// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Below the line using Degrees /////////////////////////////////////////////////////////
  KOKKOS_INLINE_FUNCTION
  void clip_below_3(int cell, 
                  Kokkos::View<Point*> points,
                  Kokkos::View<int**> cells,
                  Line const &interface,
                  Kokkos::View<int**> belowline,
                  Kokkos::View<int*> size,
                  Kokkos::View<Point**> allPoints) {

    // Store all Points in a single list
    list_of_points(cell, points, cells, interface, allPoints);

    // Store the Orientation of every node
    int signs[6];
    orientation_clip(cell, allPoints, interface, signs);
    int const n = 6;

    // Clip below
    int count = 0;
    for (int p = 0; p < n; p++) {
        if (signs[p] <= 0) {                // NEW PORTION
            belowline(cell, count++) = p;
        }
    }
    // keep track of number of vertices for the section of the cell
    // that is below the cutting plane
    size(cell) = count;
  }
}*/
