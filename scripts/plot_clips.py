#!/usr/bin/python3
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator, LogFormatter
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
    
    (threads_openmp_120, avg_openmp_120) = compute_avg_time("TimeResults_OpenMP_120.txt")
    (threads_kokkos_120, avg_kokkos_120) = compute_avg_time("TimeResults_Kokkos_120.txt")
    (threads_openmp_80, avg_openmp_80) = compute_avg_time("TimeResults_OpenMP_80.txt")
    (threads_kokkos_80, avg_kokkos_80) = compute_avg_time("TimeResults_Kokkos_80.txt")

    # Plot the Averages of the Threads ##################
    plt.plot(threads_kokkos_120, avg_kokkos_120, marker='o', linestyle='-', color='red', markerfacecolor='red', linewidth = 3, label='kokkos_cpu, 120x120 cell')
    plt.plot(threads_openmp_120, avg_openmp_120, marker='o', linestyle='-', color='blue', markerfacecolor='blue', linewidth = 3, label='openmp, 120x120 cell')
    plt.plot(threads_kokkos_80, avg_kokkos_80, marker='o', linestyle='-', color='grey', markerfacecolor='grey', linewidth = 3, label='kokkos_cpu, 80x80 cell')
    plt.plot(threads_openmp_80, avg_openmp_80, marker='o', linestyle='-', color='black', markerfacecolor='black', linewidth = 3, label='openmp, 80x80 cell')

    plt.xlabel('Threads')
    plt.ylabel('Time (ms)')
    plt.title('Clipping Strong Scaling')
    plt.grid(True)

    plt.yscale('log', base=2)
    plt.xscale('log', base=2)
    plt.gca().xaxis.set_major_locator(LogLocator(base=2))
    plt.gca().xaxis.set_major_formatter(LogFormatter(base=2, labelOnlyBase=False))
    plt.xticks(threads_kokkos_120, [str(t) for t in threads_kokkos_120])
    
    plt.legend()
    plt.show()