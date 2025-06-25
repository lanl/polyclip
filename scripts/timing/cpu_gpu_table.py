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

import matplotlib.pyplot as plt
import numpy as np

def compute_avg_time(filename):
    time_data = {}

    # Open and read file containing timing data ##########
    with open(filename, "r") as file:
        for line in file:
            if "Duration" in line and "Deep Copy":
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

def compute_avg_time2(filename):
    time_data = {}

    # Open and read file containing timing data ##########
    with open(filename, "r") as file:
        for line in file:
            if "Threads" in line and "Duration" in line:
                parts = line.strip().split(", ")
                time = int(parts[0].split(": ")[1]) 
                deep = int(parts[1].split(": ")[1]) 

                if 1024 not in time_data:
                    time_data1[1024] = []
                    time_data2[1024] = []

            time_data1[1024].append(time)     # Store the timings
            time_data2[1024].append(deep) 

    # Obtain the Threads ################################# 
    threads = sorted(time_data.keys())

    # Calculate the Average ##############################
    avg1 = []
    avg2 = []
    for t in 1024:
        if time_data[t]:
            avg1.append(np.mean(time_data1[t]))
            avg1.append(np.mean(time_data2[t]))
        else:
            avg1.append(float('nan'))
            avg1.append(float('nan'))
    return (avg1, avg2)    

if __name__ == '__main__':
    (threads_cpu_120, avg_cpu_120) = compute_avg_time("cpu_120.txt")
    (threads_cpu_80, avg_cpu_80) = compute_avg_time("cpu_1200.txt")
    (deep_gpu_120, avg_gpu_120) = compute_avg_time2("gpu_120.txt")
    (deep_gpu_80, avg_gpu_80) = compute_avg_time2("gpu_1200.txt")

    name = 60 

    title_text = 'Kokkos CPU VS GPU'
    footer_text = 'for CPU run: 36 threads (1 thread per core)'
    fig_background_color = 'lightgray'
    fig_border = 'black'

    # plt.show()
    data =  [import matplotlib.pyplot as plt
import numpy as np

def compute_avg_time(filename):
    time_data = {}

    # Open and read file containing timing data ##########
    with open(filename, "r") as file:
        for line in file:
            if "Duration" in line and "Deep Copy":
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

# def compute_avg_time2(filename):
#     time_data1 = {}
#     time_data2 = {}

#     # Open and read file containing timing data ##########
#     with open(filename, "r") as file:
#         for line in file:
#             if "Threads" in line and "Duration" in line:
#                 parts = line.strip().split(", ")
#                 time = int(parts[0].split(": ")[1]) 
#                 deep = int(parts[1].split(": ")[1]) 
#                 threads = 1024

#                 if threads not in time_data1 and time_data2:
#                     time_data1[threads] = []
#                     time_data2[threads] = []

#             #time_data1[threads].append(time)     # Store the timings
#             #time_data2[threads].append(deep) 

#     # Obtain the Threads ################################# 
#     threads = sorted(time_data1.keys())

#     # Calculate the Average ##############################
#     avg1 = []
#     avg2 = []
#     for t in threads:
#         if time_data[t]:
#             avg1.append(np.mean(time_data1[t]))
#             avg1.append(np.mean(time_data2[t]))
#         else:
#             avg1.append(float('nan'))
#             avg1.append(float('nan'))
#     return (avg1, avg2)    

if __name__ == '__main__':
    (threads_cpu_120, avg_cpu_120) = compute_avg_time("cpu_120.txt")
    (threads_cpu_80, avg_cpu_80) = compute_avg_time("cpu_1200.txt")
    (deep_gpu_120, avg_gpu_120) = compute_avg_time("gpu_120.txt")
    (deep_gpu_80, avg_gpu_80) = compute_avg_time("gpu_1200.txt")

    name = 60 

    title_text = 'Kokkos CPU VS GPU'
    footer_text = 'for CPU run: 36 threads (1 thread per core)'
    fig_background_color = 'lightgray'
    fig_border = 'black'

    # plt.show()
    data =  [
            [         'CPU ' , 'GPU ' , 'GPU Deep Copy'],
            [ '120 x 120 Cells',  str(np.mean(avg_cpu_120)) + ' µs', str(np.mean(avg_gpu_120)) + ' µs', '1264.8 µs'],
            ['1200 x 1200 Cells',  str(np.mean(avg_cpu_80)) + ' µs', str("{:.3f}".format(np.mean(avg_gpu_80))) + ' µs', '36648.4 µs']
        ]

    # Pop the headers from the data array
    column_headers = data.pop(0)
    row_headers = [x.pop(0) for x in data]

    # Table data needs to be non-numeric text. Format the data
    # while I'm at it.
    cell_text = []
    for row in data:
        cell_text.append([f'{x}' for x in row])

    # Get some lists of color specs for row and column headers
    rcolors = plt.cm.BuPu(np.full(len(row_headers), 0.1))
    ccolors = plt.cm.BuPu(np.full(len(column_headers), 0.1))

    # Create the figure. Setting a small pad on tight_layout
    # seems to better regulate white space. Sometimes experimenting
    # with an explicit figsize here can produce better outcome.
    plt.figure(linewidth=2,
            edgecolor=fig_border,
            facecolor=fig_background_color,
            tight_layout={'pad':1},
            figsize=(5, 3),
            )

    # Add a table at the bottom of the axes
    the_table = plt.table(cellText=cell_text,
                        rowLabels=row_headers,
                        rowColours=rcolors,
                        rowLoc='right',
                        colColours=ccolors,
                        colLabels=column_headers,
                        loc='center')

    # Scaling is the only influence we have over top and bottom cell padding.
    # Make the rows taller (i.e., make cell y scale larger).
    the_table.scale(1, 1.5)

    # Hide axes
    ax = plt.gca()
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)

    # Hide axes border
    plt.box(on=None)

    # Add title
    plt.suptitle(title_text)

    # Add footer
    plt.figtext(0.95, 0.05, footer_text, horizontalalignment='right', size=6, weight='light')
    
    # Force the figure to update, so backends center objects correctly within the figure.
    # Without plt.draw() here, the title will center on the axes and not the figure.
    plt.draw()

    # Create image. plt.savefig ignores figure edge and face colors, so map them.
    fig = plt.gcf()
    plt.savefig('pyplot-table-demo.png',
                edgecolor=fig.get_edgecolor(),
                facecolor=fig.get_facecolor(),
                dpi=150
                )


            [         'CPU ' , 'GPU ' , 'GPU Deep Copy'],
            [ '120 x 120 Cells',  str(np.mean(avg_cpu_120)) + ' µs', str(np.mean(avg_gpu_120)) + ' µs', str(np.mean(deep_gpu_120)) + ' µs'],
            ['1200 x 1200 Cells',  str(np.mean(avg_cpu_80)) + ' µs', str(np.mean(avg_gpu_80)) + ' µs', str(np.mean(deep_gpu_80)) + ' µs']
        ]

    # Pop the headers from the data array
    column_headers = data.pop(0)
    row_headers = [x.pop(0) for x in data]

    # Table data needs to be non-numeric text. Format the data
    # while I'm at it.
    cell_text = []
    for row in data:
        cell_text.append([f'{x}' for x in row])

    # Get some lists of color specs for row and column headers
    rcolors = plt.cm.BuPu(np.full(len(row_headers), 0.1))
    ccolors = plt.cm.BuPu(np.full(len(column_headers), 0.1))

    # Create the figure. Setting a small pad on tight_layout
    # seems to better regulate white space. Sometimes experimenting
    # with an explicit figsize here can produce better outcome.
    plt.figure(linewidth=2,
            edgecolor=fig_border,
            facecolor=fig_background_color,
            tight_layout={'pad':1},
            figsize=(4, 2),
            )

    # Add a table at the bottom of the axes
    the_table = plt.table(cellText=cell_text,
                        rowLabels=row_headers,
                        rowColours=rcolors,
                        rowLoc='right',
                        colColours=ccolors,
                        colLabels=column_headers,
                        loc='center')

    # Scaling is the only influence we have over top and bottom cell padding.
    # Make the rows taller (i.e., make cell y scale larger).
    the_table.scale(1, 1.5)

    # Hide axes
    ax = plt.gca()
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)

    # Hide axes border
    plt.box(on=None)

    # Add title
    plt.suptitle(title_text)

    # Add footer
    plt.figtext(0.95, 0.05, footer_text, horizontalalignment='right', size=6, weight='light')
    
    # Force the figure to update, so backends center objects correctly within the figure.
    # Without plt.draw() here, the title will center on the axes and not the figure.
    plt.draw()

    # Create image. plt.savefig ignores figure edge and face colors, so map them.
    fig = plt.gcf()
    plt.savefig('pyplot-table-demo.png',
                edgecolor=fig.get_edgecolor(),
                facecolor=fig.get_facecolor(),
                dpi=150
                )

