#pragma once

#include "mesh_kokkos.h"
#include <map>

namespace polyintersect {

std::array<Point, 2> intersect_cell_with_line(Mesh_Kokkos const& mesh,
                                              int c,
                                              std::array<Point, 2> const& line);
}
