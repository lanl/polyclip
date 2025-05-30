
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

using namespace polyclip;
int main(int argc, char* argv[]){
  if (argc < 3) {
    std::cerr << "Usage: test_mesh [MESH_FILE] [LINE_FILE]";
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
    int const n_lines = 1;

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
  }
  Kokkos::finalize();
  return 0;
}