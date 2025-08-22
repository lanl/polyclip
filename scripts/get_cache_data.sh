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

PWD=$(pwd)
APP=$PWD/test_uvm_mesh
ARGS="mesh/unit_crust-0.01.gmv lines/0.01_arc_lines.dat 10 3 1"
OUTFILE=log.file

echo $APP 

# Run the profiler
ncu -f --set full \
--metrics lts__t_sectors_op_read.sum,l1tex__t_sectors_pipe_lsu_mem_global_op_ld.sum,dram__sectors_read.sum \
--csv \
--page raw \
-o test_report \
$APP $ARGS

ncu -i $PWD/test_report.ncu-rep --csv >> cache_profile.csv