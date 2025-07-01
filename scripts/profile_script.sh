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

export KOKKOS_TOOLS_LIBS=${HOME}/dev/kokkos-tools/install/lib64/libkp_nvtx_connector.so
if [ "$#" -lt 2 ]
  then
    echo "USAGE: $0 [MESH_FILE] [LINE_FILE] [NUM_ITERATIONS (optional)]"
    exit 1
fi

if ! command -v nsys &> /dev/null
  then
    echo "Error: Nsys is not currently installed on the node!"
    exit 1
fi
mkdir -p output/images
NUM_ITERATIONS="${3:-5}"  # Default to 5 if not provided
mesh_file=$1
file_name="$(basename "$mesh_file")"
rootname="${file_name%.*}"
for ((i = 1; i <= NUM_ITERATIONS; i++))
do
    output_name="${rootname}_$i"
    echo "Running iteration $i: generating $output_name"
    nsys profile -t cuda,nvtx --output="output/$output_name" ./test_mesh "$MESH_FILE" "$LINE_FILE"
    nsys export --type sqlite --output="output/${output_name}.sqlite" "output/${output_name}.nsys-rep"
done

output_name="${rootname}_$i"
python3 ../../scripts/parse_profiling_data.py "output/$rootname" $NUM_ITERATIONS

