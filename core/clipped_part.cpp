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
#include "clipped_part.h"

namespace polyclip {

// Create the Mesh //////////////////////////////////////////////////////////////////////
Clipped_Part::Clipped_Part(int total_points,
                           int total_cells,
                           int max_edges_per_cell,
                           int total_lines) {
  Kokkos::resize(line_, total_lines); // malloc
  Kokkos::resize(intersect_points_, total_cells);
  Kokkos::resize(output_, total_cells, 2,
                 (max_edges_per_cell + 2));     // 0 = below and 1 = above
  Kokkos::resize(size_output_, total_cells, 2); // 0 = below and 1 = above
  Kokkos::resize(allPoints_, total_cells, (max_edges_per_cell + 2));
  Kokkos::resize(clipped_cell_, total_cells);

  // CPU data members
  mirror_output_ = Kokkos::create_mirror_view(output_);
  mirror_size_output_ = Kokkos::create_mirror_view(size_output_);
  mirror_allPoints_ = Kokkos::create_mirror_view(allPoints_);
  mirror_intersect_points_ = Kokkos::create_mirror_view(intersect_points_);
  mirror_line_ = Kokkos::create_mirror_view(line_);
}

// GPU to CPU ///////////////////////////////////////////////////////////////////////////////////////
void Clipped_Part::send_to_cpu() {
  Kokkos::deep_copy(mirror_output_, output_);
  Kokkos::deep_copy(mirror_size_output_, size_output_);
  Kokkos::deep_copy(mirror_allPoints_, allPoints_);
  Kokkos::deep_copy(mirror_intersect_points_, intersect_points_);
  Kokkos::deep_copy(mirror_line_, line_);
}
void Clipped_Part::send_to_gpu() {
  Kokkos::deep_copy(line_, mirror_line_);
}
} // namespace polyclip
