#pragma once

#include "mesh_gpu.h"
#include "geometry_gpu.h"
#include <map>

namespace polyintersect {

    Line intersect_cell_with_line(Mesh_Kokkos const& mesh,
                                  int c,
                                  Line const& line);
}
