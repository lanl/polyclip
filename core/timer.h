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

#include <chrono>
#include <cmath>
namespace timer {

inline std::chrono::high_resolution_clock::time_point now() {
  return std::chrono::high_resolution_clock::now();
}

inline float elapsed(std::chrono::high_resolution_clock::time_point& tic) {
  auto const toc = now();
  return std::chrono::duration_cast<std::chrono::microseconds>(toc - tic)
    .count();
}

double average(std::vector<double> const& durations) {
  int const n = durations.size();
  double result = 0.;
  for (int i = 0; i < n; i++) {
    result += durations[i];
  }
  result /= static_cast<double>(n);
  return result;
}

double deviation(std::vector<double> const& durations, double avg) {
  int const n = durations.size();
  double result = 0.;
  for (int i = 0; i < n; i++) {
    result += std::pow((durations[i] - avg), 2.);
  }
  result /= n;
  return std::sqrt(result);
}

} // namespace timer
