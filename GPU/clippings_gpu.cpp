#include "clippings_gpu.h"
#include "geometry_gpu.h"
#include <omp.h>
#include <Kokkos_Core.hpp>

// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Method 3: Clipping Using Degrees /////////////////////////////////////////////////////
Kokkos::View<int**> polyintersect::clip_below_3(int cell, Mesh_Kokkos const &mesh,
                                                Line const &interface) {

    // Store all Points in a single list
    Kokkos::View<Point[6]> allPoints;
    mesh.list_of_points(cell, interface, allPoints);

    // Find Center Point and Sort from Least to Greatest Degree
    Point centerPoint = center(allPoints);
    sorting(allPoints, centerPoint);

    // Store the Orientation of every node
    int signs[6];
    orientation_clip(allPoints, interface, signs);
    //auto const sign = orientation_clip(allPoints, interface, signs);
    int const n = allPoints.size();
    Kokkos::View<int**> belowline;

    // Clip below
    int count = 0;
    for (int p = 0; p < n; p++) {
        if (signs[p] <= 0) {                // NEW PORTION
        belowline(cell, count++) = p;
        }
    }
        
    Kokkos::resize(belowline, 1, count);
    return belowline;
}




