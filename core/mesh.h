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

#define MAX_NUM_EDGES_PER_CELL 8
namespace polyclip {

// Mesh containing the coordinates and the cell
class Mesh_Kokkos {
 public:
  //We need a regular constructor that doesn't initialize anything.
  Mesh_Kokkos() = default;
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
  Kokkos::View<Point*> device_points_;
  Kokkos::View<int***> device_cells_; // cell, edge, vertex
  Kokkos::View<Point*>::HostMirror mirror_points_;
  Kokkos::View<int***>::HostMirror mirror_cells_;

  Kokkos::View<int*> num_verts_per_cell_;
  Kokkos::View<int**> signs_; // cell, allPoints
  Kokkos::View<int*>::HostMirror mirror_num_verts_per_cell_;
  Kokkos::View<int**>::HostMirror mirror_signs_;
};
} // namespace polyclip
