# Create file ########################################
output_file="TimeResults.txt" > $output_file

# Iterate Through Threads ############################
for threads in 1 2 4 8       
do 
  # Collect 5 duration times for every thread ########
  for i in {1 2 3 4 5}         
  do 
  export OMP_NUM_THREADS=$threads
  duration=$(./clip_kokkos | grep "Duration" | awk '{print $2}')
  echo "Threads: $threads, Duration: $duration" >> $output_file
  done
done  


