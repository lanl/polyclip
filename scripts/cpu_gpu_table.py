import pandas as pd

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
    (threads_gpu_80, avg_gpu_80) = compute_avg_time("gpu_80.txt")

    data = {'Threads:' [threads_gpu_120], 'Duration:' [avg_gpu_120]}
    df = pd.DataFrame(data)

    df.to_csv('table_results.csv', index=False)
    print(df)

