#!/bin/bash

MYDIR=/vast/home/nray/Projects/students/MelanieWalsmith/

KOKKOS_ROOT=${MYDIR}/kokkos/install 

cmake \
  -D Kokkos_ROOT=${KOKKOS_ROOT} \
  -D USE_KOKKOS_GPU=ON \
  ..

