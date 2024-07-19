#include "mesh_kokkos.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int n_cells, double lengthPerAxis) {
    int const n_nodes = n_cells + 1;
    int const total_nodes = n_nodes * n_nodes;
    int const total_cells = n_cells * n_cells;
    double const dy = lengthPerAxis / n_cells;
    double const dx = dy;

    Kokkos::resize(points_, total_nodes);
    Kokkos::resize(cells_, total_cells, 4); // 4 vertices per cell

    // Step 1: Creating the Mesh (KOKKOS)
    Kokkos::parallel_for(n_nodes, KOKKOS_LAMBDA(int j) { // Rows
      for (int i = 0; i < n_nodes; i++) {       // Columns
        int k = i + (j * n_nodes);              // Identify the elements
        points_(k) = {(i * dx), (j * dy)};
      }
    });
  
    // Step 2: Identifying the Cells (KOKKOS: NO DEPENDENTS)
    Kokkos:: parallel_for(n_cells, KOKKOS_LAMBDA(int j){ // Cell Rows
      for (int i = 0; i < n_cells; i++) {       // Cell Columns
        int k = i + (j * n_cells);              // Identify the individual cell elements
        cells_(k , 0) = k + j;
        cells_(k , 1) = (k + 1) + j;
        cells_(k , 2) = ((k + 1) + n_nodes) + j;
        cells_(k , 3) = k + n_nodes + j; 
      }
    });

  #ifdef ENABLE_PRINTING
    // super clean printing !
    for (int k = 0; k < total_nodes; ++k) {
      double const& x = points_(k).x;
      double const& y = points_(k).y;
      std::cout << "point "<< k <<": ("<< x << ", " << y << ")" << std::endl;
    }

    for (int k = 0; k < total_cells; ++k) {
      int const a = cells_(k, 0);
      int const b = cells_(k, 1);
      int const c = cells_(k, 2);
      int const d = cells_(k, 3);
      std::cout << "cell " << k << ": [" << a << ", "<< b << ", "<< c << ", "<< d << "]" << std::endl;
    }

    std::cout << "finished initialization" << std::endl;
  #endif    
  }

// get list of points  ////////////////////////////////////////////////////////////////
  std::vector<Point> Mesh_Kokkos::list_of_points(int cell,
                                                 std::array<Point, 2> const &line) const {

    std::vector<Point> list(6);
    std::cout << "Cell: " << cell << std::endl;

    for (int i = 0; i < 4; i++) {
      int index = cells_(cell, i);
      list[i] = points_(index);
    }
    list[4] = line[0];
    list[5] = line[1];

  #ifdef ENABLE_PRINTING
    #pragma omp critical
    {
      std::cout << "List of Cell " << cell << ": " << std::endl;;
      for(int i = 0; i < list.size(); i++){
        std::cout << list[i].x << ", " << list[i].y << std::endl;
      }
      std::cout << std::endl;
    }
  #endif    
    return list;
  }

} // namespace 'polyintersect'
