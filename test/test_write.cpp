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

#include "clippings.h"
#include "io.h"
#include "clip.h"
#include "test_predicates.h"
#include <iostream>

int main(int argc, char* argv[]) {
  using namespace polyclip;

  Kokkos::initialize(argc, argv);
  {
    // GMV variable counters
    int num_nodes = 0;
    int num_polys = 0;

    // initialize variables for the unstructured mesh
    int const total_points = 11;
    int const total_cells = 4;
    int const max_edges_per_cell = 6;
    int const total_lines = 2;

    // Testing: distances for every cell
    double vertical[total_lines] = { -0.375, -0.625 };

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
    Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell,
                              total_lines);

    // All Nodes
    mesh.add_points(0, { 0.0, 0.0 });
    mesh.add_points(1, { 0.5, 0.0 });
    mesh.add_points(2, { 0.5, 0.25 });
    mesh.add_points(3, { 0.25, 0.375 });
    mesh.add_points(4, { 0.0, 0.25 });
    mesh.add_points(5, { 0.875, 0.25 });
    mesh.add_points(6, { 1.0, 0.375 });
    mesh.add_points(7, { 0.875, 0.625 });
    mesh.add_points(8, { 0.5, 0.625 });
    mesh.add_points(9, { 0.5, 0.875 });
    mesh.add_points(10, { 0.875, 0.875 });

    // Pentagon
    mesh.add_edge(0, 0, { 0, 1 });
    mesh.add_edge(0, 1, { 1, 2 });
    mesh.add_edge(0, 2, { 2, 3 });
    mesh.add_edge(0, 3, { 3, 4 });
    mesh.add_edge(0, 4, { 4, 0 });

    // Triangle
    mesh.add_edge(1, 0, { 1, 5 });
    mesh.add_edge(1, 1, { 5, 2 });
    mesh.add_edge(1, 2, { 2, 1 });

    // Hexagon
    mesh.add_edge(2, 0, { 3, 2 });
    mesh.add_edge(2, 1, { 2, 5 });
    mesh.add_edge(2, 2, { 5, 6 });
    mesh.add_edge(2, 3, { 6, 7 });
    mesh.add_edge(2, 4, { 7, 8 });
    mesh.add_edge(2, 5, { 8, 3 });

    // Square
    mesh.add_edge(3, 0, { 8, 7 });
    mesh.add_edge(3, 1, { 7, 10 });
    mesh.add_edge(3, 2, { 10, 9 });
    mesh.add_edge(3, 3, { 9, 8 });

    // Declare the Number of Vertices
    mesh.mirror_num_verts_per_cell_(0) = 5;
    mesh.mirror_num_verts_per_cell_(1) = 3;
    mesh.mirror_num_verts_per_cell_(2) = 6;
    mesh.mirror_num_verts_per_cell_(3) = 4;

    // CPU to GPU
    mesh.send_to_gpu();

    // Max Threads and Timer
#if defined(KOKKOS_ENABLE_CUDA)
    int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
#elif defined(KOKKOS_ENABLE_OPENMP)
    int max_threads = omp_get_max_threads();
#else
    int max_threads = 1;
#endif

    // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
    Kokkos::parallel_for(
      total_lines, KOKKOS_LAMBDA(int i) {
        clipped_part.line_(i).n = { 1.0, 0.0 };
        clipped_part.line_(i).d = vertical[i];
      });

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    clip(total_cells, total_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
         clipped_part.clipped_cell_);

    mesh.send_to_cpu();
    clipped_part.send_to_cpu();

    // GMV counter
    for (int c = 0; c < total_cells; c++) { //Increase at every cell
      int below = clipped_part.mirror_size_output_(c, 0);
      num_nodes += mesh.mirror_num_verts_per_cell_(c);
      num_polys++;
      if (below > 0) { //Increase at every clipped cell
        num_nodes += 2;
        num_polys++;
      }
    }

    assert_eq(num_nodes, 26, "write clipped: num_nodes");
    assert_eq(num_polys, 8, "write clipped: num_polys");
    io::write_clipped(mesh, clipped_part, num_nodes, num_polys,
                      "test_clipped.gmv");
    io::write_mesh(mesh, "test_mesh.gmv");
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
