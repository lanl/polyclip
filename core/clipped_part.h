/*
 * (c) 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad National
 * Security, LLC for the U.S. Department of Energy/National Nuclear Security
 * Administration. All rights in the program are reserved by Triad National
 * Security, LLC, and the U.S. Department of Energy/National Nuclear Security
 * Administration. The Government is granted for itself and others acting on its
 * behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
 * to reproduce, prepare. derivative works, distribute copies to the public,
 * perform publicly and display publicly, and to permit others to do so.
 */
#pragma once //prevent headers being used twice

#include <Kokkos_Core.hpp>
#include <Kokkos_Vector.hpp>
#include "geometry.h"

namespace polyclip {

// Materials needed for the mesh
class Clipped_Part {
 public:
  // constructor
  Clipped_Part(int total_points,
               int total_cells,
               int max_edges_per_cell,
               int total_lines);

  // destructor
  ~Clipped_Part() = default;

  // Verify results
  void send_to_cpu();
  void send_to_gpu();
  // mesh data members for host and device
  Kokkos::View<Line*> line_;
  Kokkos::View<Segment*> intersect_points_;
  Kokkos::View<int***> output_;     // cell, above/below, edge
  Kokkos::View<int**> size_output_; // cell, above/below
  Kokkos::View<Point**> allPoints_; // cell, allPoints
  Kokkos::View<bool*> clipped_cell_;

  Kokkos::View<Line*>::HostMirror mirror_line_;
  Kokkos::View<Segment*>::HostMirror mirror_intersect_points_;
  Kokkos::View<int***>::HostMirror mirror_output_;
  Kokkos::View<int**>::HostMirror mirror_size_output_;
  Kokkos::View<Point**>::HostMirror mirror_allPoints_;
};
} // namespace polyclip
