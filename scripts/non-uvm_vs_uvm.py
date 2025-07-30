import os
import matplotlib.pyplot as plt

# Duration at Clipping Cells Kokkos Region
values = [0.6108, 0.5333]

# Non-UVM and UVM labels
labels = ["Non-UVM", "UVM"]
fixed_labels = [label.replace(" ", "\n") for label in labels]

# Create Bar Graph
plt.figure(figsize=(10, 12))
x_pos = range(len(labels))
colors = ['purple', 'yellow']
# Duration Labels
for i, val in enumerate(values):
    label = f'{val:.4f} ms'
    plt.text(i + 0.5, val, label, ha='center', va='bottom',fontsize=14)

plt.bar(x_pos, values, width=1.0, align='edge', color=colors)
plt.xticks([x + 0.5 for x in x_pos], fixed_labels, rotation=45, ha='right', fontsize=20)
plt.title("Kokkos Region of Clipping Below Runtime\n Comparing Non-UVM Vs. UVM", fontsize=30, weight='bold')
plt.ylabel("Runtime (ms)", fontsize=20)
plt.ticklabel_format(axis='y', style='plain')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

# Save Figure 
output_dir = "compare/"
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "non-uvm_vs_uvm_bar.png")
plt.savefig(output_path)
plt.close()
