#include "mesh.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell) {
    Kokkos::resize(device_points_, total_points);  // malloc
    Kokkos::resize(device_cells_, total_cells, max_edges_per_cell, 2);
    Kokkos::resize(num_verts_per_cell_, total_cells);
    Kokkos::resize(signs_, total_cells, max_edges_per_cell + 2);

    // CPU data members
    mirror_points_ = Kokkos::create_mirror_view(device_points_);
    mirror_cells_ = Kokkos::create_mirror_view(device_cells_);
    mirror_num_verts_per_cell_ = Kokkos::create_mirror_view(num_verts_per_cell_);
    mirror_signs_ = Kokkos::create_mirror_view(signs_);
  }

// Storing Coordinates of all the points on the host ////////////////////////////////////////////////
  void Mesh_Kokkos::add_points(int point, Point coordinate){
      mirror_points_(point) = {(coordinate.x), (coordinate.y)};
  }  

// Storing Components of a Cell on the host /////////////////////////////////////////////////////////
  void Mesh_Kokkos::add_edge(int cell, int edge, Edge const& node){
      mirror_cells_(cell, edge, 0) = node.a;
      mirror_cells_(cell, edge, 1) = node.b;
  }

// CPU to GPU ///////////////////////////////////////////////////////////////////////////////////////
  void Mesh_Kokkos::send_to_gpu(){
    Kokkos::deep_copy(device_points_, mirror_points_);
    Kokkos::deep_copy(device_cells_, mirror_cells_);
    Kokkos::deep_copy(num_verts_per_cell_, mirror_num_verts_per_cell_);
  }

// GPU to CPU ///////////////////////////////////////////////////////////////////////////////////////
  void Mesh_Kokkos::send_to_cpu(){
    Kokkos::deep_copy(mirror_points_, device_points_);
    Kokkos::deep_copy(mirror_cells_, device_cells_);
    Kokkos::deep_copy(mirror_num_verts_per_cell_, num_verts_per_cell_);
    Kokkos::deep_copy(mirror_signs_, signs_);
  }
}

