#!/bin/bash

MYDIR=/vast/home/nray/Projects/students/MelanieWalsmith/

INSTALL_DIR=${MYDIR}/kokkos/install


#  -D CMAKE_CXX_COMPILER=$path_to_compiler \
#  -D Kokkos_ARCH_VOLTA=On \
cmake \
  -D CMAKE_INSTALL_PREFIX=$INSTALL_DIR \
  -D Kokkos_ENABLE_TESTS=On \
  -D Kokkos_ENABLE_CUDA=On \
  -D CMAKE_CXX_EXTENSIONS=On \
..

