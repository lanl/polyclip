
#include "mesh.h"
#include "clip.h"
#include "clipped_part.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include "timer.h"
#include "test_predicates.h"
#include "io.h"

int main(int argc, char* argv[]) {
  using namespace polyclip;
  if (argc < 3) {
    std::cerr << "Usage: test_clip_poly_arbitrary [TOL] [LINE_FILE]";
    return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {
    int const max_edges_per_cell = 6;
    int const n_cells = 10;
    int const n_points = 17;
    int const n_lines = 2;
    double const tolerance = std::stod(argv[1]);
    std::string const lines = argv[2];

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh = io::read_mesh("mesh/poly_square.gmv");
    mesh.send_to_gpu();

    Clipped_Part clipped_part(n_points, n_cells, max_edges_per_cell, n_lines);
    io::read_lines(clipped_part, lines);
    clipped_part.send_to_gpu();

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    clip(n_cells, n_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
         clipped_part.clipped_cell_);

    // Send to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    // Intersect Points
    double x[12] = { 0.375, 0.375, 0.375, 0.375, 0.625, 0.5,
                     0.375, 0.375, 0.375, 0.375, 1.0,   0.9375 };
    double y[12] = { 0,   0.3125, 0.3125, 0.5,   0.625, 0.75,
                     0.5, 0.625,  0.625,  0.875, 0.25,  0.3125 };
    verify_intersection_points(n_cells, clipped_part, x, y, tolerance);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
