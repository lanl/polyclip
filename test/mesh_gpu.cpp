#include "mesh_gpu.h"

/*
    Code Description:
        - Creates the Mesh
        - Identifies the Cells of the Mesh
*/

namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell) {
    
    host_points_.resize(total_points);
    host_cells_.resize(total_cells, std::vector<std::vector<int>>(max_edges_per_cell, std::vector<int>(2)));

    Kokkos::resize(device_points_, total_points);  // malloc
    Kokkos::resize(device_cells_, total_cells, max_edges_per_cell, 2);

    // Check GPU Results
    host_points_view_ = Kokkos::create_mirror_view(device_points_); // copy
    host_cells_view_ = Kokkos::create_mirror_view(device_cells_);
    mirror_points_ = Kokkos::create_mirror_view(device_points_); 
    mirror_cells_ = Kokkos::create_mirror_view(device_cells_);

  }

// Storing Coordinates of all the points on the host ////////////////////////////////////////////////
  void Mesh_Kokkos::add_all_points(int point, Point coordinate, std::vector<Point>& host_points_){
      host_points_[point] = {(coordinate.x), (coordinate.y)};
  }  

// Storing Components of a Cell on the host /////////////////////////////////////////////////////////
  void Mesh_Kokkos::add_edge(int cell, int edge, Edge const& node, std::vector<std::vector<std::vector<int>>>& host_cells_){
      host_cells_[cell][edge][0] = node.a;
      host_cells_[cell][edge][1] = node.b;
  }

// CPU to GPU ///////////////////////////////////////////////////////////////////////////////////////
  void Mesh_Kokkos::send_to_gpu(int total_points, int total_cells, int max_edges_per_cell){
    for(int i = 0; i < total_points; i++){
      host_points_view_(i) = host_points_[i];
    }

    for(int i = 0; i < total_cells; i++){
      for(int j = 0; j < max_edges_per_cell; j++){
        host_cells_view_(i, j, 0) = host_cells_[i][j][0];
        host_cells_view_(i, j, 1) = host_cells_[i][j][1];
      }
    }

    Kokkos::deep_copy(device_points_, host_points_view_);
    Kokkos::deep_copy(device_cells_, host_cells_view_);
  }
}

/*#include "mesh_gpu.h"


   // Code Description:
       // - Creates the Mesh
       // - Identifies the Cells of the Mesh


namespace polyintersect {

// Create the Mesh //////////////////////////////////////////////////////////////////////
  Mesh_Kokkos::Mesh_Kokkos(int total_points, int total_cells, int max_edges_per_cell) {

    Kokkos::resize(points_, total_points);  // malloc
    Kokkos::resize(cells_, total_cells, max_edges_per_cell, 2);

    mirror_points_ = Kokkos::create_mirror_view(points_); //copy
    mirror_cells_ = Kokkos::create_mirror_view(cells_);
  }

// Storing Coordinates of all the points ////////////////////////////////////////////////
  void Mesh_Kokkos::add_all_points(int point, Point coordinate, Kokkos::View<Point*> points_){
    Kokkos::parallel_for( "add_point", 1, KOKKOS_LAMBDA(const int){
    	points_(point) = {(coordinate.x), (coordinate.y)};
    });
  }  
// Storing Components of a Cell /////////////////////////////////////////////////////////
  void Mesh_Kokkos::add_edge(int cell, int edge, Edge const& node, Kokkos::View<int***> cells_){
    Kokkos::parallel_for("create cell", 1, KOKKOS_LAMBDA(const int){
    	cells_(cell, edge, 0) = node.a;
    	cells_(cell, edge, 1) = node.b;
    });
  }
} */
