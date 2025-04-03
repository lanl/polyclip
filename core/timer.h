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
