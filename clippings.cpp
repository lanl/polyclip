#include "clippings.h"
#include "mesh.h"

/*
    Code Description:
        - This code contains 3 different clipping methods.
            1) Method 1: Clips using edges, compares the sign of every node,
               and stores as edges
                * But skips the the interface edge and uses maps.

            2) Method 2: Clips by Pointing to the next node based on the sign 
               comparison.
                * But it uses a map and a reverse map.

            3) Method 3: Finds the center Point of all the nodes, calculates 
               the degree of every node WTR to the center Point, and sorts based 
               on smallest to biggest degree. Uses the sorted value of all Points to 
               compare the signs.

*/


// Method 1: Clipping Using Edges ///////////////////////////////////////////////////////
std::vector<std::pair<int, int>> polyintersect::clip_below_1(int cell, Mesh const &mesh,
                                                             std::array<Point, 2> const &interface,
                                                             bool print,
                                                             std::map<std::pair<int, int>, int> const &intersectPoints) {


  std::vector<std::pair<int, int>> belowline;
  std::vector<std::pair<int, int>> edges = {{0, 1},
                                            {1, 2},
                                            {2, 3},
                                            {3, 0}};

  // Coordinates of the Cell Currently On
  std::vector<Point> nodes;
  for (const auto &c: mesh.cells_[cell]) {
    nodes.emplace_back(mesh.points_[c]);
  }

  auto const sign = orientation_clip(nodes, interface);

  int i, j;
  for (const auto &edge: edges) {
    i = edge.first;
    j = edge.second;

    // Checking all possibilities
    if (sign[i] == sign[j] && sign[i] < 0) {
      belowline.emplace_back(i, j);
    } else if (sign[i] < 0) {
      belowline.emplace_back(i, intersectPoints.at({i, j}));
    } else if (sign[j] < 0) {
      belowline.emplace_back(intersectPoints.at({i, j}), j);  // Fix the Flip
    }
  }

  if (print) {
    std::cout << "\nMethod 1: " << std::endl;
    for (const auto &b: belowline) {        // Method 1
      std::cout << "Edges: (" << b.first << ", " << b.second << ")" << std::endl;
    }
  }

  return belowline;
}

// Method 2 : Clipping by Pointing to the Next Node /////////////////////////////////////
std::vector<int> polyintersect::clip_below_2(int cell, Mesh const &mesh,
                                             std::array<Point, 2> const &interface,
                                             bool print,
                                             std::map<std::pair<int, int>, int> const &intersectPoints) {

  std::map<int, std::pair<int, int>> reverse_map;
  std::vector<int> belowline;
  std::array<double, 2> V{};

  // Store all Points in a single list
  auto const allPoints = mesh.list_of_points(cell, interface);

  for (auto [key, val]: intersectPoints) {
    reverse_map[val] = key;
  }

  std::array<int, 6> const sign = orientation_clip(allPoints, interface);

  int i = 0, j = 1;
  int const n = allPoints.size();

  do {
    belowline.emplace_back(i);

    // Checking all possibilities
    if (sign[i] == sign[j] && sign[i] < 0 || sign[j] < 0) {   // Below the Line
      i = j;
      j = (j + 1) % n;
    } else if (sign[i] < 0) {                                   // Detecting intersecting Points
      i = intersectPoints.at({i, j});
      j = i + 1;
    } else if (sign[i] == sign[j] && sign[i] == 0) {            // Detecting on the line
      i = j;
      j = reverse_map.at(i).second;
    }
  } while (j != 0);
  belowline.emplace_back(i);

  if (print) {
    std::cout << "\nMethod 2: " << std::endl;
    for (const auto &b: belowline) {        // Method 2
      std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
    }
  }

  return belowline;
}

// Method 3: Clipping Using Degrees /////////////////////////////////////////////////////
std::vector<int> polyintersect::clip_below_3(int cell, Mesh const &mesh,
                                             std::array<Point, 2> const &interface,
                                             bool print) {

  // Store all Points in a single list
  auto allPoints = mesh.list_of_points(cell, interface);

  // Find Center Point and Sort from Least to Greatest Degree
  Point centerPoint = center(allPoints);
  sorting(allPoints, centerPoint);

  // Store the Orientation of every node
  std::array<int, 6> const sign = orientation_clip(allPoints, interface);
  std::vector<int> belowline;

  // Clip below
  for (int p = 0; p < allPoints.size(); p++) {
    if (sign[p] <= 0) {
      belowline.emplace_back(p);
    }
  }

  if (print) {
    std::cout << "\nCell " << cell << ": " << std::endl;
    for (const auto &b: belowline) {        // method 3
      std::cout << "Coordinates: (" << allPoints[b].x << ", " << allPoints[b].y << ")" << std::endl;
    }
  }
  return belowline;
}



