import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import platform
import subprocess
import os
import sys
import argparse

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

def read_and_plot_csv(main_csv="dgemm_benchmark_noavx_copy_trans_kernel_results.csv", 
                     comparison_csv=None):
    """CSVファイルを読み込み、グラフを描画する関数
    Parameters:
    -----------
    main_csv : str
        メインとなるCSVファイルのパス
    comparison_csv : str or None
        比較用のCSVファイルのパス。Noneの場合は比較を行わない
    """
    if not os.path.exists(main_csv):
        print(f"Error: {main_csv} not found.")
        return

    # メインのデータ読み込み・平均値計算
    df_main = pd.read_csv(main_csv)
    gflops_cols = [c for c in df_main.columns if "GFLOPS" in c]
    df_main["mean_gflops"] = df_main[gflops_cols].mean(axis=1)
    df_main["std_gflops"]  = df_main[gflops_cols].std(axis=1)

    # 比較用データの読み込み（指定がある場合）
    if comparison_csv and os.path.exists(comparison_csv):
        df_comp = pd.read_csv(comparison_csv)
        gflops_cols = [c for c in df_comp.columns if "GFLOPS" in c]
        df_comp["mean_gflops"] = df_comp[gflops_cols].mean(axis=1)
        df_comp["std_gflops"]  = df_comp[gflops_cols].std(axis=1)
    else:
        df_comp = None
        if comparison_csv:
            print(f"Warning: Comparison CSV file {comparison_csv} not found.")

    # CPU情報
    cpu_info = get_cpu_info()

    # コンソール出力（メインデータ）
    main_name = os.path.basename(main_csv).replace('.csv', '')
    print(f"CPU: {cpu_info}")
    print(f"[{main_name}]")
    print(f"Matrix size range: {df_main['m'].min()} to {df_main['m'].max()}")
    print(f"Average performance: {df_main['mean_gflops'].mean():.4f} GFLOPS")
    print(f"Peak performance: {df_main['mean_gflops'].max():.4f} GFLOPS (at size {df_main.loc[df_main['mean_gflops'].idxmax(), 'm']})")

    # 比較データの情報出力
    if df_comp is not None:
        comp_name = os.path.basename(comparison_csv).replace('.csv', '')
        print(f"\n[{comp_name}]")
        print(f"Matrix size range: {df_comp['m'].min()} to {df_comp['m'].max()}")
        print(f"Average performance: {df_comp['mean_gflops'].mean():.4f} GFLOPS")
        print(f"Peak performance: {df_comp['mean_gflops'].max():.4f} GFLOPS (at size {df_comp.loc[df_comp['mean_gflops'].idxmax(), 'm']})")

    # グラフ描画
    plt.figure(figsize=(12, 8))
    
    # メインデータのプロット
    main_label = main_name.replace('dgemm_benchmark_', '').replace('_results', '')
    plt.plot(df_main["m"], df_main["mean_gflops"],
             "o-", markersize=4, linewidth=1.5, color='blue',
             label=f"{main_label}", zorder=2)

    # 比較データのプロット（ある場合）
    if df_comp is not None:
        comp_label = comp_name.replace('dgemm_benchmark_', '').replace('_results', '')
        plt.plot(df_comp["m"], df_comp["mean_gflops"],
                "s--", markersize=4, linewidth=1.5, color='red',
                label=f"{comp_label}", zorder=3)

    # キャッシュ境界（L1/L2, L2/L3, L3/Memory）を示す縦線とラベル
    boundaries = [
        (45, 'L1'),
        (1024, 'L2'),
        (2896, 'L3'),
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
    
    # タイトルの作成
    if df_comp is not None:
        plt.title(f"DGEMM Performance Comparison on {cpu_info}", fontsize=16)
    else:
        plt.title(f"{main_label} DGEMM Performance on {cpu_info}", fontsize=16)
    
    plt.xlabel("Matrix Size (N×N)", fontsize=14)
    plt.ylabel("Performance (GFLOPS)", fontsize=14)

    # ピーク性能のハイライト（メイン）
    peak_idx_main = df_main["mean_gflops"].idxmax()
    peak_size_main = df_main.loc[peak_idx_main, "m"]
    peak_gflops_main = df_main.loc[peak_idx_main, "mean_gflops"]
    plt.scatter(
        [peak_size_main], [peak_gflops_main],
        color="blue", s=100, zorder=4,
        label=f"{main_label} Peak: {peak_gflops_main:.4f} GFLOPS at size {peak_size_main}"
    )

    # ピーク性能のハイライト（比較データ）
    if df_comp is not None:
        peak_idx_comp = df_comp["mean_gflops"].idxmax()
        peak_size_comp = df_comp.loc[peak_idx_comp, "m"]
        peak_gflops_comp = df_comp.loc[peak_idx_comp, "mean_gflops"]
        plt.scatter(
            [peak_size_comp], [peak_gflops_comp],
            color="red", s=100, zorder=4,
            label=f"{comp_label} Peak: {peak_gflops_comp:.4f} GFLOPS at size {peak_size_comp}"
        )

    plt.legend(loc="best", fontsize=12)
    
    # 出力ファイル名の決定
    if df_comp is not None:
        output_file = f"dgemm_benchmark_comparison_plot.png"
    else:
        output_file = f"dgemm_benchmark_{main_label}_plot.png"
    
    plt.savefig(output_file, dpi=300)
    print(f"\nPerformance visualization saved to {output_file}")
    return df_main, df_comp

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Plot DGEMM benchmark results')
    parser.add_argument('--main', default='dgemm_benchmark_noavx_copy_trans_kernel_results.csv',
                        help='Main CSV file to plot (automatically uses dgemm_benchmark_noavx_copy_trans_kernel_results if not specified)')
    parser.add_argument('--comp', default=None,
                        help='Comparison CSV file to plot (e.g. dgemm_benchmark_results.csv for naive implementation)')
    
    args = parser.parse_args()
    read_and_plot_csv(args.main, args.comp)
