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

# Things that need to be changed
#   1) text file
#   2) the name of the cmake file being called
#         - ./clip_kokkos
#         - ./allclips
#         - ./clip_gpu (gpu)


# Arguments for 120x120 and 80x80 ####################
arg_1_120="120" 
arg_2_120="30"
arg_1_80="1200"
arg_2_80="300"

# Kokkos file ########################################
output_file="TimeResults_Kokkos_120.txt" > $output_file   # 120x120

# Iterate Through Threads ############################
for threads in 1 2 4 8 
do 
  # Collect 5 duration times for every thread ########
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=$threads
  duration=$(./clip_kokkos "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
done  

# OpenMP file ########################################
output_file="TimeResults_OpenMP_120.txt" > $output_file # 120x120 

# Iterate Through Threads ############################
for threads in 1 2 4 8   
do 
  # Collect 5 duration times for every thread ########
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=$threads
  duration=$(./allclips "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
done  

# Kokkos file ########################################
output_file="TimeResults_Kokkos_80.txt" > $output_file   # 80x80

# Iterate Through Threads ############################
for threads in 1 2 4 8 
do 
  # Collect 5 duration times for every thread ########
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=$threads
  duration=$(./clip_kokkos "$arg_1_80" "$arg_2_80" | grep "Duration" | awk '{print $2}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
done  

# OpenMP file ########################################
output_file="TimeResults_OpenMP_80.txt" > $output_file # 80x80 

# Iterate Through Threads ############################
for threads in 1 2 4 8   
do 
  # Collect 5 duration times for every thread ########
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=$threads
  duration=$(./allclips "$arg_1_80" "$arg_2_80" | grep "Duration" | awk '{print $2}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
done  



