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
#include "../gmv/io.h"
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
    int const total_lines = 1;

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);

  Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell, total_lines);

    if(argc < 4) {
      std::cout << "Usage: test_clip_poly_legacy [LINE_TYPE] [TOLERANCE] [LINE_FILE_NAME]";
      exit(1);
    }

    bool debug_flag = false;
    if(argc >= 5)
      debug_flag = true;

    std::string file_name = argv[3];
    std::string output;

    double arbitrary[2] = { 0.0, -0.08838834765 };
    int line_rep = std::stoi(
      argv[1]); // (0) Line goes through nodes (1) Line doesnt go through nodes
    double const tolerance = std::stod(argv[2]);

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


      io::read_lines(clipped_part, file_name);
      clipped_part.send_to_gpu();


    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    clip(total_cells, total_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
	 clipped_part.clipped_cell_);

    auto const end = timer::elapsed(start); // time deep copy

    // Send to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();
    auto const end_including_copy = timer::elapsed(start);

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    // Intersect Points
    std::vector<double> x, y;
    if (line_rep == 0) {
      x = { 0,   0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5,  0.5,  0.5,
            0.5, 0.5,  0.5,  0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 1.0 };
      y = { 0,   0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5,  0.5,  0.5,
            0.5, 0.5,  0.5,  0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 1.0 };
    } else {
      x = { 0.125, 0,     0.125, 0.25,  0.375, 0.25,  0.375,
            0.5,   0.625, 0.5,   0.625, 0.75,  0.875, 0.75 };
      y = { 0.25,  0.125, 0.25,  0.375, 0.5,   0.375, 0.5,
            0.625, 0.75,  0.625, 0.75,  0.875, 1.0,   0.875 };
    }

    verify_intersection_points(total_cells, clipped_part, x.data(), y.data(),
                               tolerance);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
