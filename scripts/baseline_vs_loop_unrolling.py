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

# Duration at Clipping Cells Kokkos Region
#values = [0.5841, 0.5558]  # 3 and 5 0.01
#values = [0.6108, 0.7840]  # 3 and 5 0.08

#values = [0.5841, 0.5737]  # 2 and 4 0.01
#values = [0.6108, 0.7977]  # 2 and 4 0.08

values = [0.5841, 0.5847]  # 4 and 8 0.01
#values = [0.6108, 0.7944]  # 4 and 8 0.08

# Non-UVM and UVM labels
labels = ["No Loop Unrolling", "Loop Unrolling"]
fixed_labels = [label.replace(" ", "\n") for label in labels]

# Create Bar Graph
plt.figure(figsize=(10, 12))
x_pos = range(len(labels))
colors = ['blue', 'orange']
# Duration Labels
for i, val in enumerate(values):
    label = f'{val:.4f} ms'
    plt.text(i + 0.5, val, label, ha='center', va='bottom',fontsize=14)

plt.bar(x_pos, values, width=1.0, align='edge', color=colors)
plt.xticks([x + 0.5 for x in x_pos], fixed_labels, rotation=45, ha='right', fontsize=20)
plt.title("Clipping Runtime", fontsize=30, weight='bold')
plt.ylabel("Runtime (ms)", fontsize=20)
plt.ticklabel_format(axis='y', style='plain')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

# Save Figure 
output_dir = "compare/"
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "baseline_vs_loop_unrolling.png")
plt.savefig(output_path)
plt.close()
