/*
 * (c) 2025. Triad National Security, LLC. All rights reserved.
 * This program was produced under U.S. Government contract 89233218CNA000001
 * for Los Alamos National Laboratory (LANL), which is operated by Triad National
 * Security, LLC for the U.S. Department of Energy/National Nuclear Security
 * Administration. All rights in the program are reserved by Triad National
 * Security, LLC, and the U.S. Department of Energy/National Nuclear Security
 * Administration. The Government is granted for itself and others acting on its
 * behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
 * to reproduce, prepare. derivative works, distribute copies to the public,
 * perform publicly and display publicly, and to permit others to do so.
 */

#include "io.h"
#include "test_predicates.h"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "You need to pass the file name bro." << std::endl;
    return EXIT_FAILURE;
  }
  std::string file_name = argv[1];
  std::cout << "file_name: " << file_name << std::endl;

  Kokkos::initialize(argc, argv);
  {
    polyclip::Mesh_Kokkos mesh = polyclip::io::read_mesh(file_name);

    std::vector<polyclip::Point> points = {
      { 0.000000000000000e+00, 0.000000000000000e+00 },
      { 7.000000000000000e+00, 0.000000000000000e+00 },
      { 7.000000000000000e+00, 3.000000000000000e+00 },
      { 0.000000000000000e+00, 3.000000000000000e+00 },
      { 0.000000000000000e+00, 3.481809764468654e-01 },
      { 1.387695646119859e+00, 0.000000000000000e+00 },
      { 9.830124447427289e-01, 8.843892629912024e-01 },
      { 9.646814804971522e-01, 8.900970799009288e-01 },
      { 1.734431471879353e+00, 0.000000000000000e+00 },
      { 3.068925204380143e+00, 0.000000000000000e+00 },
      { 3.069593647703083e+00, 1.350585298062404e-01 },
      { 2.464862090873698e+00, 9.421692894937580e-01 },
      { 2.140048683231722e+00, 9.179298886667057e-01 },
      { 4.480977197939190e+00, 0.000000000000000e+00 },
      { 4.481102827197334e+00, 1.649697804449280e-01 },
      { 3.896006244011467e+00, 9.308868187269134e-01 },
      { 3.629601210640684e+00, 9.253562064939818e-01 },
      { 5.866035882288134e+00, 0.000000000000000e+00 },
      { 5.853027345417333e+00, 2.711357726592403e-01 },
      { 5.286556996171021e+00, 9.294415853103707e-01 },
      { 5.061695863614696e+00, 9.268155762459331e-01 },
      { 7.000000000000000e+00, 9.166150260364594e-01 },
      { 6.425323680914148e+00, 9.737942460337289e-01 },
      { 0.000000000000000e+00, 1.499298532208340e+00 },
      { 5.381686246074910e-01, 1.498509671021036e+00 },
      { 1.685065028818605e+00, 1.324041977033921e+00 },
      { 3.126267145068220e+00, 1.496928706361902e+00 },
      { 2.915380892160040e+00, 1.496765736645619e+00 },
      { 4.565460259139887e+00, 1.497422184054787e+00 },
      { 4.364611283038471e+00, 1.497138055390429e+00 },
      { 6.035370047969234e+00, 1.498691694555424e+00 },
      { 5.742183733826077e+00, 1.498605615315502e+00 },
      { 1.684740280670023e+00, 1.668788765619045e+00 },
      { 9.797225971677869e-01, 2.110479686758923e+00 },
      { 9.696587061807134e-01, 2.107382225656224e+00 },
      { 2.142774458488092e+00, 2.079129670198848e+00 },
      { 2.461727581128567e+00, 2.055886624993001e+00 },
      { 3.892800510832855e+00, 2.066783361243361e+00 },
      { 3.630350989396582e+00, 2.071765320105936e+00 },
      { 5.285592639261830e+00, 2.069242034506545e+00 },
      { 5.060703979258783e+00, 2.071338203467119e+00 },
      { 7.000000000000000e+00, 2.083250216478605e+00 },
      { 6.425402539865143e+00, 2.025475964547122e+00 },
      { 0.000000000000000e+00, 2.650962228455231e+00 },
      { 1.726227503890317e+00, 3.000000000000000e+00 },
      { 1.395559291471623e+00, 3.000000000000000e+00 },
      { 3.067955774900923e+00, 2.855933684155547e+00 },
      { 4.478821007626653e+00, 2.827550213901720e+00 },
      { 5.850926433998492e+00, 2.726501129142131e+00 },
      { 3.067318026178516e+00, 3.000000000000000e+00 },
      { 4.478716767251954e+00, 3.000000000000000e+00 },
      { 5.863888775047688e+00, 3.000000000000000e+00 }
    };

    for (int i = 0; i < mesh.mirror_points_.extent(0); i++) {
      auto const& p = mesh.mirror_points_(i);
      polyclip::assert_near(p.x, points[i].x, 1.e-15, "parsed x coord");
      polyclip::assert_near(p.y, points[i].y, 1.e-15, "parsed y coord");
    }
    std::cout << "100% parsed coords match" << std::endl;

    std::vector<std::vector<int>> expected = { { 4, 0, 5, 6, 7 },
                                               { 8, 9, 10, 11, 12 },
                                               { 9, 13, 14, 15, 16, 10 },
                                               { 13, 17, 18, 19, 20, 14 },
                                               { 17, 1, 21, 22, 18 },
                                               { 23, 4, 7, 24 },
                                               { 5, 8, 12, 25, 6 },
                                               { 16, 26, 27, 11, 10 },
                                               { 28, 29, 15, 14, 20 },
                                               { 30, 31, 19, 18, 22 },
                                               { 32, 33, 34, 24, 7, 6, 25 },
                                               { 35, 32, 25, 12, 11, 27, 36 },
                                               { 37, 38, 26, 16, 15, 29 },
                                               { 39, 40, 28, 20, 19, 31 },
                                               { 21, 41, 42, 30, 22 },
                                               { 43, 23, 24, 34 },
                                               { 44, 45, 33, 32, 35 },
                                               { 38, 46, 36, 27, 26 },
                                               { 47, 37, 29, 28, 40 },
                                               { 31, 30, 42, 48, 39 },
                                               { 45, 3, 43, 34, 33 },
                                               { 49, 44, 35, 36, 46 },
                                               { 50, 49, 46, 38, 37, 47 },
                                               { 51, 50, 47, 40, 39, 48 },
                                               { 41, 2, 51, 48, 42 } };

    int const n = mesh.mirror_cells_.extent(0);
    for (int i = 0; i < n; i++) {
      int const m = mesh.mirror_num_verts_per_cell_(i);
      for (int j = 0; j < m; j++) {
        int const vij = mesh.mirror_cells_(i, j, 0);
        polyclip::assert_eq(vij, expected[i][j], "parsed cell vertex");
      }
    }
    std::cout << "100% parsed cell vertices match" << std::endl;
  }

  Kokkos::finalize();
  return EXIT_SUCCESS;
}
