#!/bin/bash

APP=./test_mesh
ARGS="mesh/unit_crust-0.08.gmv lines/0.08_double_lines.dat 16 2 0"
OUTFILE=cache_profile.csv

if [ ! -r "$ARGS" ]; then
    echo "ERROR: [INSERT ACTUAL ARGS]"
    exit 1
fi

# Run the profiler
ncu --set full \
    --metrics lts__t_sectors_op_read.sum,l1tex__t_sectors_pipe_lsu_mem_global_op_ld.sum \
    --csv \
    --log-file "$OUTFILE" \
    $APP $ARGS
