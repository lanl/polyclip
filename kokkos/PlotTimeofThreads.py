#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np

time_data = {}

# Open and read file containing timing data ##########
with open("TimeResults.txt", "r") as file:
    for line in file:
        if "Threads" in line and "Duration" in line:
            parts = line.strip().split(", ")
            threads = int(parts[0].split(": ")[1]) 
            time = int(parts[1].split(": ")[1]) 

            if threads not in time_data:
                time_data[threads] = []

        time_data[threads].append(time)     # Store the timings

# Obtain the Threads ################################# 
threads = sorted(time_data.keys())

# Calculate the Average ##############################
avg = []
for t in threads:
    if time_data[t]:
        avg.append(np.mean(time_data[t]))
    else:
        avg.append(float('nan'))


# Plot the Averages of the Threads ##################
plt.plot(threads, avg, marker='o', linestyle='-', color='pink')
plt.xlabel('Threads')
plt.ylabel('Time (ms)')
plt.title('Kokkos Clipping Duration over Threads')
plt.grid(True)
plt.xticks(threads)
plt.show()
