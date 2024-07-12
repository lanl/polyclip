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
    int size = m.points.size();
    m.cells.resize(size);
    std::cout << std::endl << std::endl << "    //////////////////////// Cells ////////////////////////" << std::endl << std::endl;
    for(int k = 0; k < size; ++k){
        m.cells[k] = {k, (k+1), ((k+1) + n_nodes), k + n_nodes};
        std::cout << "Cell " << k << ":   ";
        std::cout << k << ", " << k+1 << ", " << k+1 + n_nodes << ", " << k + n_nodes << std::endl;
    }    
    return m;
}
