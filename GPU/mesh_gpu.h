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

    void list_of_points(int cell, 
                        Kokkos::View<Line*> const &line,
                        Kokkos::View<Point[6]> list) const;

    // mesh data members
    //Kokkos::View<Point*> points_;
    Kokkos::View<Point*, Kokkos::CudaSpace> points_;
    Kokkos::View<Point*> host_points_ = Kokkos::create_mirror_view(points_);

    //Kokkos::View<int**> cells_;
    Kokkos::View<int**> cells_;
    Kokkos::View<int**> host_cells_ = Kokkos::create_mirror_view(cells_);
  };
}