#include "mesh_gpu.h"

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

    Kokkos::resize(points_, total_nodes);
    Kokkos::resize(cells_, total_cells, 4); // 4 vertices per cell
    mirror_points_ = Kokkos::create_mirror_view(points_);
    mirror_cells_ = Kokkos::create_mirror_view(cells_);

    init(n_nodes, n_cells, lengthPerAxis, points_, cells_);
  }

// Init the views //////////////////////////////////////////////////////////////////////
  void Mesh_Kokkos::init(int n_nodes, 
                         int n_cells, 
                         double lengthPerAxis,
                         Kokkos::View<Point*> points,
                         Kokkos::View<int**> cells) {

    double const dy = lengthPerAxis / n_cells;
    double const dx = dy;
    
    // Step 1: Creating the Mesh (KOKKOS)
    Kokkos::parallel_for(n_nodes, KOKKOS_LAMBDA(int j) { // Rows
      for (int i = 0; i < n_nodes; i++) {       // Columns
        int k = i + (j * n_nodes);              // Identify the elements
        points(k) = {(i * dx), (j * dy)};
      }
    });
  
    // Step 2: Identifying the Cells (KOKKOS: NO DEPENDENTS)
    Kokkos:: parallel_for(n_cells, KOKKOS_LAMBDA(int j){ // Cell Rows
      for (int i = 0; i < n_cells; i++) {       // Cell Columns
        int k = i + (j * n_cells);              // Identify the individual cell elements
        cells(k , 0) = k + j;
        cells(k , 1) = (k + 1) + j;
        cells(k , 2) = ((k + 1) + n_nodes) + j;
        cells(k , 3) = k + n_nodes + j; 
      }
    });
  }	  
} 
