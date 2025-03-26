#include "geometry.h"

/*
    Code Description:
        - Calculates the normal vector of the interface.
        - Calculates the direction vector with respect to (WRT) the normal vector.
        - Calculates the dot product of the normal and direction vector.
            - This will indicate the sign of the node. 
*/


// Finding the normal vector between 2 Points ///////////////////////////////////////////
std::array<double, 2> polyintersect::normVec(Line const &line) {
  // Direction vec
  double dx = line[1].x - line[0].x;     // x2 - x1
  double dy = line[1].y - line[0].y;     // y2 - y1

  // Normal vec
  std::array<double, 2> normal = {dy, -dx};
  return normal;
}

// Finding the dot product of the array and vector //////////////////////////////////////
double polyintersect::dotProduct(std::array<double, 2> const &v,
                                 std::array<double, 2> const &n) {
  double product = (v[0] * n[0]) + (v[1] * n[1]);
  return product;
}

// Print the normal vector //////////////////////////////////////////////////////////////
void polyintersect::printNorm(std::array<double, 2> const &normal) {
  printf("Normal: ");
  for (int i = 0; i < 2; ++i) {
    printf("%.1lf ", normal[i]);
  }
  printf("\n");
}

// Point Vector /////////////////////////////////////////////////////////////////////////
std::array<double, 2> polyintersect::pointVec(const Point &p, Line const &line) {
  double dx = p.x - line[0].x;
  double dy = p.y - line[0].y;

  std::array<double, 2> Point = {dx, dy};
  return Point;
}

// Orientation of Every Node for Method 2 and 3 /////////////////////////////////////////
std::array<int, 6> polyintersect::orientation_clip(std::vector<Point> const& allPoints,
                                                   Line const& line) {

  // Deduce the normal vector of the cutting line
  std::array<double, 2> normal = normVec(line);
  std::array<int, 6> signs{};
  int index = 0;
  double dp;

  for (const auto &p: allPoints) {
    // Vector of Node
    auto const V = pointVec(p, line);

    // Dot Product of normal and node vector
    dp = dotProduct(normal, V);

    // Convection of placement with respect to the line
    if (dp < 0) {         // Below the line
      signs[index] = -1;
    } else if (dp > 0) {    // Above the line
      signs[index] = 1;
    } else {               // On the line
      signs[index] = 0;
    }
    index++;
  }

  return signs;
}

// Sort in Counter Clockwise manner Based on Degree /////////////////////////////////////
void polyintersect::sorting(std::vector<Point> &nodes, Point const &center) {
  std::sort(nodes.begin(), nodes.end(), [&center](const Point &a, const Point &b) {
    double a1 = (std::atan2(a.y - center.y, a.x - center.x) * (180 / M_PI));
    double a2 = (std::atan2(b.y - center.y, b.x - center.x) * (180 / M_PI));
    return a1 < a2;
  });
}

// Find the Center Coordinate ///////////////////////////////////////////////////////////
polyintersect::Point polyintersect::center(std::vector<Point> const &nodes) {
  std::vector<Point> result;
  double sumX = 0, sumY = 0;

  // Add up all the coordinates /////
  for (const auto &p: nodes) {
    sumX += p.x;
    sumY += p.y;
  }
  double const n = nodes.size();

  // Store middle coordinates ///////
  return {sumX / n, sumY / n};
}