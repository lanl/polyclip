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
import numpy as np

# Data
regions = ['CLIPPED PART: \nCPU-TO-GPU TRANSFER', 
           'MESH: \nCPU-TO-GPU TRANSFER', 
           'CLIPPING CELLS', 
           'CLIPPED PART: \nGPU-TO-CPU TRANSFER']
# 0.01 arc 
#double_precision      = [0.0850, 0.9669, 0.5841, 2.3634]
#single_precision      = [0.1499, 1.6016, 0.5918, 1.7455]
#single_precision_sfu  = [0.0835, 1.4340, 0.5325, 1.2882]

# 0.08 16 lines
#double_precision      = [0.0597, 0.0847, 0.6108, 0.6585]
#single_precision      = [0.0804, 0.0982, 0.8058, 0.5875]
#single_precision_sfu  = [0.0791, 0.0981, 0.7899, 0.3807]

# 0.08 14 lines
#double_precision      = [0.0603, 0.0845, 0.5645, 0.6065]
#single_precision      = [0.0799, 0.0955, 0.7536, 0.5650]
#single_precision_sfu  = [0.0792, 0.0976, 0.7427, 0.3759]

# 0.08 12 lines
#double_precision      = [0.0600, 0.0829, 0.5406, 0.5522]
#single_precision      = [0.0800, 0.0977, 0.7037, 0.5345]
#single_precision_sfu  = [0.0811, 0.0966, 0.6963, 0.3607]

# 0.08 10 lines
#double_precision      = [0.0592, 0.0887, 0.5079, 0.4997]
#single_precision      = [0.0852, 0.1018, 0.6608, 0.4841]
#single_precision_sfu  = [0.0789, 0.0956, 0.6446, 0.3490]

# 0.08 8 lines
#double_precision      = [0.0603, 0.0851, 0.4636, 0.4453]
#single_precision      = [0.0796, 0.1003, 0.6067, 0.4444]
#single_precision_sfu  = [0.0793, 0.0963, 0.5927, 0.3329]

# 0.08 6 lines
#double_precision      = [0.0588, 0.0852, 0.4375, 0.3781]
#single_precision      = [0.0812, 0.1002, 0.5542, 0.4053]
#single_precision_sfu  = [0.0807, 0.0968, 0.5549, 0.3069]

# 0.08 4 lines
#double_precision      = [0.0601, 0.0852, 0.3999, 0.3206]
#single_precision      = [0.0795, 0.0959, 0.5109, 0.3519]
#single_precision_sfu  = [0.0797, 0.0961, 0.4965, 0.2858]

# 0.08 1 lines
double_precision      = [0.0600, 0.0840, 0.3358, 0.2418]
single_precision      = [0.0810, 0.0950, 0.4272, 0.2937]
single_precision_sfu  = [0.0802, 0.0975, 0.4114, 0.2650]

# X-axis configuration
x = np.arange(len(regions))  # the label locations
width = 0.25  # the width of the bars

# Plotting
fig, ax = plt.subplots(figsize=(10, 6))
bars1 = ax.bar(x - width, double_precision, width, label='Double Precision')
bars2 = ax.bar(x, single_precision, width, label='Single Precision')
bars3 = ax.bar(x + width, single_precision_sfu, width, label='Single Precision & SFU')

# Labels and title
ax.set_ylabel('Timing (ms)')
ax.set_xlabel('Kokkos Region')
ax.set_title('Timing Comparison Across Precision Types and Regions')
ax.set_xticks(x)
ax.set_xticklabels(regions)
ax.legend()

# Annotate bars with timing values (optional)
for bars in [bars1, bars2, bars3]:
    for bar in bars:
        height = bar.get_height()
        ax.annotate(f'{height:.2f}',
                    xy=(bar.get_x() + bar.get_width() / 2, height),
                    xytext=(0, 3),  # offset
                    textcoords="offset points",
                    ha='center', va='bottom')

plt.tight_layout()
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Save Figure 
output_dir = "compare/"
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "percision_comparison.png")
plt.savefig(output_path)
plt.close()
