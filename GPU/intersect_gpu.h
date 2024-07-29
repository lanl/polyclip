#pragma once

#include "mesh_gpu.h"
#include "geometry_gpu.h"
#include <map>

namespace polyintersect {

// CANT BE AN ARRAY MUST BE STRUCTURE OF POINTS

std::array<Point, 2> intersect_cell_with_line(Mesh_Kokkos const& mesh,
                                              int c,
                                              polyintersect::Line const& line);
}
