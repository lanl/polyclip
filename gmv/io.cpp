#include "io.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace polyclip {

/* ------------------------------------------------------------------------- */
void io::write_clipped(Mesh_Kokkos mesh,
                       Clipped_Part clipped_part,
                       int num_total_nodes,
                       int num_total_polys,
                       const std::string& file_name) {
  std::ofstream gmv_file(file_name);

  // Original cells and pointsf
  int total_cells = mesh.mirror_cells_.extent(0);
  int points = mesh.mirror_points_.extent(0);

  // Print all points
  gmv_file << "gmvinput ascii\n";
  gmv_file << "nodev " << num_total_nodes << "\n";

  // Print all points /////////////////////////////////////////////////////////
  for (int c = 0; c < total_cells; c++) {
    int below = clipped_part.mirror_size_output_(c, 0);
    int v;

    // Check if its a Clipped Cell
    if (below == 0) { // Non-clipped cell
      v = mesh.mirror_num_verts_per_cell_(c);
      for (int i = 0; i < v; i++) {
        gmv_file << std::scientific << std::setprecision(17);
        int id = mesh.mirror_cells_(c, i, 0);
        auto const p = mesh.mirror_points_(id);
        gmv_file << p.x << " " << p.y << " " << 0.0 << "\n";
      }
    } else { // Clipped cell
      v = mesh.mirror_num_verts_per_cell_(c) + 2;
      for (int i = 0; i < v; i++) {
        gmv_file << std::scientific << std::setprecision(17);
        auto const p = clipped_part.mirror_allPoints_(c, i);
        gmv_file << p.x << " " << p.y << " " << 0.0 << "\n";
      }
    }
  }

  // Print Cells + All Clipped Cells
  gmv_file << "cells " << num_total_polys << "\n";

  // Print Cell Nodes /////////////////////////////////////////////////////////
  int node_increment =
    0; // keep track of what node we are on with respect to the gmv file
  for (int c = 0; c < total_cells; c++) {
    gmv_file << "general 1 ";
    std::string store_points = "";
    int num_verts = mesh.mirror_num_verts_per_cell_(c);

    int below = clipped_part.mirror_size_output_(c, 0);
    int above = clipped_part.mirror_size_output_(c, 1);

    // Cell with no Clipping ///////////////////
    if (below == 0) {
      for (int j = 0; j < num_verts; j++) {
        int const node_id = j + node_increment;
        store_points += std::to_string(node_id + 1) + " ";
      }
      gmv_file << std::to_string(num_verts) << " " << store_points << "\n";

      // vertex increment
      node_increment += num_verts;
    }

    // Clipped Cell //////////////////////////
    else {
      for (int i = 0; i < below; i++) {
        int const j = clipped_part.mirror_output_(c, 0, i);
        int const node_id = j + node_increment;
        store_points += std::to_string(node_id + 1) + " ";
      }

      // Cell Below
      gmv_file << std::to_string(below) << " " << store_points << "\n";
      std::string store_points = "";

      for (int i = 0; i < above; i++) {
        int const j = clipped_part.mirror_output_(c, 1, i);
        int const node_id = j + node_increment;
        store_points += std::to_string(node_id + 1) + " ";
      }

      // Cell Above
      gmv_file << "general 1 ";
      gmv_file << std::to_string(above) << " " << store_points << "\n";

      // vertices + intersect points increment
      node_increment += num_verts;
      node_increment += 2;
    }
  }

  gmv_file << "material\n";
  gmv_file << num_total_polys << " 0\n";
  for (int i = 1; i <= num_total_polys; i++) {
    gmv_file << "mat" << i << "\n";
  }

  for (int i = 1; i <= num_total_polys; i++) {
    gmv_file << i << " ";
  }

  gmv_file << "\n";
  gmv_file << "endgmv\n";
  gmv_file.close();
}

/* ------------------------------------------------------------------------- */
void io::read_lines(Clipped_Part& clips, const std::string& file_name) {
  std::ifstream line_file(file_name);
  std::string buffer;
  std::stringstream tokenizer;
  double x, y, d;
  int index = 0;

  while (std::getline(line_file, buffer)) {
    tokenizer.clear();
    tokenizer.str(buffer);
    tokenizer >> x >> y >> d;
    clips.mirror_line_(index).n = { x, y };
    clips.mirror_line_(index).d = d;
    index++;
  }
}

/* ------------------------------------------------------------------------- */
void io::write_mesh(Mesh_Kokkos mesh, const std::string& file_name) {
  std::ofstream gmv_file(file_name);
  int total_cells = mesh.mirror_cells_.extent(0);
  int points = mesh.mirror_points_.extent(0);

  gmv_file << "gmvinput ascii\n";
  gmv_file << "nodev " << points << "\n";

  // Print all points /////////////////////////////////////////////////////////
  for (int c = 0; c < mesh.mirror_points_.extent(0); c++) {
    gmv_file << std::scientific << std::setprecision(17);
    auto const p = mesh.mirror_points_(c);
    gmv_file << p.x << " " << p.y << " " << 0.0 << "\n";
  }

  // Print Cell Nodes /////////////////////////////////////////////////////////
  gmv_file << "cells " << total_cells << "\n";

  for (int c = 0; c < total_cells; c++) {
    gmv_file << "general 1 ";
    int const num_verts = mesh.mirror_num_verts_per_cell_(c);
    gmv_file << num_verts << " ";
    for (int j = 0; j < num_verts; j++) {
      gmv_file << mesh.mirror_cells_(c, j, 0) + 1 << " ";
    }
    gmv_file << std::endl;
  }
  gmv_file << "endgmv\n";
  gmv_file.close();
}

void io::flatten_gmv(std::string const& file_name) {
  Mesh_Kokkos mesh = Mesh_Kokkos();
  std::ifstream gmv_file(file_name);
  std::ofstream debug_log;
  debug_log.open("debug.txt");

  std::vector<int>vector_of_edges(10);
  std::ofstream buffer_log;
  buffer_log.open("cleaned_" + file_name);
  std::string line;
  int line_count = 1;

  while (std::getline(gmv_file, line)) {
    std::stringstream parser(line);
    std::string token;
    parser >> token;

    if (token == "gmvinput") {
      buffer_log << line << "\n";
      continue;
    }

    if (token == "nodev") {
      buffer_log << line << "\n";
      int num_of_nodes;
      parser >> num_of_nodes;

      Kokkos::resize(mesh.device_points_, num_of_nodes); // malloc
      mesh.mirror_points_ = Kokkos::create_mirror_view(mesh.device_points_);

      for (int i = 0; i < num_of_nodes; i++) {
        std::getline(gmv_file, line);
        line_count++;
        parser.clear();
        parser.str(line);
        buffer_log << line << "\n";
        double x, y, z;
        parser >> x >> y >> z;
        mesh.add_points(i, { x, y });
      }
    } else if (token == "cells") {
      buffer_log << line << "\n";

      int num_of_cells;
      parser >> num_of_cells;
      line_count++;

      constexpr int max_edges = 8;
      Kokkos::resize(mesh.device_cells_, num_of_cells, max_edges, 2);
      Kokkos::resize(mesh.num_verts_per_cell_, num_of_cells);
      Kokkos::resize(mesh.signs_, num_of_cells, max_edges + 2);

      mesh.mirror_cells_ = Kokkos::create_mirror_view(mesh.device_cells_);
      mesh.mirror_num_verts_per_cell_ =
        Kokkos::create_mirror_view(mesh.num_verts_per_cell_);
      mesh.mirror_signs_ = Kokkos::create_mirror_view(mesh.signs_);

      // (!) the vertices of a cell may be listed in several lines,
      // so put all remaining lines into buffer and parse it later.
      std::cout << "Mirror Num of Verts = " << mesh.mirror_num_verts_per_cell_.extent(0);
      std::stringstream buffer;
      while (std::getline(gmv_file, line)) {
        parser.clear();
        parser.str(line);

        while (parser >> token) {
          if (token == "general" or token == "endgmv") {
            if (!buffer.str().empty()) { // first line
              buffer << '\n';
            }
          } else {
            buffer << token << ' ';
          }
        }
      }

      std::string buffer_copy = buffer.str();
      // now parse it
      int c = 0;
      while (std::getline(buffer, line)) {
        parser.clear();
        parser.str(line);
        buffer_log << "general " << line << "\n";
#ifdef DEBUG
        std::cout << "cell: " << c << ", num_of_edges: " << num_of_edges
                  << ", vertices: [";
        for (int i = 0; i < num_of_edges; ++i) {
          std::cout << list_of_nodes[i] << ", ";
        }
        std::cout << "]\n";
#endif

        c++;
      }
    }
  }
  buffer_log << "endgmv";
  buffer_log.close();
}

/* ------------------------------------------------------------------------- */
Mesh_Kokkos io::read_mesh(std::string const& file_name) {
  Mesh_Kokkos mesh = Mesh_Kokkos();
  std::ifstream gmv_file(file_name);
  std::string line;

  while (std::getline(gmv_file, line)) {
    std::stringstream parser(line);
    std::string token;
    parser >> token;

    if (token == "gmvinput") {
      continue;
    }

    if (token == "nodev") {
      int num_of_nodes;
      parser >> num_of_nodes;

      Kokkos::resize(mesh.device_points_, num_of_nodes); // malloc
      mesh.mirror_points_ = Kokkos::create_mirror_view(mesh.device_points_);

      for (int i = 0; i < num_of_nodes; i++) {
        std::getline(gmv_file, line);
        parser.clear();
        parser.str(line);

        double x, y, z;
        parser >> x >> y >> z;
        mesh.add_points(i, { x, y });
      }
    } else if (token == "cells") {
      int num_of_cells;
      parser >> num_of_cells;

      constexpr int max_edges = 8;
      Kokkos::resize(mesh.device_cells_, num_of_cells, max_edges, 2);
      Kokkos::resize(mesh.num_verts_per_cell_, num_of_cells);
      Kokkos::resize(mesh.signs_, num_of_cells, max_edges + 2);

      mesh.mirror_cells_ = Kokkos::create_mirror_view(mesh.device_cells_);
      mesh.mirror_num_verts_per_cell_ =
        Kokkos::create_mirror_view(mesh.num_verts_per_cell_);
      mesh.mirror_signs_ = Kokkos::create_mirror_view(mesh.signs_);

      // (!) the vertices of a cell may be listed in several lines,
      // so put all remaining lines into buffer and parse it later.
      std::stringstream buffer;
      while (std::getline(gmv_file, line)) {
        parser.clear();
        parser.str(line);
        while (parser >> token) {
          if (token == "general" or token == "endgmv") {
            if (!buffer.str().empty()) { // first line
              buffer << '\n';
            }
          } else {
            buffer << token << ' ';
          }
        }
      }

      // now parse it
      int c = 0;
      while (std::getline(buffer, line)) {
        parser.clear();
        parser.str(line);
        parser >> token;

        int num_of_edges;
        parser >> num_of_edges;
        mesh.mirror_num_verts_per_cell_(c) = num_of_edges;
        std::vector<int> list_of_nodes(num_of_edges);

        int j = 0;
        while (parser >> token) {
          list_of_nodes[j++] = std::stoi(token) - 1;
        }

#ifdef DEBUG
        std::cout << "cell: " << c << ", num_of_edges: " << num_of_edges
                  << ", vertices: [";
        for (int i = 0; i < num_of_edges; ++i) {
          std::cout << list_of_nodes[i] << ", ";
        }
        std::cout << "]\n";
#endif

        for (int k = 0; k < num_of_edges; k++) {
          int const a = list_of_nodes[k];
          int const b = list_of_nodes[(k + 1) % (num_of_edges)];
          mesh.add_edge(c, k, { a, b });
        }
        c++;
      }
    }
  }

  return mesh;
}
} // namespace polyclip