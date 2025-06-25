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
#include "test_predicates.h"
#include "intersect_n_d.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "timer.h"
#include "../core/clippings.h"
#include "../core/mesh.h"
#include "../core/print.h"
#include "../core/clip.h"
#include "../core/clipped_part.h"
#include "../core/timer.h"
#include "../gmv/io.h"

int main(int argc, char* argv[]) {
  using namespace polyclip;
  if (argc < 4) {
    std::cerr << "Usage: test_clip_poly_legacy [LINE_TYPE] [TOL] [LINE_FILE]";
    return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {
    int const max_edges_per_cell = 6;
    int const n_cells = 4;
    int const n_points = 11;
    // 1: horizontal| 2: vertical| 3: arbitrary
    int const line_rep = std::stoi(argv[1]);
    double const tolerance = std::stod(argv[2]);
    std::string const lines = argv[3];

    int n_lines = 0;
    switch (line_rep) {
      case 1:
        n_lines = 3;
        break;
      case 2:
        n_lines = 2;
        break;
      default:
        n_lines = 3;
        break;
    }

    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh = io::read_mesh("mesh/poly.gmv");
    mesh.send_to_gpu();

    // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////
    Kokkos::Profiling::pushRegion("INIT LINE INTERFACE");
    Clipped_Part clipped_part(n_points, n_cells, max_edges_per_cell, n_lines);
    io::read_lines(clipped_part, lines);
    clipped_part.send_to_gpu();
    Kokkos::Profiling::popRegion();

    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    Kokkos::Profiling::pushRegion("CLIPPING BELOW CELLS");
    clip(n_cells, n_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
         clipped_part.clipped_cell_);
    Kokkos::Profiling::popRegion();

    // Send to CPU
    Kokkos::Profiling::pushRegion("MESH: GPU-TO-CPU TRANSFER");
    mesh.send_to_cpu();
    Kokkos::Profiling::popRegion();

    Kokkos::Profiling::pushRegion("CLIPPED PART: GPU-TO-CPU TRANSFER");
    clipped_part.send_to_cpu();
    Kokkos::Profiling::popRegion();

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    std::array<double, 8> x{}, y{};
    if (line_rep == 1) {
      x = { 0.5, 0.0, 0.6875, 0.5, 0.9375, 0.375, 0.875, 0.5 };
      y = { 0.125, 0.125, 0.125, 0.125, 0.5, 0.5, 0.75, 0.75 };
    } else if (line_rep == 2) {
      x = { 0.375, 0.375, 0.625, 0.625, 0.375, 0.375, 0.625, 0.625 };
      y = { 0.0,   0.3125, 0.0833333333333333, 0.25, 0.312500000000000, 0.5,
            0.625, 0.875 };
    } else {
      x = { 0.375000000629262, 0.083333333752841, 0.725000000880966,
            0.625000001468277, 0.625000001468277, 0.375000000734139,
            0.625000002097539, 0.500000000000000 };
      y = { 0.000000000000000, 0.291666666876421, 0.150000000587311,
            0.250000000000000, 0.250000000000000, 0.500000000734139,
            0.625000000000000, 0.750000002097539 };
    }

    verify_intersection_points(n_cells, clipped_part, x.data(), y.data(),
                               tolerance);
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
