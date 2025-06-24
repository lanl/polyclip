#!/bin/bash

# (c) 2025. Triad National Security, LLC. All rights reserved.
# This program was produced under U.S. Government contract 89233218CNA000001
# for Los Alamos National Laboratory (LANL), which is operated by Triad National
# Security, LLC for the U.S. Department of Energy/National Nuclear Security
# Administration. All rights in the program are reserved by Triad National
# Security, LLC, and the U.S. Department of Energy/National Nuclear Security
# Administration. The Government is granted for itself and others acting on its
# behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
# to reproduce, prepare derivative works, distribute copies to the public,
# perform publicly and display publicly, and to permit others to do so.

OPENMPI_MODULE="openmpi/3.1.6-gcc_9.4.0" 
CMAKE_MODULE="cmake/3.26.3"
CUDA_MODULE="cuda/12.0.0"

module purge
module load ${CMAKE_MODULE}
module load ${CUDA_MODULE}
module load ${OPENMPI_MODULE}
module load nsight-systems

export KOKKOS_TOOLS_LIBS=${HOME}/kokkos-tools/install/lib64/libkp_nvtx_connector.so
echo $KOKKOS_TOOLS_LIBS
