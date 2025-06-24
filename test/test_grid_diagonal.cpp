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
  if (argc < 4) {
    std::cerr << "Usage: test_clip_poly_legacy [LINE_TYPE] [TOL] [LINE_FILE]";
    return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {
    int const max_edges_per_cell = 4;
    int const n_cells = 4 * 4;
    int const n_points = 5 * 5;
    int const n_lines = 1;
    // (0) goes through nodes (1) doesn't go through nodes
    int line_rep = std::stoi(argv[1]);
    double const tolerance = std::stod(argv[2]);
    std::string const lines = argv[3];
    std::string output;

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh = io::read_mesh("mesh/grid.gmv");
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

    // GPU to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();

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

    verify_intersection_points(n_cells, clipped_part, x.data(), y.data(),
                               tolerance);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
