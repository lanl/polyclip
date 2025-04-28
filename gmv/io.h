#pragma once
#include <string>
#include "mesh.h"
#include "clipped_part.h"

namespace polyclip::io {
void write_clipped(Mesh_Kokkos mesh,
                   Clipped_Part clipped_part,
                   int num_total_nodes,
                   int num_total_polys,
                   std::string const& file_name);
void write_mesh(Mesh_Kokkos mesh, std::string const& file_name);
Mesh_Kokkos read_mesh(std::string const& file_name);
void read_lines(Clipped_Part& clips, const std::string& file_name);
} // namespace polyclip::io
