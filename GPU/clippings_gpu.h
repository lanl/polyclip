#pragma once
#include "mesh_gpu.h"

namespace polyintersect {

// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Below the line using Degrees /////////////////////////////////////////////////////////
void clip_below_3(int cell, 
                   Kokkos::View<Point*> points,
                   Kokkos::View<int**> cells,
                   Line const &interface,
                   Kokkos::View<int**> belowline,
                   Kokkos::View<int*> size,
                   Kokkos::View<Point[6]> allPoints);

}