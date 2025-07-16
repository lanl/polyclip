import os
import matplotlib.pyplot as plt

# Data from all meshes
timed_values = [0.7196, 0.8141, 0.7511, 0.8029, 0.7891, 0.9912, 1.1089]
total_cells = [4, 4, 4, 10, 16, 465, 465]

# Create new directory
out_dir = "compare"
os.makedirs(out_dir, exist_ok=True)

# Create plot
plt.figure(figsize=(8, 5))
plt.plot(total_cells, timed_values, marker='o', linestyle='-', color='blue')
plt.ylabel("Time (ms)")
plt.xlabel("Number of Cells Clipped")
plt.title("Cells Clipped Over Time")
plt.grid(True)

# Save as figure
out_path = os.path.join(out_dir, "mesh_compare_time_plot.png")
plt.savefig(out_path)
