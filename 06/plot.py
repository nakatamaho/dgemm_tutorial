import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import platform
import subprocess
import os

def get_cpu_info():
    """CPUの情報を取得する関数"""
    cpu_info = "Unknown CPU"
    if platform.system() == "Linux":
        try:
            with open("/proc/cpuinfo", "r") as f:
                for line in f:
                    if "model name" in line:
                        cpu_info = line.split(":", 1)[1].strip()
                        break
        except:
            pass
    elif platform.system() == "Darwin":  # macOS
        try:
            output = subprocess.check_output(
                ["sysctl", "-n", "machdep.cpu.brand_string"]
            ).decode().strip()
            if output:
                cpu_info = output
        except:
            pass
    elif platform.system() == "Windows":
        try:
            output = subprocess.check_output(["wmic", "cpu", "get", "name"]).decode()
            cpu_info = output.strip().split("\n")[1]
        except:
            try:
                cpu_info = platform.processor()
            except:
                pass
    return cpu_info

def read_and_plot_csv(csv_file="dgemm_benchmark_results.csv"):
    """CSVファイルを読み込み、1–1000までの単一グラフを描画する関数"""
    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found.")
        return

    # データ読み込み・平均値計算
    df = pd.read_csv(csv_file)
    gflops_cols = [c for c in df.columns if "GFLOPS" in c]
    df["mean_gflops"] = df[gflops_cols].mean(axis=1)
    df["std_gflops"]  = df[gflops_cols].std(axis=1)

    # CPU情報
    cpu_info = get_cpu_info()

    # コンソール出力
    print(f"CPU: {cpu_info}")
    print(f"Matrix size range: {df['m'].min()} to {df['m'].max()}")
    print(f"Average performance: {df['mean_gflops'].mean():.4f} GFLOPS")
    print(f"Peak performance: {df['mean_gflops'].max():.4f} GFLOPS (at size {df.loc[df['mean_gflops'].idxmax(), 'm']})")

    # グラフ描画
    plt.figure(figsize=(12, 8))
    plt.plot(df["m"], df["mean_gflops"],
             "o-", markersize=4, linewidth=1.5,
             label="Mean Performance", zorder=2)

    # キャッシュ境界（L1/L2, L2/L3, L3/Memory）を示す縦線とラベル
    boundaries = [
        (43, 'L1'),
        (128, 'L2'),
        (319, 'L3'),
    ]
    ymin, ymax = plt.ylim()
    for x, label in boundaries:
        plt.axvline(x=x, color="black", linestyle="--", linewidth=1, zorder=1)
        plt.text(
            x - 10,
            ymax - (ymax - ymin) * 0.4,              # 上端から40%下
            label,
            verticalalignment="top",
            fontsize=12,
            color="red",                             # 文字色を赤に
            backgroundcolor="white",
            fontweight="bold", 
            zorder=3
        )

    plt.grid(True, linestyle="--", alpha=0.7, zorder=0)
    plt.title(f"DGEMM Performance (1–1000 Matrix Size) on {cpu_info}", fontsize=16)
    plt.xlabel("Matrix Size (N×N)", fontsize=14)
    plt.ylabel("Performance (GFLOPS)", fontsize=14)

    # ピーク性能のハイライト
    peak_idx   = df["mean_gflops"].idxmax()
    peak_size  = df.loc[peak_idx, "m"]
    peak_gflops= df.loc[peak_idx, "mean_gflops"]
    plt.scatter(
        [peak_size], [peak_gflops],
        color="red", s=100, zorder=4,
        label=f"Peak: {peak_gflops:.4f} GFLOPS at size {peak_size}"
    )

    plt.legend(loc="best", fontsize=12)
    output_file = "dgemm_benchmark_simple_plot.png"
    plt.savefig(output_file, dpi=300)
    print(f"Performance visualization saved to {output_file}")
    return df

if __name__ == "__main__":
    read_and_plot_csv()
