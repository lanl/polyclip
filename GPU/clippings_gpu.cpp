#include "clippings_gpu.h"
#include "geometry_gpu.h"
#include <omp.h>


// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Method 3: Clipping Using Degrees /////////////////////////////////////////////////////
void  polyintersect::clip_below_3(int cell, 
                                  Kokkos::View<Point*> points,
                                  Kokkos::View<int**> cells,
                                  Line const &interface,
                                  Kokkos::View<int**> belowline,
                                  Kokkos::View<int*> size,
                                  Kokkos::View<Point[6]> allPoints) {

    // Store all Points in a single list
    list_of_points(cell, points, cells, interface, allPoints);

    // Find Center Point and Sort from Least to Greatest Degree
    // Point centerPoint = center(allPoints);
    // sorting(allPoints, centerPoint); // still a problem

    // Store the Orientation of every node
    int signs[6];
    orientation_clip(allPoints, interface, signs);
    //auto const sign = orientation_clip(allPoints, interface, signs);
    int const n = allPoints.size();

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




