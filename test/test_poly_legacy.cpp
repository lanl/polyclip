#include "clippings.h"
#include "mesh.h"
#include "print.h"
#include "clip.h"
#include "clipped_part.h"
#include "test_predicates.h"
#include "intersect_n_d.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "timer.h"
#include "../core/clippings.h"
#include "../core/mesh.h"
#include "../core/print.h"
#include "../core/clip.h"
#include "../core/clipped_part.h"
#include "../core/timer.h"
#include "../gmv/io.h"

int main(int argc, char* argv[]) {
  using namespace polyclip;

  Kokkos::initialize(argc, argv);
  {
    // initialize variables for the unstructured mesh
    int const total_points = 11;
    int const total_cells = 4;
    int const max_edges_per_cell = 6;
<<<<<<< HEAD
    int total_lines = 0;
=======
    if(argc < 4) {
      std::cout << "Usage: test_clip_poly_legacy [LINE_TYPE] [TOLERANCE] [LINE_FILE_NAME]";
      exit(1);
    }

    bool debug_flag = false;
    if(argc >= 5)
      debug_flag = true;

    std::string file_name = argv[3];
>>>>>>> 764ffee (I really really want to make sure that my code is functional. So doing an exhaustive test to be 100% certain that it works if the line files are created properly.)

    int const line_rep =
      std::stoi(argv[1]); // 1: horizontal| 2: vertical| 3: arbitrary
    double const tolerance = std::stod(argv[2]);
    // Testing: distances for every cell
    if(line_rep == 1){
       total_lines = 3;
    } else if(line_rep == 2){
       total_lines = 2;
    } else{
       total_lines = 3;
    }

   double horizontal[3] = { -0.125, -0.5, -0.75 };
    double vertical[2] = { -0.375, -0.625};
    double arbitrary[3] = {
         -0.26516504294495535, -0.618718433538229, -0.8838834764831844};
 
   /* //double vertical[total_lines] = { -0.375, -0.625, -0.75, -0.625 };
    double arbitrary[total_lines] = {
      -0.26516504294495535, -0.4419417382415923, -0.618718433538229,
      -0.8838834764831844
      }; // Test dummy: replace with -1*/


    // Create mesh /////////////////////////////////////////////////////////////////////////////////////////
    Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);
    Clipped_Part clipped_part(total_points, total_cells, max_edges_per_cell, total_cells);

    // All Nodes
    mesh.add_points(0, { 0.0, 0.0 });
    mesh.add_points(1, { 0.5, 0.0 });
    mesh.add_points(2, { 0.5, 0.25 });
    mesh.add_points(3, { 0.25, 0.375 });
    mesh.add_points(4, { 0.0, 0.25 });
    mesh.add_points(5, { 0.875, 0.25 });
    mesh.add_points(6, { 1.0, 0.375 });
    mesh.add_points(7, { 0.875, 0.625 });
    mesh.add_points(8, { 0.5, 0.625 });
    mesh.add_points(9, { 0.5, 0.875 });
    mesh.add_points(10, { 0.875, 0.875 });

    // Pentagon
    mesh.add_edge(0, 0, { 0, 1 });
    mesh.add_edge(0, 1, { 1, 2 });
    mesh.add_edge(0, 2, { 2, 3 });
    mesh.add_edge(0, 3, { 3, 4 });
    mesh.add_edge(0, 4, { 4, 0 });

    // Triangle
    mesh.add_edge(1, 0, { 1, 5 });
    mesh.add_edge(1, 1, { 5, 2 });
    mesh.add_edge(1, 2, { 2, 1 });

    // Hexagon
    mesh.add_edge(2, 0, { 3, 2 });
    mesh.add_edge(2, 1, { 2, 5 });
    mesh.add_edge(2, 2, { 5, 6 });
    mesh.add_edge(2, 3, { 6, 7 });
    mesh.add_edge(2, 4, { 7, 8 });
    mesh.add_edge(2, 5, { 8, 3 });

    // Square
    mesh.add_edge(3, 0, { 8, 7 });
    mesh.add_edge(3, 1, { 7, 10 });
    mesh.add_edge(3, 2, { 10, 9 });
    mesh.add_edge(3, 3, { 9, 8 });

    // Declare the Number of Vertices
    mesh.mirror_num_verts_per_cell_(0) = 5;
    mesh.mirror_num_verts_per_cell_(1) = 3;
    mesh.mirror_num_verts_per_cell_(2) = 6;
    mesh.mirror_num_verts_per_cell_(3) = 4;

    // CPU to GPU
    mesh.send_to_gpu();

    // Max Threads and Timer
#if defined(KOKKOS_ENABLE_CUDA)
    int max_threads = Kokkos::Cuda().cuda_device_prop().maxThreadsPerBlock;
#elif defined(KOKKOS_ENABLE_OPENMP)
    int max_threads = omp_get_max_threads();
#else
    int max_threads = 1;
#endif
    auto start = timer::now();

    // Overlapping Test Lines for every cell ////////////////////////////////////////////////////////////////

    Kokkos::Profiling::pushRegion("INIT LINE INTERFACE");

    io::read_lines(clipped_part, "line_test");
    clipped_part.send_to_cpu();


    Kokkos::Profiling::popRegion();


    // Clipping below for Every Cell ////////////////////////////////////////////////////////////////////////
    clip(total_cells, total_lines, mesh.device_points_, mesh.device_cells_,
         clipped_part.intersect_points_, clipped_part.line_,
         mesh.num_verts_per_cell_, clipped_part.allPoints_,
         clipped_part.size_output_, clipped_part.output_, mesh.signs_,
	 clipped_part.clipped_cell_);

    int const end = timer::elapsed(start); // time deep copy

    // Send to CPU
    mesh.send_to_cpu();
    clipped_part.send_to_cpu();
    int const end_including_copy = timer::elapsed(start);

    // Compare and Verify Results ////////////////////////////////////////////////////////////////////////////
    std::array<double, 8> x{}, y{};
    if (line_rep == 1) {
      x = { 0.5, 0.0, 0.6875, 0.5, 0.9375, 0.375, 0.875, 0.5 };
      y = { 0.125, 0.125, 0.125, 0.125, 0.5, 0.5, 0.75, 0.75 };
    } else if (line_rep == 2) {
      x = { 0.375, 0.375, 0.625, 0.625, 0.375, 0.375, 0.625, 0.625 };
      y = { 0.0, 0.3125, 0.0833333333333333, 0.25, 0.312500000000000, 0.5, 0.625, 0.875 };
    } else {
      x = { 0.375000000629262, 0.083333333752841, 0.725000000880966, 0.625000001468277,
	    0.625000001468277, 0.375000000734139, 0.625000002097539, 0.5 };
      y = { 0.0  , 0.291666666876421, 0.150000000587311, 0.25, 
	   0.25, 0.500000000734139, 0.625, 0.750000002097539 };
    }

    verify_intersection_points(total_cells, clipped_part, x.data(), y.data(),
                               tolerance);


  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
