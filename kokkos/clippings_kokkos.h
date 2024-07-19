#pragma once

#include "geometry.h"
#include "mesh_kokkos.h"

namespace polyintersect {
    
// Below the line using Degrees /////////////////////////////////////////////////////////
  std::vector<int> clip_below_3(int cell, Mesh_Kokkos const &mesh,
                                std::array<Point, 2> const &interface,
                                bool print);

}