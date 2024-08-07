# Arguments for 120x120 and 80x80 ####################
arg_1_120="120" 
arg_2_120="30"
arg_1_1200="1200"
arg_2_1200="300"

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
  

# Kokkos GPU file ####################################
output_file="gpu_120.txt" > $output_file   # 120x120

# Iterate Through Threads 
# Collect 5 duration times for every thread 
for i in {1 2 3 4 5}         
do 
duration=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
threads=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Max Threads" | awk '{print $3}')
deepcopy=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Duration with copy" | awk '{print $4}')
echo "Threads: $threads, Duration: $duration, Deep Copy: $deepcopy" >> $output_file
done

# Kokkos GPU file ####################################
output_file="gpu_1200.txt" > $output_file   # 80x80

# Iterate Through Threads 
# Collect 5 duration times for every thread 
for i in {1 2 3 4 5}         
do 
duration=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Duration" | awk '{print $2}')
threads=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Max Threads" | awk '{print $3}')
deepcopy=$(./clip_gpu "$arg_1_1200" "$arg_2_1200" | grep "Duration with copy" | awk '{print $4}')
echo "Threads: $threads, Duration: $duration, Deep Copy: $deepcopy" >> $output_file
done



