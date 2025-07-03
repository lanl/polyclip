
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
#include <sstream>

using namespace polyclip;
int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: test_mesh [MESH_FILE] [LINE_FILE] [TOTAL_LINES]";
    return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {
    std::string const file_name = argv[1];
    std::string const lines = argv[2];
    Mesh_Kokkos mesh = io::read_mesh(argv[1]);

    int const max_edges_per_cell = 8;
    int const n_cells = static_cast<int>(mesh.mirror_cells_.extent(0));
    int const n_points = static_cast<int>(mesh.mirror_points_.extent(0));
    
    int n_lines;
    std::istringstream iss(argv[3]);
    iss >> n_lines;

    Clipped_Part clipped_part(n_points, n_cells, max_edges_per_cell, n_lines);
    io::read_lines(clipped_part, lines);
    clipped_part.send_to_gpu();

    Kokkos::Profiling::pushRegion("GENERATING MESH");

    mesh.send_to_gpu();
    Kokkos::Profiling::popRegion();
    Kokkos::Profiling::pushRegion("CLIPPING BELOW CELLS");
    clip(n_cells, n_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
         clipped_part.clipped_cell_);
    Kokkos::Profiling::popRegion();

    Kokkos::Profiling::pushRegion("MESH: GPU-TO-CPU TRANSFER");
    mesh.send_to_cpu();
    Kokkos::Profiling::popRegion();
    Kokkos::Profiling::pushRegion("CLIPPED PART: GPU-TO-CPU TRANSFER");
    clipped_part.send_to_cpu();
    Kokkos::Profiling::popRegion();

    //////////////////////////// Visual Test /////////////////////////////////
    // GMV variable counters
    int num_nodes = 0;
    int num_polys = 0;

    // GMV counter
    for (int c = 0; c < n_cells; c++) { //Increase at every cell
      int below = clipped_part.mirror_size_output_(c, 0);
      num_nodes += mesh.mirror_num_verts_per_cell_(c);
      num_polys++;
      if (below > 0) { //Increase at every clipped cell
        num_nodes += 2;
        num_polys++;
      }
    }

    io::write_clipped(mesh, clipped_part, num_nodes, num_polys,
                      "test_clipped.gmv");
    io::write_mesh(mesh, "test_mesh.gmv");
  }
  Kokkos::finalize();
  return 0;
}
