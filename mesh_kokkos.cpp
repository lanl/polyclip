#include "mesh_kokkos.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int n_cells, double lengthPerAxis) {
    int n_nodes = n_cells + 1;
    double dy = lengthPerAxis / n_cells;
    double dx = dy;

    int total_nodes = n_nodes * n_nodes;
    int total_cells = n_cells * n_cells;
    Kokkos::resize(points_, total_nodes);
    Kokkos::resize(cells_, total_cells, 4); // 4 vertices per cell

    // Step 1: Creating the Mesh (KOKKOS)
    //std::cout << "    //////////////////////// Mesh ////////////////////////" << std::endl << std::endl;
    for (int j = 0; j < n_nodes; j++) {         // Rows
      for (int i = 0; i < n_nodes; i++) {       // Columns
        int k = i + (j * n_nodes);              // Identify the elements
        points_(k) = {(i * dx), (j * dy)};
        std::cout << "point "<< k <<": ("<< i * dx << ", " << j * dy << ")" << std::endl;
      }
    }

    // Step 2: Identifying the Cells (KOKKOS: NO DEPENDENTS)
    //Kokkos:: parallel_for(n_cells, KOKKOS_LAMBDA(int j){
    //std::cout << "    //////////////////////// Cells ////////////////////////" << std::endl << std::endl;
    for (int j = 0; j < n_cells; j++) {         // Cell Rows
      for (int i = 0; i < n_cells; i++) {       // Cell Columns
        int k = i + (j * n_cells);              // Identify the individual cell elements
        cells_(k , 0) = k + j;
        cells_(k , 1) = (k + 1) + j;
        cells_(k , 2) = ((k + 1) + n_nodes) + j;
        cells_(k , 3) = k + n_nodes + j; 
      
        std::cout << "Cell " << k << ":   ";
        std::cout << k + j << ", " << k + 1 + j << ", " << k + 1 + n_nodes + j << ", " << k + n_nodes + j << std::endl;
      }
    }
    std::cout << "finished initialization" << std::endl;
      //  });
  }

// get list of points  ////////////////////////////////////////////////////////////////
  std::vector<Point> Mesh_Kokkos::list_of_points(int cell,
                                                 std::array<Point, 2> const &line) const {

    std::vector<Point> list;
    //Kokkos::resize(cells_, cell, n_);

    #pragma omp critical
    {
      std::cout << "Cell: " << cell << std::endl;
      for (int i = 0; i < 4; i++) {

        // int index = cells_(cell, i);
        // std::cout << "Index: " << index << std::endl;
        // std::cout << "Point[" << index << "]: " << "(" << points_[index].x 
        // << ", " << points_[index].y << ")" << std::endl;    
      }
      std::cout << std::endl;
      std::cout << "Interface of Cell " << cell << ": " << line[0].x << ", " << line[0].y << "\t" << line[1].x << ", " << line[1].y;

      std::cout << std::endl;
    }
    list.insert(list.end(), line.begin(), line.end());

    #pragma omp critical
    {
      std::cout << "List of Cell " << cell << ": " << std::endl;;
      for(int i = 0; i < list.size(); i++){
        std::cout << list[i].x << ", " << list[i].y << std::endl;
      }
      std::cout << std::endl;
    }

    return list;
  }

} // namespace 'polyintersect'
