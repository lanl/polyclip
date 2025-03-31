#pragma once      //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry.h"

namespace polyintersect {

// Materials needed for the mesh
  class Clipped_Part {
  public:
    // constructor
    Clipped_Part(int total_points, int total_cells, int max_edges_per_cell);

    // destructor
    ~Clipped_Part() = default;
    
    // Verify results
    void send_to_cpu();

    // mesh data members for host and device
    Kokkos::View<Line*, Kokkos::CudaSpace> line_;
    Kokkos::View<Segment*, Kokkos::CudaSpace> intersect_points_;
    Kokkos::View<int***, Kokkos::CudaSpace> output_;  // Cell ID, Above/Below ID, Edge ID
    Kokkos::View<int**, Kokkos::CudaSpace> size_output_; // Cell ID, Above/Below ID                                           
    Kokkos::View<Point**, Kokkos::CudaSpace> allPoints_;  // Cell ID, All Points Coordinate (Vertices + intersect points)
 
    Kokkos::View<Line*>::HostMirror mirror_line_;
    Kokkos::View<Segment*>::HostMirror mirror_intersect_points_;
    Kokkos::View<int***>::HostMirror mirror_output_;  // Cell ID, Edge ID, Above/Below ID
    Kokkos::View<int**>::HostMirror mirror_size_output_; // Cell ID, Above/Below ID                                           
    Kokkos::View<Point**>::HostMirror mirror_allPoints_;  // Cell ID, All Points Coordinate (Vertices + intersect points)

  };
}
