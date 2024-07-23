#!/usr/bin/python3
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
import numpy as np

def compute_avg_time(filename):
    time_data = {}

    # Open and read file containing timing data ##########
    with open(filename, "r") as file:
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
    return (threads, avg)            

if __name__ == '__main__':
    
    (threads_openmp, avg_openmp) = compute_avg_time("TimeResults_OpenMP.txt")
    (threads_kokkos, avg_kokkos) = compute_avg_time("TimeResults_Kokkos.txt")

    # Plot the Averages of the Threads ##################
    plt.plot(threads_openmp, avg_kokkos, marker='o', linestyle='-', color='red', markerfacecolor='red', linewidth = 3, label='kokkos')
    plt.plot(threads_openmp, avg_openmp, marker='o', linestyle='-', color='blue', markerfacecolor='blue', linewidth = 3, label='openmp')
    plt.xlabel('Threads')
    plt.ylabel('Time (ms)')
    plt.title('Clipping Strong Scaling, mesh size: 120x120 cells')
    plt.grid(True)
    plt.xticks(threads_openmp)
    plt.yscale('log', base=2)
    plt.xscale('log', base=2)
    plt.legend()
    plt.show()