#pragma once      //prevent headers being used twice

#include "geometry.h"

namespace polyintersect {

// Mesh containing the coordinates and the cell
  class Mesh {
  public:
    // constructor
    explicit Mesh(int n_cells, double lengthPerAxis);

    // destructor
    ~Mesh() = default;

    std::vector<Point> list_of_points(int cell,
                                      std::array<Point, 2> const &line) const;

    // data members
    // REPLACED WITH KOKKOS VIEWS (NEW FILE) 
    std::vector<Point> points_;
    std::vector<std::vector<int>> cells_;
  };
}
