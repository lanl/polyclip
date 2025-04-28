#!/bin/bash

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
