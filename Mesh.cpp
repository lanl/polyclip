#include "Mesh.h"

// Create the Mesh //////////////////////////////////////////////////////////////////////
mesh createMesh(int n_cells){
    mesh m;
    int n_nodes = n_cells + 1;
    double lengthPerAxis = 1;
    double dy = lengthPerAxis / n_cells;
    double dx = dy;

    // Creating the Mesh ////////////////////////////////////////////
    m.points.resize(n_nodes * n_nodes);
    std::cout << "    //////////////////////// Mesh ////////////////////////" << std::endl << std::endl;
    for(int j = 0; j < n_nodes; j++){               // Rows
        for(int i = 0; i < n_nodes; i++){           // Columns
            int k = i + (j * n_nodes);              // Identify the elements
            m.points[k] = {(i * dx), (j * dy)};
            std::cout << "(" << i * dx << ", " << j * dy << ")   ";
        }
        std::cout << std::endl << std::endl;
    }

    // Identifying the Cells ////////////////////////////////////////
    int size = n_cells * n_cells;
    m.cells.resize(size);
    std::cout << "    //////////////////////// Cells ////////////////////////" << std::endl << std::endl;
    for(int j = 0; j < n_cells; j++){               // Cell Rows
        for(int i = 0; i < n_cells; i++){           // Cell Columns
            int k = i + (j * n_cells);              // Identify the individual cell elements
            m.cells[k] = {k + j, (k+1) + j, ((k+1) + n_nodes) + j, k + n_nodes + j};
            std::cout << "Cell " << k << ":   ";
            std::cout << k + j << ", " << k+1 + j << ", " << k+1 + n_nodes + j << ", " << k + n_nodes + j << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    return m;
}
