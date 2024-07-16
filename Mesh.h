#pragma once      //prevent headers being used twice

#include "geometry.h"

namespace polyintersect {

class Mesh {
public:
   // constructor
   Mesh(int n_cells);
   // destructor
   ~Mesh() = default;

   // data members
   std::vector<Point> points_;
   std::vector<std::vector<int>> cells_;
};
}
