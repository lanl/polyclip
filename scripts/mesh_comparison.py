import os
import matplotlib.pyplot as plt

# Data from all meshes
timed_values = [4.9006, 5.6175, 5.6354, 6.9660]
total_cells = [1, 4, 8, 16]

# Create new directory
out_dir = "compare"
os.makedirs(out_dir, exist_ok=True)

# Create plot
plt.figure(figsize=(8, 5))
plt.plot(total_cells, timed_values, marker='o', linestyle='-', color='blue')
plt.ylabel("Time (ms)")
plt.xlabel("Number of Lines Clipping")
plt.title("Duration of Clipping Below Cells Region Across Multiple Lines \nin a 0.08 Unit Mesh")
plt.grid(True)

# Save as figure
out_path = os.path.join(out_dir, "mesh_compare_time_plot.png")
plt.savefig(out_path)
