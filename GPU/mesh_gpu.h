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


    void init();
    void list_of_points(int cell, 
                        Kokkos::View<Line*> const &line,
                        Kokkos::View<Point[6]> list) const;

    // mesh data members
    int n_nodes_ = 0;
    int n_cells_ = 0;
    double length_per_axis_ = 0.;
    Kokkos::View<Point*, Kokkos::CudaSpace> points_;
    Kokkos::View<int**, Kokkos::CudaSpace> cells_;
    
    Kokkos::View<Point*>::HostMirror mirror_points_;
    Kokkos::View<int**,>::HostMirror mirror_cells_;
  };
}
