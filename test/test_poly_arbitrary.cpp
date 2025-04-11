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

    int total_cells = 10;
    int max_edges_per_cell = 6;
    int total_points = 17;
    double const tolerance = std::stod(argv[1]);
    int const total_lines = 10;

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
    Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell, total_lines);

    int vertices[total_lines] = { 5, 3, 6, 4, 3, 5, 3, 4, 4, 4 };
    double mixed[total_lines] = { -0.375, -0.4419417382415923,
                         	  -0.5,   -0.8838834764831844,
                	          -2,     -2,
                        	  -2,     -2,
                        	  -2,     -2 };

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

    // New points
    mesh.add_points(11, { 0.0, 0.375 });
    mesh.add_points(12, { 0.0, 0.625 });
    mesh.add_points(13, { 0.25, 0.625 });
    mesh.add_points(14, { 0.0, 0.875 });
    mesh.add_points(15, { 1.0, 0.0 });
    mesh.add_points(16, { 1.0, 0.875 });

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

    // New Shapes ///////////////////////////
    mesh.add_edge(4, 0, { 4, 3 }); // Cell 4
    mesh.add_edge(4, 1, { 3, 11 });
    mesh.add_edge(4, 2, { 11, 4 });

    mesh.add_edge(5, 0, { 11, 3 }); // Cell 5
    mesh.add_edge(5, 1, { 3, 8 });
    mesh.add_edge(5, 2, { 8, 13 });
    mesh.add_edge(5, 3, { 13, 12 });
    mesh.add_edge(5, 4, { 12, 11 });

    mesh.add_edge(6, 0, { 12, 13 }); // Cell 6
    mesh.add_edge(6, 1, { 13, 14 });
    mesh.add_edge(6, 2, { 14, 12 });

    mesh.add_edge(7, 0, { 13, 8 }); // Cell 7
    mesh.add_edge(7, 1, { 8, 10 });
    mesh.add_edge(7, 2, { 10, 14 });
    mesh.add_edge(7, 3, { 14, 13 });

    mesh.add_edge(8, 0, { 1, 15 }); // Cell 8
    mesh.add_edge(8, 1, { 15, 6 });
    mesh.add_edge(8, 2, { 6, 5 });
    mesh.add_edge(8, 3, { 5, 1 });

    mesh.add_edge(9, 0, { 6, 16 }); // Cell 9
    mesh.add_edge(9, 1, { 16, 9 });
    mesh.add_edge(9, 2, { 9, 7 });
    mesh.add_edge(9, 3, { 7, 6 });

    // Declare the Number of Vertices
    for (int i = 0; i < total_cells; i++) {
      mesh.mirror_num_verts_per_cell_(i) = vertices[i];
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
        if (i == 0) { // Vertical Lines
          clipped_part.line_(i).n = { 1.0, 0.0 };
          clipped_part.line_(i).d = mixed[i];
        } else if (i == 1 || i == 3) { // Arbitrary Lines
          clipped_part.line_(i).n = { 0.70710678, 0.70710678 };
          clipped_part.line_(i).d = mixed[i];
        } else {
          clipped_part.line_(i).n = { 0.0, 1.0 }; // Horizontal Line
          clipped_part.line_(i).d = mixed[i];
        }
      });

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
 /*   clip(total_cells, mesh.device_points_, mesh.device_cells_,
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
    double x[8] = { 0.375, 0.375, 0.575, 0.5, 0.9375, 0.375, 0.625, 0.5 };
    double y[8] = { 0, 0.3125, 0.05, 0.125, 0.5, 0.5, 0.625, 0.75 };
    verify_intersection_points(total_cells, clipped_part, x, y, tolerance);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
