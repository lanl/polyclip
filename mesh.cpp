#include "mesh.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh::Mesh(int n_cells) {
    int n_nodes = n_cells + 1;
    double lengthPerAxis = 1;
    double dy = lengthPerAxis / n_cells;
    double dx = dy;

    // Step 1: Creating the Mesh 
    points_.resize(n_nodes * n_nodes);
    //std::cout << "    //////////////////////// Mesh ////////////////////////" << std::endl << std::endl;
    for (int j = 0; j < n_nodes; j++) {         // Rows
      for (int i = 0; i < n_nodes; i++) {       // Columns
        int k = i + (j * n_nodes);              // Identify the elements
        points_[k] = {(i * dx), (j * dy)};
        //std::cout << "(" << i * dx << ", " << j * dy << ")   ";
      }
      //std::cout << std::endl << std::endl;
    }

    // Step 2: Identifying the Cells 
    int size = n_cells * n_cells;
    cells_.resize(size);
    //std::cout << "    //////////////////////// Cells ////////////////////////" << std::endl << std::endl;
    for (int j = 0; j < n_cells; j++) {         // Cell Rows
      for (int i = 0; i < n_cells; i++) {       // Cell Columns
        int k = i + (j * n_cells);              // Identify the individual cell elements
        cells_[k] = {k + j, (k + 1) + j, ((k + 1) + n_nodes) + j, k + n_nodes + j};
        //std::cout << "Cell " << k << ":   ";
        //std::cout << k + j << ", " << k + 1 + j << ", " << k + 1 + n_nodes + j << ", " << k + n_nodes + j << std::endl;
      }
      //std::cout << std::endl << std::endl;
    }
  }

// get list of points  ////////////////////////////////////////////////////////////////
  std::vector<Point> Mesh::list_of_points(int cell,
                                          std::array<Point, 2> const &line) const {

    std::vector<Point> list;
    for (int i: cells_[cell]) {
      list.emplace_back(points_[i]);
    }
    list.insert(list.end(), line.begin(), line.end());
    return list;
  }

} // namespace 'polyintersect'
