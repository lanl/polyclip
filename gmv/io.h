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
