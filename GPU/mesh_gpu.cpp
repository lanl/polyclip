#include "mesh_gpu.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int n_cells, double lengthPerAxis) {
    n_cells_ = n_cells;
    n_nodes_ = n_cells + 1;
    length_per_axis_ = lengthPerAxis;
    int const total_nodes = n_nodes_ * n_nodes_;
    int const total_cells = n_cells * n_cells;

    Kokkos::resize(points_, total_nodes);
    Kokkos::resize(cells_, total_cells, 4); // 4 vertices per cell
    mirror_points_ = Kokkos::create_mirror_view(points_);
    mirror_cells_ = Kokkos::create_mirror_view(cells_);
  }

// Init the views //////////////////////////////////////////////////////////////////////
  void Mesh_Kokkos::init() {
    double const dy = length_per_axis_ / n_cells_;
    double const dx = dy;
    
    // Step 1: Creating the Mesh (KOKKOS)
    Kokkos::parallel_for(n_nodes_, KOKKOS_LAMBDA(int j) { // Rows
      for (int i = 0; i < n_nodes_; i++) {       // Columns
        int k = i + (j * n_nodes_);              // Identify the elements
        points_(k) = {(i * dx), (j * dy)};
      }
    });
  
    // Step 2: Identifying the Cells (KOKKOS: NO DEPENDENTS)
    Kokkos:: parallel_for(n_cells_, KOKKOS_LAMBDA(int j){ // Cell Rows
      for (int i = 0; i < n_cells_; i++) {       // Cell Columns
        int k = i + (j * n_cells_);              // Identify the individual cell elements
        cells_(k , 0) = k + j;
        cells_(k , 1) = (k + 1) + j;
        cells_(k , 2) = ((k + 1) + n_nodes_) + j;
        cells_(k , 3) = k + n_nodes_ + j; 
      }
    });

  }	  
} 
