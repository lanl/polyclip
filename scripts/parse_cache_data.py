import csv
import os
import numpy as np

def parse_cache_metrics(csv_path):
    """
    Parse cache metrics from the CSV:
    Scan each cell. If it matches a target metric name, read value 2 columns ahead.
    """
    l1_accesses = []
    l2_accesses = []
    l1_hit_rates = []
    l2_hit_rates = []

    with open(csv_path, newline='') as csvfile:
        reader = csv.reader(csvfile)
        count = 0
        for row in reader:
            for i, cell in enumerate(row):
                cell_clean = cell.strip().strip('"')



                def try_get_value(offset=2):
                    if i + offset < len(row):
                        raw = row[i + offset].strip().strip('"').replace(',', '')
                        try:
                            return float(raw)
                        except ValueError:
                            return None
                    return None

                if cell_clean == "L1/TEX Hit Rate":
                    val = try_get_value()
                    if val is not None:
                        l1_hit_rates.append(val)

                elif cell_clean == "L2 Hit Rate":
                    val = try_get_value()
                    if val is not None:
                        l2_hit_rates.append(val)

                elif cell_clean == "l1tex__t_sectors_pipe_lsu_mem_global_op_ld.sum":
                    val = try_get_value()
                    if val is not None:
                        l1_accesses.append(val)

                elif cell_clean == "lts__t_sectors_op_read.sum":
                    val = try_get_value()
                    if val is not None:
                        l2_accesses.append(val)

    print("Length of l1_accesses:", len(l1_accesses))
    print("Length of l2_accesses:", len(l2_accesses))
    print("Length of l1_hit_rates:", len(l1_hit_rates))
    print("Length of l2_hit_rates:", len(l2_hit_rates))

    total_l2_accesses = sum(l2_accesses) if l2_accesses else 0
    total_l1_accesses = sum(l1_accesses) if l1_accesses else 0
    avg_l2_hit_rate = np.mean(l2_hit_rates) if l2_hit_rates else float('nan')
    avg_l1_hit_rate = np.mean(l1_hit_rates) if l1_hit_rates else float('nan')

    return {
        "total_l2_accesses": total_l2_accesses,
        "total_l1_accesses": total_l1_accesses,
        "avg_l2_hit_rate": avg_l2_hit_rate,
        "avg_l1_hit_rate": avg_l1_hit_rate,
    }

def calculate_hits_and_misses(metrics):
    """
    Calculate estimated hits and misses based on accesses and hit rates.
    """
    total_l2_accesses = metrics["total_l2_accesses"]
    total_l1_accesses = metrics["total_l1_accesses"]
    avg_l2_hit_rate = metrics["avg_l2_hit_rate"]
    avg_l1_hit_rate = metrics["avg_l1_hit_rate"]

    estimated_l2_hits = total_l2_accesses * (avg_l2_hit_rate / 100) if total_l2_accesses > 0 else 0
    estimated_l2_misses = total_l2_accesses - estimated_l2_hits

    estimated_l1_hits = total_l1_accesses * (avg_l1_hit_rate / 100) if total_l1_accesses > 0 else 0
    estimated_l1_misses = total_l1_accesses - estimated_l1_hits

    return {
        "estimated_l2_hits": estimated_l2_hits,
        "estimated_l2_misses": estimated_l2_misses,
        "estimated_l1_hits": estimated_l1_hits,
        "estimated_l1_misses": estimated_l1_misses,
    }

def main():
    csv_file = "cache_profile.csv"

    if not os.path.isfile(csv_file):
        print(f"File not found: {csv_file}")
        return

    metrics = parse_cache_metrics(csv_file)
    hits_misses = calculate_hits_and_misses(metrics)

    print(f"\n--- Cache Analysis Summary ---")
    print(f"Total L2 Accesses: {metrics['total_l2_accesses']:.0f}")
    print(f"Average L2 Hit Rate: {metrics['avg_l2_hit_rate']:.2f}%")
    print(f"Estimated L2 Hits: {hits_misses['estimated_l2_hits']:.0f}")
    print(f"Estimated L2 Misses: {hits_misses['estimated_l2_misses']:.0f}")
    print()
    print(f"Total L1 Accesses: {metrics['total_l1_accesses']:.0f}")
    print(f"Average L1 Hit Rate: {metrics['avg_l1_hit_rate']:.2f}%")
    print(f"Estimated L1 Hits: {hits_misses['estimated_l1_hits']:.0f}")
    print(f"Estimated L1 Misses: {hits_misses['estimated_l1_misses']:.0f}")

if __name__ == "__main__":
    main()
