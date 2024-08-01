import matplotlib.pyplot as plt

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
    (threads_gpu_120, avg_gpu_120) = compute_avg_time("gpu_120.txt")
    # (threads_gpu_80, avg_gpu_80) = compute_avg_time("gpu_80.txt")

    fig, ax = plt.sublots()

    ax.xaxis.set_visible(False)
    ax.yaxis.set_visible(False)
    ax.set_frame_on(False)

    table_data = [["Threads", threads_gpu_120], ["Duration (µs)", avg_gpu_120]]

    table = ax.table(CellText=table_data, loc='center')

    table.set_fontsize(14)
    table.scale(1.5, 1.5)

    plt.show()

