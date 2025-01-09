#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "intersect_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

int main(int argc, char * argv[]) {

    using namespace polyintersect;

    Kokkos::initialize(argc, argv);
    {
        // initialize variables for the unstructured mesh
        int total_points = 11;
        int total_cells = 4;
        int max_edges_per_cell = 6;

        // Create mesh
        Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);

        // All Nodes 
        mesh.add_all_points(0, {0.0, 0.0});
        mesh.add_all_points(1, {0.5, 0.0});
        mesh.add_all_points(2, {0.5, 0.25});
        mesh.add_all_points(3, {0.25, 0.375});
        mesh.add_all_points(4, {0.0, 0.25});
        mesh.add_all_points(5, {0.875, 0.25});
        mesh.add_all_points(6, {1.0, 0.375});
        mesh.add_all_points(7, {0.875, 0.625});
        mesh.add_all_points(8, {0.5, 0.625});
        mesh.add_all_points(9, {0.5, 0.875});
        mesh.add_all_points(10, {0.875, 0.875});

        // Pentagon
        mesh.add_edge(0, 0, {0, 1});
        mesh.add_edge(0, 1, {1, 2});
        mesh.add_edge(0, 2, {2, 3});
        mesh.add_edge(0, 3, {3, 4});
        mesh.add_edge(0, 4, {4, 0});

        // Triangle
        mesh.add_edge(1, 0, {1, 5});
        mesh.add_edge(1, 1, {5, 2});
        mesh.add_edge(1, 2, {2, 1});

        // Hexagon
        mesh.add_edge(2, 0, {2, 5});
        mesh.add_edge(2, 1, {5, 6});
        mesh.add_edge(2, 2, {6, 7});
        mesh.add_edge(2, 3, {7, 8});
        mesh.add_edge(2, 4, {8, 3});   
        mesh.add_edge(2, 5, {3, 2});   

        // Square
        mesh.add_edge(3, 0, {8, 7});
        mesh.add_edge(3, 1, {7, 10});
        mesh.add_edge(3, 2, {10, 9});
        mesh.add_edge(3, 3, {9, 8});
 
        // CPU to GPU
        mesh.send_to_gpu();

	// GPU Results ///////////////////////////////////////////////////////////////////////
        // Print Cells 
	std::cout << std::endl;
	std::cout << "-------- GPU Results --------" << std::endl;
        for(int j = 0; j < total_cells; j++){   // Cell
            std::cout << "Cell " << j << ":" << std::endl;
            for (int i = 0; i < max_edges_per_cell; i++) {      // Edge       
                std::cout << "Edge " << i << " (" << mesh.mirror_cells_(j , i, 0) << ", ";
                std::cout << mesh.mirror_cells_(j, i, 1) << ") ";

                if(j == 0 && i == 4 || j == 1 && i == 2 || j == 3 && i == 3){   
                    break;
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // Print Point Coordinates
        for (int j = 0; j < total_points; j++) {             // All Points
                std::cout << "Point " << j << ": (" << mesh.mirror_points_(j).x << ", " << mesh.mirror_points_(j).y << ")" << std::endl;
     
       }
    }
    Kokkos::finalize();
}


/*#include "clippings_gpu.h"
#include "mesh_gpu.h"
#include "intersect_gpu.h"
#include <Kokkos_Core.hpp>
#include <omp.h>
#include <cstdlib>
#include "../timer.h"

int main(int argc, char * argv[]) {

    using namespace polyintersect;

    Kokkos::initialize(argc, argv);
    {
        // initialize variables for the unstructured mesh
        int total_points = 11;
        int total_cells = 4;
        int max_edges_per_cell = 6;

        // Create mesh
        Mesh_Kokkos mesh(total_points, total_cells, max_edges_per_cell);

        // All Nodes
        mesh.add_all_points(0, {0.0, 0.0}, mesh.points_);
        mesh.add_all_points(1, {0.5, 0.0}, mesh.points_);
        mesh.add_all_points(2, {0.5, 0.25}, mesh.points_);
        mesh.add_all_points(3, {0.25, 0.375}, mesh.points_);
        mesh.add_all_points(4, {0.0, 0.25}, mesh.points_);
        mesh.add_all_points(5, {0.875, 0.25}, mesh.points_);
        mesh.add_all_points(6, {1.0, 0.375}, mesh.points_);
        mesh.add_all_points(7, {0.875, 0.625}, mesh.points_);
        mesh.add_all_points(8, {0.5, 0.625}, mesh.points_);
        mesh.add_all_points(9, {0.5, 0.875}, mesh.points_);
        mesh.add_all_points(10, {0.875, 0.875}, mesh.points_);

        // Pentagon
        mesh.add_edge(0, 0, {0, 1}, mesh.cells_);
        mesh.add_edge(0, 1, {1, 2}, mesh.cells_);
        mesh.add_edge(0, 2, {2, 3}, mesh.cells_);
        mesh.add_edge(0, 3, {3, 4}, mesh.cells_);
        mesh.add_edge(0, 4, {4, 0}, mesh.cells_);

        // Triangle
        mesh.add_edge(1, 0, {1, 5}, mesh.cells_);
        mesh.add_edge(1, 1, {5, 2}, mesh.cells_);
        mesh.add_edge(1, 2, {2, 1}, mesh.cells_);

        // Hexagon
        mesh.add_edge(2, 0, {2, 5}, mesh.cells_);
        mesh.add_edge(2, 1, {5, 6}, mesh.cells_);
        mesh.add_edge(2, 2, {6, 7}, mesh.cells_);
        mesh.add_edge(2, 3, {7, 8}, mesh.cells_);
        mesh.add_edge(2, 4, {8, 3}, mesh.cells_);   
        mesh.add_edge(2, 5, {3, 2}, mesh.cells_);   

        // Square
        mesh.add_edge(3, 0, {8, 7}, mesh.cells_);
        mesh.add_edge(3, 1, {7, 10}, mesh.cells_);
        mesh.add_edge(3, 2, {10, 9}, mesh.cells_);
        mesh.add_edge(3, 3, {9, 8}, mesh.cells_);

	Kokkos::deep_copy(mesh.mirror_points_, mesh.points_);
        Kokkos::deep_copy(mesh.mirror_cells_, mesh.cells_);

        
        // Print Cells 
        for(int j = 0; j < total_cells; j++){   // Cell
            std::cout << "----- Cell " << j << " -----" << std::endl;
            for (int i = 0; i < max_edges_per_cell; i++) {      // Edge       
                std::cout << "Edge " << i << ": (" << mesh.mirror_cells_(j , i, 0) << ", ";
                std::cout << mesh.mirror_cells_(j, i, 1) << ") " << std::endl;

                if(j == 0 && i == 4 || j == 1 && i == 2 || j == 3 && i == 3){   
                    break;
                }
            }
	    std::cout <<  std::endl;
        }
        std::cout << std::endl;

        // Print Point Coordinates
        for (int j = 0; j < total_points; j++) {             // All Pointd
                std::cout << "Point " << j << ": (" << mesh.mirror_points_(j).x << ", " << mesh.mirror_points_(j).y << ")" << std::endl;
            }
     }
    Kokkos::finalize();
}
*/
