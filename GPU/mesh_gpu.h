#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry_gpu.h"

namespace polyintersect {

// Mesh containing the coordinates and the cell
  class Mesh_Kokkos {
  public:
    // constructor
    Mesh_Kokkos(int n_cells, double lengthPerAxis);

    // destructor
    ~Mesh_Kokkos() = default;

    std::vector<Point> list_of_points(int cell,
                                      polyintersect::Line const &line) const;

    // data members
    Kokkos::View<Point*> points_;
    Kokkos::View<int**> cells_;
  };
}