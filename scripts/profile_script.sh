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
if [ "$#" -lt 5 ]
  then
	  echo "USAGE: $0 [MESH_FILE] [LINE_FILE] [TOTAL_LINES] [MATERIAL_FORMAT] [UES_END_POINTS] [NUM_ITERATIONS (optional)]"
	  echo -e "\n\n  ----  DESCRIPTION ----"
	  echo -e " (1) GMV Mesh File \n (2) Data Line File \n (3) Total Lines \n (4) Material Format (1. all clipped cells 2. clipped + non-clip + multiple lines 3. clipped + non-clipped) \n (5) Line uses End Points (yes = 1 or no = 0)\n (6) Total Iterations \n"
    exit 1
fi

if ! command -v nsys &> /dev/null
  then
    echo "Error: Nsys is not currently installed on the node!"
    exit 1
fi
mkdir -p output/images

MESH_FILE=$1
LINE_FILE=$2
N_LINES=$3
MATERIAL_FORMAT=$4
USE_END_POINTS=$5
NUM_ITERATIONS="${6:-5}"  # Default to 5 if not provided

file_name="$(basename "$MESH_FILE")"
rootname="${file_name%.*}"
for ((i = 1; i <= NUM_ITERATIONS; i++))
do
    output_name="${rootname}_$i"
    echo "Running iteration $i: generating $output_name"
    nsys profile -t cuda,nvtx --output="output/$output_name" ./test_mesh "$MESH_FILE" "$LINE_FILE" "$N_LINES" "$MATERIAL_FORMAT" "$USE_END_POINTS"
    nsys stats --report nvtxsum --format csv -o "output/${output_name}_summary" "output/${output_name}.nsys-rep"
    ncu --set full --target-processes all --export output/$output_name ./test_mesh "$MESH_FILE" "$LINE_FILE" "$N_LINES" "$MATERIAL_FORMAT" "$USE_END_POINTS"
done

output_name="${rootname}_$i"
python3 ../../scripts/parse_profiling_data.py "output/$rootname" $NUM_ITERATIONS

