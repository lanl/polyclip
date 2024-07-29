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

    void list_of_points(int cell, Line const &line,
                        Kokkos::View<Point[6]> list) const;

    // data members
    Kokkos::View<Point*> points_;
    Kokkos::View<int**> cells_;
  };
}