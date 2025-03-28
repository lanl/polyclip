//
// Created by Jonathan Mathurin on 2/16/25.
//
#pragma once
#include <string>
#include "../core/mesh_gpu.h"
#include "../core/clipped_part_gpu.h"
//#include "../test/mesh_gpu.h"
//#include "../test/clipped_part_gpu.h"

namespace polyintersect::io {

  void write_gmv(Mesh_Kokkos mesh, Clipped_Part clipped_part, int num_total_nodes, int num_total_polys, const std::string& basename);
  Mesh_Kokkos read_gmv(std::string& file_name);
}
