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
  std::string file_name = argv[1];
  polyclip::io::flatten_gmv(file_name);
  return 0;

}