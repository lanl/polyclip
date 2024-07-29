#include "geometry_gpu.h"

/*
    Code Description:
        - Calculates the normal vector of the interface.
        - Calculates the direction vector with respect to (WRT) the normal vector.
        - Calculates the dot product of the normal and direction vector.
            - This will indicate the sign of the node. 
*/

// DONE
// Finding the normal vector between 2 Points ///////////////////////////////////////////
polyintersect::Point polyintersect::normVec(Line const &line) {
  // Direction vec
  double dx = line.b.x - line.a.x;     // x2 - x1
  double dy = line.b.y - line.a.y;     // y2 - y1

  // Normal vec
  return {dy, -dx};
}

// DONE
// Finding the dot product of the array and vector //////////////////////////////////////
double polyintersect::dotProduct(Point const  &v,
                                 Point const &n) {
  double product = (v.x * n.x) + (v.y * n.y);
  return product;
}

// DONE
// Point Vector /////////////////////////////////////////////////////////////////////////
polyintersect::Point polyintersect::pointVec(Point const &p, Line const &line) {
  double dx = p.x - line.a.x;
  double dy = p.y - line.a.y;

  //std::array<double, 2> Point = {dx, dy};
  return {dx, dy};
}

// DONE
// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
void polyintersect::orientation_clip(Kokkos::View<Point[6]> const &allPoints,
                                     Line line, int signs[6]) {

  // Deduce the normal vector of the cutting line
  auto normal = normVec(line);
  //int signs[6];
  //std::array<int, 6> signs{};
  int index = 0;
  double dp;

  for(int p = 0; p < 6; p++){ //(const auto &p: allPoints) {
    // Vector of Node
    auto const V = pointVec(allPoints(p), line);

    // Dot Product of normal and node vector
    dp = dotProduct(normal, V);

    // Convection of placement with respect to the line
    if (dp < 0) {           // Below the line
      signs[index] = -1;
    } else if (dp > 0) {    // Above the line
      signs[index] = 1;
    } else {                // On the line
      signs[index] = 0;
    }
    index++;
  }

  //return signs;
}

// HELP --SOLVE LATER
// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void polyintersect::sorting(Kokkos::View<Point[6]> &nodes, Point const &center) {
  std::sort(&nodes(0), &nodes(0) + 6, [&center](const Point &a, const Point &b) {
    double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));
    double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));
    return a1 < a2;
  });
}

// DONE
// Find the Center Coordinate ///////////////////////////////////////////////////////////
polyintersect::Point polyintersect::center(Kokkos::View<Point[6]> const &nodes) {
  double sumX = 0, sumY = 0;

  // Add up all the coordinates /////
  for(int p = 0; p < 6; p++){ //(const auto &p: nodes) {
    sumX += nodes(p).x;
    sumY += nodes(p).y;
  }
  double const n = nodes.size();

  // Store middle coordinates ///////
  return {sumX / n, sumY / n};
}