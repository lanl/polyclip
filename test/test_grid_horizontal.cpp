#include "../core/clippings.h"
#include "../core/mesh.h"
#include "../core/print.h"
#include "../core/clip.h"
#include "../core/clipped_part.h"
//#include "../core/intersect.h"
#include "../core/intersect_n_d.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

int main(int argc, char* argv[]) {
  using namespace polyintersect;

  Kokkos::initialize(argc, argv);
  {
    // initialize variables for the unstructured mesh

    int n_cells = 4;
    int total_cells = n_cells * n_cells;
    int max_edges_per_cell = 4;
    int n_nodes = n_cells + 1;
    int total_points = n_nodes * n_nodes;

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
    Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell);
    double horizontal[16] = { -2,     -2,     -2,     -2,     -2, -2, -2, -2,
                              -0.625, -0.625, -0.625, -0.625, -2, -2, -2, -2 };

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
    int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
    auto start = timer::now();

    // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
    Kokkos::parallel_for(
      total_cells, KOKKOS_LAMBDA(int i) {
        clipped_part.line_(i).n = { 0.0, 1.0 }; // Horizontal Lines
        clipped_part.line_(i).d = horizontal[i];
      });

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    clip(total_cells, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_);

    auto const end = timer::elapsed(start); // time deep copy

    // Send to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();

    auto const end_including_copy = timer::elapsed(start);

    // Verify Results by Printing on the CPU ////////////////////////////////////////////////////////////////
    print_results(end, end_including_copy, max_threads, total_cells,
                  total_points, mesh.mirror_points_, mesh.mirror_cells_,
                  clipped_part.mirror_intersect_points_,
                  clipped_part.mirror_line_, mesh.mirror_num_verts_per_cell_,
                  clipped_part.mirror_allPoints_,
                  clipped_part.mirror_size_output_, clipped_part.mirror_output_,
                  mesh.mirror_signs_);

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    // Intersect Points
    double x_value[8] = { 0.25, 0, 0.5, 0.25, 0.75, 0.5, 1.0, 0.75 };
    double y_value = 0.625;
    int counter = 0;

    for (int i = 0; i < total_cells; i++) {
      if (clipped_part.mirror_intersect_points_(i).a.x != DBL_MAX) {
        assert_equal(clipped_part.mirror_intersect_points_(i).a.x,
                     x_value[counter],
                     "Intersect A x_value at Cell " + std::to_string(i) + ": ");
        assert_equal(clipped_part.mirror_intersect_points_(i).a.y, y_value,
                     "Intersect A y_value at Cell " + std::to_string(i) + ": ");

        assert_equal(clipped_part.mirror_intersect_points_(i).b.x,
                     x_value[counter + 1],
                     "Intersect B x_value at Cell " + std::to_string(i) + ": ");
        assert_equal(clipped_part.mirror_intersect_points_(i).b.y, y_value,
                     "Intersect B y_value at Cell " + std::to_string(i) + ": ");

        counter += 2;
      }
    }
    std::cout << "100% Intersecting Points Match" << std::endl;

    // Output Points
    int below_index[4] = { 0, 1, 2, 5 };
    int above_index[4] = { 2, 3, 4, 5 };

    for (int c = 0; c < total_cells; c++) {
      int below = clipped_part.mirror_size_output_(c, 0);
      int above = clipped_part.mirror_size_output_(c, 1);
      for (int i = 0; i < below; i++) {
        int const j = clipped_part.mirror_output_(c, 0, i);
        assert_equal(j, below_index[i],
                     "Ouput Index at Cell " + std::to_string(c) + ": ");
      }
      for (int i = 0; i < above; i++) {
        int const j = clipped_part.mirror_output_(c, 1, i);
        assert_equal(j, above_index[i],
                     "Ouput Index at Cell " + std::to_string(c) + ": ");
      }
    }
    std::cout << "100% Output Match" << std::endl;
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
