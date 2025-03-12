#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry_gpu.h"

#define MAX_NUM_EDGES_PER_CELL 8
namespace polyintersect {

  // Mesh containing the coordinates and the cell
  class Mesh_Kokkos {
  public:

    //We need a regular constructor that doesn't initialize anything.
    Mesh_Kokkos();
    // constructor
    Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell);

    // destructor
    ~Mesh_Kokkos() = default;

    // create list of all the points and create cells
    void add_points(int point_index, Point coordinates);
    void add_edge(int cell_index, int edge, Edge const& points);
    void send_to_gpu();
    void send_to_cpu();

    // mesh data members for host and device
    Kokkos::View<Point*, Kokkos::CudaSpace> device_points_;
    Kokkos::View<int***, Kokkos::CudaSpace> device_cells_;  // Cell ID, Edge ID, Edge Point ID's
    Kokkos::View<Point*>::HostMirror mirror_points_;
    Kokkos::View<int***,>::HostMirror mirror_cells_;

    Kokkos::View<int*, Kokkos::CudaSpace>  num_verts_per_cell_;
    Kokkos::View<int**, Kokkos::CudaSpace> signs_;  // Cell ID, All Points ID (Vertices + Intersect points)
    Kokkos::View<int*>::HostMirror mirror_num_verts_per_cell_;
    Kokkos::View<int**,>::HostMirror mirror_signs_;

  };
}

