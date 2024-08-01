#include "clippings_kokkos.h"
#include "geometry.h"
#include <omp.h>
#include <Kokkos_Core.hpp>

// Method 3: Clipping Using Degrees /////////////////////////////////////////////////////
std::vector<int> polyintersect::clip_below_3(int cell, Mesh_Kokkos const &mesh,
                                                std::array<Point, 2> const &interface,
                                                bool print) {

  // Store all Points in a single list
  auto allPoints = mesh.list_of_points(cell, interface);
  //printf("All Points Size - %d\n", allPoints.size());

  // Find Center Point and Sort from Least to Greatest Degree
  Point centerPoint = center(allPoints);
  sorting(allPoints, centerPoint);

  // Store the Orientation of every node
  auto const sign = orientation_clip(allPoints, interface);
  int const n = allPoints.size();
  std::vector<int> belowline(n);

  // Clip below:
  int count = 0;
  for (int p = 0; p < n; p++) {
    if (sign[p] <= 0) {
      belowline[count++] = p;
    }
  }

  belowline.resize(count);

  // if (print) {
  //   #pragma omp critical
  //   {
  //     std::cout << "\nCell " << cell << ": " << std::endl;
  //     for (const auto &b: belowline) {        // method 3
  //       std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
  //     }
  //   }
  // }
  return belowline;
}




