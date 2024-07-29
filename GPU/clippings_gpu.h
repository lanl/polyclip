#pragma once
#include "mesh_gpu.h"

namespace polyintersect {

// TAKES AN ARRAY AND THE VECTOR NEEDS TO CHANGE TO A VIEW

// Below the line using Degrees /////////////////////////////////////////////////////////
std::vector<int> clip_below_3(int cell, Mesh_Kokkos const &mesh,
                              Line const &interface);

}