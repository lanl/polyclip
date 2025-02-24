//
// Created by Jonathan Mathurin on 2/16/25.
//

#ifndef GMV_READER_H
#define GMV_READER_H
#include <string>
#include "../test/mesh_gpu.h"



namespace polyintersect::gmvstream{
    //
    // void clear_gmv(std::string file_name);
    // void set_header(std::fstream& gmv_file);
    // void set_node(std::fstream& gmv_file, Mesh_Kokkos mesh);
    // void set_point(std::fstream& gmv_file, Point point);
    // void set_cell_header(std::fstream& gmv_file, Mesh_Kokkos mesh);
    void write_gmv(Mesh_Kokkos mesh, const std::string& basename);
    Mesh_Kokkos read_gmv(std::string& file_name);
}




#endif //GMV_READER_H
