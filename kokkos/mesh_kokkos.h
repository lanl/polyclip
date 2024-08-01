#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "omp/geometry.h"

namespace polyintersect {

// Mesh containing the coordinates and the cell
  class Mesh_Kokkos {
  public:
    // constructor
    Mesh_Kokkos(int n_cells, double lengthPerAxis);

    // destructor
    ~Mesh_Kokkos() = default;

    std::vector<Point> list_of_points(int cell,
                                      std::array<Point, 2> const &line) const;

    // data members
    // REPLACED WITH KOKKOS VIEWS (NEW FILE) 
    Kokkos::View<Point*> points_;
    Kokkos::View<int**> cells_;
  };
}