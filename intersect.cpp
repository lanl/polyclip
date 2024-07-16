#include "intersect.h"


namespace polyintersect {

  std::array<Point, 2> intersect_cell_with_line(Mesh const& mesh,
                                                int c,
                                                Line const& line,
                                                std::map<std::pair<int, int>, int>& mapping) {

    std::array<Point, 2> result;
    int const n = mesh.cells_[c].size();

    // deduce bounds on coordinates
    double x_min = std::numeric_limits<double>::max();
    double y_min = std::numeric_limits<double>::max();
    double x_max = -x_min;
    double y_max = -y_min;

    for (int i = 0; i < n; ++i) {
      int const a = mesh.cells_[c][i];
      x_min = std::min(x_min, mesh.points_[a].x);
      y_min = std::min(y_min, mesh.points_[a].y);
      x_max = std::max(x_max, mesh.points_[a].x);
      y_max = std::max(y_max, mesh.points_[a].y);
    }

    int k = 0;
    for (int i = 0; i < n; ++i) {
      int const j = (i + 1) % n;
      int const a = mesh.cells_[c][i];
      int const b = mesh.cells_[c][j];
      double const& xa = mesh.points_[a].x;
      double const& ya = mesh.points_[a].y;
      double const& xb = mesh.points_[b].x;
      double const& yb = mesh.points_[b].y;

      double const& xp = line[0].x;
      double const& yp = line[0].y;
      double const& xq = line[1].x;
      double const& yq = line[1].y;

      // (ab) is represented as: a1x + b1y = c1
      // (pq) is represented as: a2x + b2y = c2
      double const a1 = yb - ya;
      double const b1 = xa - xb;
      double const a2 = yq - yp;
      double const b2 = xp - xq;
      double const det = a1 * b2 - a2 * b1;
      if (std::abs(det) < 1.e-15) {
        continue;
      } else {
        double const c1 = a1 * xa + b1 * ya;
        double const c2 = a2 * xp + b2 * yp;
        double const x = (b2 * c1 - b1 * c2) / det;
        double const y = (a1 * c2 - a2 * c1) / det;
        if (x < x_min or x > x_max or y < y_min or y > y_max) {
          continue;
        }
        result[k] = {x, y};
        mapping[{i, j}] = k + n;
        k++;
      #ifdef DEBUG
        std::cout << "x: " << x << ", y: " << y << std::endl;
      #endif
      }
    }
    return result;
  }
}