#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import platform, subprocess, os, argparse

def get_cpu_info():
    """Return a human-readable CPU name."""
    cpu = "Unknown CPU"
    sys = platform.system()
    try:
        if sys == "Linux":
            with open("/proc/cpuinfo") as f:
                for L in f:
                    if "model name" in L:
                        cpu = L.split(":",1)[1].strip()
                        break
        elif sys == "Darwin":
            # macOS
            cpu = subprocess.check_output(
                ["sysctl","-n","machdep.cpu.brand_string"],
                universal_newlines=True
            ).strip()
        else:
            cpu = platform.processor()
    except Exception:
        pass
    return cpu

def plot_kernels(csv_files, output_file):
    cpu_info = get_cpu_info()
    plt.figure(figsize=(12,8))

    # choose markers and line styles in a cycle
    styles = ["o-", "s--", "^-.", "d-", "v--", "*-.", "x-", "p--", "h-",
        ">-", "<--", "D-.", "|-", "_--", "+-.", ".-", ",--"
    ]

    for (csv, style) in zip(csv_files, styles):
        if not os.path.exists(csv):
            print(f"Warning: {csv} not found, skipping.")
            continue

        # load and compute mean performance
        df = pd.read_csv(csv)
        gcols = [c for c in df.columns if "GFLOPS" in c]
        df["mean_gflops"] = df[gcols].mean(axis=1)

        # short label from filename
        short = (os.path.basename(csv)
                   .replace("dgemm_benchmark_","")
                   .replace("_results.csv","")
                   .replace("_"," "))

        # compute peak
        i_peak = df["mean_gflops"].idxmax()
        m_peak = df.loc[i_peak, "m"]
        g_peak = df.loc[i_peak, "mean_gflops"]

        # build legend label including peak GFLOPS
        label = f"{short} ({g_peak:.2f} GFLOPS)"

        # plot curve
        plt.plot(df["m"], df["mean_gflops"],
                 style, markersize=5, linewidth=1.8,
                 label=label)

        # mark the peak point
        plt.scatter(m_peak, g_peak,
                    s=120, edgecolors='black',
                    zorder=3)

    # cache–size boundaries
    for x, lbl in [(45,"L1"), (1024,"L2"), (2896,"L3")]:
        plt.axvline(x=x, color="gray", linestyle="--", linewidth=1)
        plt.text(x+5, plt.ylim()[1]*0.9, lbl,
                 color='red', rotation=0, va="top", weight="bold")

    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend(loc="best", fontsize=11)
    plt.xlabel("Matrix Size (N × N)", fontsize=14)
    plt.ylabel("Performance (GFLOPS)", fontsize=14)
    plt.title(f"DGEMM Kernel Comparison on {cpu_info}", fontsize=16)

    plt.tight_layout()
    plt.savefig(output_file, dpi=300)
    print(f"Saved comparison plot as {output_file}")

if __name__ == "__main__":
    p = argparse.ArgumentParser(
        description="Overlay multiple DGEMM kernel benchmark CSVs"
    )
    p.add_argument("csv_files", nargs="+",
                   help="One or more dgemm_benchmark_*_results.csv files")
    p.add_argument("--out", default="dgemm_kernels_comparison.png",
                   help="Output image filename")
    args = p.parse_args()

    plot_kernels(args.csv_files, args.out)
