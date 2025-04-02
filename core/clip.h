#pragma once //prevent headers being used twice

#include "mesh.h"
#include "intersect_n_d.h"

namespace polyintersect {

void clip(int total_cells,
          Kokkos::View<Point*> device_points_,
          Kokkos::View<int***> device_cells_,
          Kokkos::View<Segment*> intersect_points_,
          Kokkos::View<Line*> line_,
          Kokkos::View<int*> num_verts_per_cell_,
          Kokkos::View<Point**> allPoints_,
          Kokkos::View<int**> size_output_,
          Kokkos::View<int***> output_,
          Kokkos::View<int**> signs_) {
  Kokkos::parallel_for(
    total_cells, KOKKOS_LAMBDA(int c) {
      intersect_points_(c) = intersect_cell_with_line_n_d(
        device_points_, device_cells_, c, line_(c), num_verts_per_cell_);

      // Check if cell contains intersect points
      if (intersects(device_points_, device_cells_, c, intersect_points_(c),
                     num_verts_per_cell_)) {
        clip_below_3(c, device_points_, device_cells_, intersect_points_(c),
                     output_, size_output_, num_verts_per_cell_, signs_,
                     allPoints_, line_(c));
      }
    });
}

} // namespace polyintersect
