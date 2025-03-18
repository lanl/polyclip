//
// Created by Jonathan Mathurin on 2/16/25.
//
#pragma once
#include <string>
#include "../test/mesh_gpu.h"
#include "../test/clipped_part_gpu.h"

namespace polyintersect::io {

  void write_clipped_gmv(Mesh_Kokkos mesh, Clipped_Part clipped_part, int num_total_nodes, int num_total_polys, const std::string& file_name);
  void write_gmv(Mesh_Kokkos mesh, const std::string& file_name);
  Mesh_Kokkos read_gmv(std::string& file_name);

}
