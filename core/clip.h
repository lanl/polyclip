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

#include "mesh.h"
#include "intersect_n_d.h"
#include "clippings.h"

namespace polyclip {

void clip(int total_cells,
          int total_lines,
          Kokkos::View<Point*> device_points_,
          Kokkos::View<int***> device_cells_,
          Kokkos::View<Segment*> intersect_points_,
          Kokkos::View<Line*> line_,
          Kokkos::View<int*> num_verts_per_cell_,
          Kokkos::View<Point**> allPoints_,
          Kokkos::View<int**> size_output_,
          Kokkos::View<int***> output_,
          Kokkos::View<int**> signs_,
          Kokkos::View<bool*> clipped_cell_) {
  for (int line = 0; line < total_lines; line++) {
    Kokkos::parallel_for(
      total_cells, KOKKOS_LAMBDA(int c) {
        if (!clipped_cell_(c)) {
          intersect_points_(c) = intersect_cell_with_line_n_d(
            device_points_, device_cells_, c, line_(line), num_verts_per_cell_);

          // Check if cell contains intersect points
          if ((intersects(device_points_, device_cells_, c,
                          intersect_points_(c), num_verts_per_cell_))) {
            clip_below_3(c, device_points_, device_cells_, intersect_points_(c),
                         output_, size_output_, num_verts_per_cell_, signs_,
                         allPoints_, line_(line));
            clipped_cell_(c) = true;
          }
        }
      });
  }
}

} // namespace polyclip
