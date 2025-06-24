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
#pragma once //prevent headers being used twice

#include <iostream>
#include <cstdio>
#include <string>
#include "geometry.h"
#include <iomanip>
#include <fstream>
#include <sstream>
namespace polyclip {

void print_results(int total_cells,
                   int total_points,
                   Kokkos::View<Point*>::HostMirror mirror_points_,
                   Kokkos::View<int***>::HostMirror mirror_cells_,
                   Kokkos::View<Segment*>::HostMirror mirror_intersect_points_,
                   Kokkos::View<Line*>::HostMirror mirror_line_,
                   Kokkos::View<int*>::HostMirror mirror_num_verts_per_cell_,
                   Kokkos::View<Point**>::HostMirror mirror_allPoints_,
                   Kokkos::View<int**>::HostMirror mirror_size_output_,
                   Kokkos::View<int***>::HostMirror mirror_output_,
                   Kokkos::View<int**>::HostMirror mirror_signs_,
                   std::string const& output_name = "") {
  if (output_name.empty()) {
    throw std::runtime_error("empty output file name");
  }

  std::ofstream output_file(output_name);
  output_file << "---------------- GPU Results ----------------" << std::endl;
  output_file << std::endl;

  output_file << "------ Cell + Edges ------" << std::endl;
  for (int j = 0; j < total_cells; j++) { // Cell
    output_file << "Cell " << j << ":" << std::endl;
    int t = mirror_num_verts_per_cell_(j);
    for (int i = 0; i < t; i++) { // Edge
      output_file << "Edge " << i << " (" << mirror_cells_(j, i, 0) << ", ";
      output_file << mirror_cells_(j, i, 1) << ") ";
    }
    output_file << std::endl;
  }
  output_file << std::endl;

  // Print Point Coordinates
  output_file << "------ Cell Vertices ------" << std::endl;
  for (int j = 0; j < total_points; j++) { // All Points
    output_file << "Point " << j << ": (" << mirror_points_(j).x << ", "
                << mirror_points_(j).y << ")" << std::endl;
  }

  // Print Line
  output_file << std::endl;
  output_file << "------ Line ------" << std::endl;
  for (int j = 0; j < total_cells; ++j) {
    auto const pa = mirror_line_(j).n;
    auto const dist = mirror_line_(j).d;
    output_file << "Line at Cell  " << j << ": normal = (" << pa.x << ", "
                << pa.y << ") and distance = " << dist << std::endl;
  }

  // Print Intersect Points
  output_file << std::endl;
  output_file << "------ Intersect Points ------" << std::endl;
  for (int j = 0; j < total_cells; ++j) {
    int below = mirror_size_output_(j, 0);
    if (below > 0) {
      auto const pa = mirror_intersect_points_(j).a;
      auto const pb = mirror_intersect_points_(j).b;
      output_file << "Intersection Points at Cell  " << j << ": (" << pa.x
                  << ", " << pa.y << "), (" << pb.x << ", " << pb.y << ")"
                  << std::endl;
    }
  }

  // Print all Points (Vertices + Intersect Points)
  output_file << std::endl;
  output_file << "------ All Points ------" << std::endl;
  for (int c = 0; c < total_cells; c++) {
    int t = mirror_num_verts_per_cell_(c) + 2;
    int below = mirror_size_output_(c, 0);
    if (below > 0) {
      for (int i = 0; i < t; i++) {
        auto const p = mirror_allPoints_(c, i);
        output_file << "Points at Cell  " << c << ": (" << p.x << ", " << p.y
                    << ") " << std::endl;
      }
      output_file << std::endl;
    }
  }

  // Output Results
  output_file << std::endl;
  output_file << "------ Output ------" << std::endl;
  for (int c = 0; c < total_cells; c++) {
    int below = mirror_size_output_(c, 0);
    int above = mirror_size_output_(c, 1);
    if (below > 0) {
      output_file << "Cell " << c << " Output: " << std::endl;
      for (int i = 0; i < below; i++) {
        int const j = mirror_output_(c, 0, i);
        auto const p = mirror_allPoints_(c, j);
        output_file << "Below " << j << ": (" << p.x << ", " << p.y << ") "
                    << std::endl;
      }
      for (int i = 0; i < above; i++) {
        int const j = mirror_output_(c, 1, i);
        auto const p = mirror_allPoints_(c, j);
        output_file << "Above " << j << ": (" << p.x << ", " << p.y << ") "
                    << std::endl;
      }
      output_file << std::endl;
    }
  }

  // Print signs
  output_file << "------ Signs ------" << std::endl;
  for (int i = 0; i < total_cells; i++) {
    int t = mirror_num_verts_per_cell_(i) + 2;
    for (int j = 0; j < t; j++) {
      output_file << "Sign at cell " << i << ": " << mirror_signs_(i, j)
                  << std::endl;
    }
    output_file << std::endl;
  }
}

} // namespace polyclip
