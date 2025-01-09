#include "mesh_gpu.h"

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

    // CPU data members
    mirror_points_ = Kokkos::create_mirror_view(device_points_); 
    mirror_cells_ = Kokkos::create_mirror_view(device_cells_);
  }

// Storing Coordinates of all the points on the host ////////////////////////////////////////////////
  void Mesh_Kokkos::add_all_points(int point, Point coordinate){
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
