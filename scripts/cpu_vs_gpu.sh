# Arguments for 120x120 and 80x80 ####################
arg_1_120="120" 
arg_2_120="30"
arg_1_80="80"
arg_2_80="20"

# # Kokkos CPU file ####################################
# output_file="cpu_120.txt" > $output_file   # 120x120

# # Iterate Through Threads 
# # Collect 5 duration times for every thread 
# for i in {1 2 3 4 5}         
# do 
# export OMP_NUM_THREADS=16
# threads=16
# duration=$(./clip_kokkos "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
# echo "Threads: $threads, Duration: $duration" >> $output_file
# done

# # Kokkos CPU file ####################################
# output_file="cpu_80.txt" > $output_file   # 120x120

# # Iterate Through Threads 
# # Collect 5 duration times for every thread 
# for i in {1 2 3 4 5}         
# do 
# export OMP_NUM_THREADS=16
# threads=16
# duration=$(./clip_kokkos "$arg_1_80" "$arg_2_80" | grep "Duration" | awk '{print $2}')
# echo "Threads: $threads, Duration: $duration" >> $output_file
# done
  

# Kokkos GPU file ####################################
output_file="gpu_120.txt" > $output_file   # 120x120

# Iterate Through Threads 
# Collect 5 duration times for every thread 
for i in {1 2 3 4 5}         
do 
duration=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Duration" | awk '{print $2}')
duration=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Max Threads" | awk '{print $2}')
echo "Threads: $threads, Duration: $duration" >> $output_file
done

# Kokkos GPU file ####################################
output_file="gpu_80.txt" > $output_file   # 80x80

# Iterate Through Threads 
# Collect 5 duration times for every thread 
for i in {1 2 3 4 5}         
do 
duration=$(./clip_gpu "$arg_1_80" "$arg_2_80" | grep "Duration" | awk '{print $2}')
threads=$(./clip_gpu "$arg_1_120" "$arg_2_120" | grep "Max Threads" | awk '{print $2}')
echo "Threads: $threads, Duration: $duration" >> $output_file
done



