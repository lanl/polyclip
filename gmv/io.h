//
// Created by Jonathan Mathurin on 2/16/25.
//
#pragma once
#include <string>
#include "../core/mesh.h"
#include "../core/clipped_part.h"
//#include "../test/mesh.h"
//#include "../test/clipped_part.h"

namespace polyintersect::io {

  void write_clipped(Mesh_Kokkos mesh, Clipped_Part clipped_part, int num_total_nodes, int num_total_polys, const std::string& file_name);
  void write_mesh(Mesh_Kokkos mesh, const std::string& file_name);
  Mesh_Kokkos read_mesh(std::string& file_name);

}
