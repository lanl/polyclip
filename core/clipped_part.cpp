#include "clipped_part.h"

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
Clipped_Part::Clipped_Part(int total_points,
                           int total_cells,
                           int max_edges_per_cell) {
  Kokkos::resize(line_, total_cells); // malloc
  Kokkos::resize(intersect_points_, total_cells);
  Kokkos::resize(output_, total_cells, 2,
                 (max_edges_per_cell + 2));     // 0 = below and 1 = above
  Kokkos::resize(size_output_, total_cells, 2); // 0 = below and 1 = above
  Kokkos::resize(allPoints_, total_cells, (max_edges_per_cell + 2));

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
} // namespace polyintersect
