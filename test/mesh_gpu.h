#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry_gpu.h"

namespace polyintersect {

// Mesh containing the coordinates and the cell
  class Mesh_Kokkos {
  public:
    // constructor
    Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell);

    // destructor
    ~Mesh_Kokkos() = default;

    // create list of all the points and create cells
    void add_all_points(int point_index, Point coordinates, Kokkos::View<Point*> host_points_);
    void add_edge(int cell_index, int edge, Edge const& points, Kokkos::View<int***> host_cells_);
    void send_to_gpu( Kokkos::View<int***> host_cells_, Kokkos::View<Point*> host_points_, Kokkos::View<int***> device_cells_, Kokkos::View<Point*> device_points_ );

    // mesh data members for host and device
    Kokkos::View<Point*> host_points_;
    Kokkos::View<int***> host_cells_;

    Kokkos::View<Point*, Kokkos::CudaSpace> device_points_;
    Kokkos::View<int***, Kokkos::CudaSpace> device_cells_;

    // Check GPU Results
    Kokkos::View<Point*>::HostMirror mirror_points_;
    Kokkos::View<int***,>::HostMirror mirror_cells_;
  };
}


/*#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry_gpu.h"

namespace polyintersect {

// Mesh containing the coordinates and the cell
  class Mesh_Kokkos {
  public:
    // constructor
    Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell);

    // destructor
    ~Mesh_Kokkos() = default;

    // create list of all the points and create cells
    void add_all_points(int point_index, Point coordinates, Kokkos::View<Point*> points_);
    void add_edge(int cell_index, int edge, Edge const& points, Kokkos::View<int***> cells_);

    // mesh data members
    Kokkos::View<Point*, Kokkos::CudaSpace> points_;
    Kokkos::View<int***, Kokkos::CudaSpace> cells_;
    
    Kokkos::View<Point*>::HostMirror mirror_points_;
    Kokkos::View<int***,>::HostMirror mirror_cells_;
  };
}*/
