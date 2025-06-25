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

# Arguments for 120x120 and 80x80 ####################
arg_1_120="120" 
arg_2_120="30"
arg_1_1200="1200"
arg_2_1200="300"

RUN_CPU=false

if ${RUN_CPU}; then
  # Kokkos CPU file ####################################
  output_file="cpu_120.txt" > $output_file   # 120x120
  
  # Iterate Through Threads 
  # Collect 5 duration times for every thread 
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=16
  threads=16
  duration=$(./clip_kokkos "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
  threads=$(./clip_kokkos "$arg_1_120" "$arg_2_120" | grep "Max Threads" | awk '{print $3}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
  
  # Kokkos CPU file ####################################
  output_file="cpu_1200.txt" > $output_file   # 120x120
  
  # Iterate Through Threads 
  # Collect 5 duration times for every thread 
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=16
  threads=16
  duration=$(./clip_kokkos "$arg_1_1200" "$arg_2_1200" | grep "Duration" | awk '{print $2}')
  threads=$(./clip_kokkos "$arg_1_1200" "$arg_2_1200" | grep "Max Threads" | awk '{print $3}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
else  
  # Kokkos GPU file ####################################
  output_file="gpu_120.txt" > $output_file   # 120x120
  
  # Iterate Through Threads 
  # Collect 5 duration times for every thread 
  for i in {1 2 3 4 5}         
  do 
  duration=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
  deepcopy=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Deep copy" | awk '{print $3}')
  threads=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Max Threads" | awk '{print $4}')
  echo "Threads: $threads, Duration: $duration, Deep Copy: $deepcopy" >> $output_file
  done
  
  # Kokkos GPU file ####################################
  output_file="gpu_1200.txt" > $output_file   # 80x80
  
  # Iterate Through Threads 
  # Collect 5 duration times for every thread 
  for i in {1 2 3 4 5}         
  do 
  duration=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Duration" | awk '{print $2}')
  deepcopy=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Deep copy" | awk '{print $3}')
  threads=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Max Threads" | awk '{print $4}')
  echo "Threads: $threads, Duration: $duration, Deep Copy: $deepcopy" >> $output_file
  done
fi


