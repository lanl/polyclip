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

import os
import matplotlib.pyplot as plt

# Data from all meshes
#timed_values_clipped_transfer_1 = [0.0600, 0.0601, 0.0588, 0.0603, 0.0592, 0.0600, 0.0603, 0.0597]
#timed_values_mesh_transfer = [0.0840, 0.0852, 0.0852, 0.0851, 0.0887, 0.0829, 0.0845, 0.0847]
timed_values_clipping = [0.4162, 0.5009, 0.5418, 0.6012, 0.6518, 0.6939, 0.7493, 0.8000]
timed_values_clipped_transfer_2 = [0.2917, 0.3501, 0.4244, 0.4480, 0.4856, 0.5182, 0.5605, 0.5860]

total_cells = [28, 100, 139, 180, 226, 260, 289, 318]

# Create new directory
out_dir = "compare"
os.makedirs(out_dir, exist_ok=True)

# Create plot
plt.figure(figsize=(8, 5))
#plt.plot(total_cells, timed_values_clipped_transfer_1, marker='o', linestyle='-', label='CLIPPED PART: CPU-TO-GPU TRANSFER', color='blue')
#plt.plot(total_cells, timed_values_mesh_transfer, marker='s', linestyle='-', label='MESH: CPU-TO-GPU TRANSFER', color='orange')
plt.plot(total_cells, timed_values_clipping, marker='^', linestyle='-', label='CLipping Multi-Material Cells', color='green')
plt.plot(total_cells, timed_values_clipped_transfer_2, marker='d', linestyle='-', label='Muli-Material Polygons: GPU-TO-CPU', color='red')

plt.ylabel("Time (ms)")
plt.xlabel("Number of Multi-Material Cells Clipped")
plt.title("Evolution of the Execution Time with Respect to the Multi-Material Cells")
plt.grid(True)

plt.legend(loc='upper left', title='Kokkos Regions')

# Save as figure
out_path = os.path.join(out_dir, "mesh_compare_time_plot.png")
plt.savefig(out_path)
