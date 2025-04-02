#!/bin/bash
#MYDIR=/vast/home/nray/Projects/students/MelanieWalsmith/

#KOKKOS_DIR=${MYDIR}/kokkos/install/kokkos/lib64/cmake/Kokkos
KOKKOS_DIR=${HOME}/kokkos/install/lib64/cmake/Kokkos
INSTALL_DIR=${HOME}/kokkos-tools/install

# on a gpu node, this should build nvtx stuff
cmake \
  -DCMAKE_CXX_EXTENSIONS=Off \
  -DKokkos_DIR=${KOKKOS_DIR} \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  ..

