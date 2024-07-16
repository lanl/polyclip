#pragma once

#include "Mesh.h"
#include <map>

namespace polyintersect {

std::array<Point, 2> intersect_cell_with_line(Mesh const& mesh,
                                              int c,
                                              std::array<Point, 2> const& line,
                                              std::map<std::pair<int, int>, int>& mapping);
}
