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

    void init(int n_nodes, 
              int n_cells,
              double lengthPerAxis, 
              Kokkos::View<Point*> points,
              Kokkos::View<int**> cells);

    // mesh data members
    Kokkos::View<Point*, Kokkos::CudaSpace> points_;
    Kokkos::View<int**, Kokkos::CudaSpace> cells_;
    
    Kokkos::View<Point*>::HostMirror mirror_points_;
    Kokkos::View<int**,>::HostMirror mirror_cells_;
  };
}
