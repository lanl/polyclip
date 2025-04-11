#include "clippings.h"
#include "mesh.h"
#include "print.h"
#include "clip.h"
#include "clipped_part.h"
#include "intersect_n_d.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "timer.h"
#include "test_predicates.h"

int main(int argc, char* argv[]) {
  using namespace polyclip;

  Kokkos::initialize(argc, argv);
  {
    // initialize variables for the unstructured mesh

    int n_cells = 4;
    int total_cells = n_cells * n_cells;
    int max_edges_per_cell = 4;
    int n_nodes = n_cells + 1;
    int total_points = n_nodes * n_nodes;
    double const tolerance = std::stod(argv[1]);
    int const total_lines = 1;

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
    Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell, total_lines);
    double horizontal[total_lines] = { -0.625};

    // All Nodes
    double lengthPerAxis = 1.0;
    double dy = lengthPerAxis / n_cells;
    double dx = dy;

    for (int j = 0; j < n_nodes; j++) {
      for (int i = 0; i < n_nodes; i++) {
        int k = i + (j * n_nodes); // Identify the elements
        double x = (i * dx);
        double y = (j * dy);

        mesh.add_points(k, { x, y });
      }
    }
    std::cout << std::endl;

    // All Cells
    for (int j = 0; j < n_cells; j++) {   // Cell Rows
      for (int i = 0; i < n_cells; i++) { // Cell Columns
        int k = i + (j * n_cells); // Identify the individual cell elements
        int a = (k + j);
        int b = ((k + 1) + j);
        int c = (((k + 1) + n_nodes) + j);
        int d = (k + n_nodes + j);

        mesh.add_edge(k, 0, { a, b });
        mesh.add_edge(k, 1, { b, c });
        mesh.add_edge(k, 2, { c, d });
        mesh.add_edge(k, 3, { d, a });
      }
    }

    // All Vertices
    for (int i = 0; i < total_cells; i++) {
      mesh.mirror_num_verts_per_cell_(i) = 4;
    }

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
    auto start = timer::now();

    // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
    Kokkos::parallel_for(
      total_lines, KOKKOS_LAMBDA(int i) {
        clipped_part.line_(i).n = { 0.0, 1.0 }; // Horizontal Lines
        clipped_part.line_(i).d = horizontal[i];
      });

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
  /*  clip(total_cells, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_);
*/
    auto const end = timer::elapsed(start); // time deep copy

    // Send to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();

    auto const end_including_copy = timer::elapsed(start);

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    // Intersect Points
    double x[8] = { 0.25, 0, 0.5, 0.25, 0.75, 0.5, 1.0, 0.75 };
    double y[8] = { 0.625, 0.625, 0.625, 0.625, 0.625, 0.625, 0.625, 0.625 };
    verify_intersection_points(total_cells, clipped_part, x, y, tolerance);

    // Output Points
    int below_index[4] = { 0, 1, 2, 5 };
    int above_index[4] = { 2, 3, 4, 5 };
    verify_clipped_polys(total_cells, clipped_part, above_index, below_index);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
