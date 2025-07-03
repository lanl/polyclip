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
#include "clipped_part.h"
#include <iomanip>

namespace polyclip {

void assert_eq(int a, int b, std::string const& label) {
  if (a != b) {
    std::cerr << label << " " << a << " != " << b << std::endl;
    Kokkos::finalize();
    std::exit(EXIT_FAILURE);
  }
}

bool assert_near(double a, double b, double tol, std::string const& label) {
  if (std::abs(a - b) > tol) {
    std::cout << label << " " << std::setprecision(15) << a << " != " << b
              << ", delta: " << std::abs(a - b) << ", tol: " << tol
              << std::endl;
    return false;
  }
  return true;
}

void verify_intersection_points(int total_cells,
                                Clipped_Part const& clipped_part,
                                double* x_expected,
                                double* y_expected,
                                double tolerance) {
  int counter = 0;
  int correct = 0;
  int total_intersect_points = 0;

  for (int i = 0; i < total_cells; i++) {
    auto const& a = clipped_part.mirror_intersect_points_(i).a;
    auto const& b = clipped_part.mirror_intersect_points_(i).b;
    if (a.x < DBL_MAX and a.y < DBL_MAX) {
      total_intersect_points += 4; 

      if(assert_near(a.x, x_expected[counter], tolerance,
                  "intersect: a.x at cell " + std::to_string(i) + ": ")){
	      correct++;
      }
      if(assert_near(a.y, y_expected[counter], tolerance,
                  "intersect: a.y at cell " + std::to_string(i) + ": ")){
	      correct++;
      }
      if(assert_near(b.x, x_expected[counter + 1], tolerance,
                  "intersect: b.x at cell " + std::to_string(i) + ": ")){
	      correct++;
      }
      if(assert_near(b.y, y_expected[counter + 1], tolerance,
                  "intersect: b.y at cell " + std::to_string(i) + ": ")){
	      correct++;
      }
      counter += 2;
    }
  }
  double accuracy = 100 * correct / total_intersect_points;

  std::cout << std::endl;
  std::cout << accuracy << "% intersection points match with tolerance: " << tolerance
            << std::endl;
}

void verify_clipped_polys(int total_cells,
                          Clipped_Part const& clipped_part,
                          int* above_index,
                          int* below_index) {
  for (int c = 0; c < total_cells; c++) {
    int below = clipped_part.mirror_size_output_(c, 0);
    int above = clipped_part.mirror_size_output_(c, 1);
    for (int i = 0; i < below; i++) {
      int const j = clipped_part.mirror_output_(c, 0, i);
      assert_eq(j, below_index[i],
                "ouput index at cell " + std::to_string(c) + ": ");
    }
    for (int i = 0; i < above; i++) {
      int const j = clipped_part.mirror_output_(c, 1, i);
      assert_eq(j, above_index[i],
                "ouput index at cell " + std::to_string(c) + ": ");
    }
  }
  std::cout << "100% clipped polys match" << std::endl;
}

} // namespace polyclip
