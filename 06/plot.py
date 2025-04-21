import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import platform
import subprocess
import re
import os

def get_cpu_info():
    """CPUの情報を取得する関数"""
    cpu_info = "Unknown CPU"
    
    if platform.system() == "Linux":
        try:
            # /proc/cpuinfo からCPU情報を取得
            with open("/proc/cpuinfo", "r") as f:
                for line in f:
                    if "model name" in line:
                        cpu_info = line.split(":")[1].strip()
                        break
        except:
            pass
            
    elif platform.system() == "Darwin":  # macOS
        try:
            # sysctlコマンドでCPU情報を取得
            output = subprocess.check_output(["sysctl", "-n", "machdep.cpu.brand_string"]).decode().strip()
            if output:
                cpu_info = output
        except:
            pass
            
    elif platform.system() == "Windows":
        try:
            # WMICコマンドでCPU情報を取得
            output = subprocess.check_output(["wmic", "cpu", "get", "name"]).decode()
            cpu_info = output.strip().split("\n")[1]
        except:
            try:
                # platform.processor()を試す
                cpu_info = platform.processor()
            except:
                pass
    
    return cpu_info

def read_and_plot_csv(csv_file="dgemm_benchmark_results.csv"):
    """CSVファイルを読み込み、グラフを描画する関数"""
    # CSVファイルが存在するか確認
    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found.")
        return
    
    # CSVファイルを読み込み
    df = pd.read_csv(csv_file)
    
    # 各試行の平均GFLOPSを計算
    gflops_cols = [col for col in df.columns if 'GFLOPS' in col]
    df['mean_gflops'] = df[gflops_cols].mean(axis=1)
    df['std_gflops'] = df[gflops_cols].std(axis=1)
    
    # CPU情報を取得
    cpu_info = get_cpu_info()
    
    # 結果をコンソールに出力
    print(f"CPU: {cpu_info}")
    print(f"Matrix size range: {df['m'].min()} to {df['m'].max()}")
    print(f"Average performance: {df['mean_gflops'].mean():.4f} GFLOPS")
    print(f"Peak performance: {df['mean_gflops'].max():.4f} GFLOPS (at size {df.loc[df['mean_gflops'].idxmax(), 'm']})")
    
    # 単一グラフの設定
    plt.figure(figsize=(12, 8))
    
    # 行列サイズとGFLOPSの関係をプロット（エラーバー付き）
    plt.errorbar(df['m'], df['mean_gflops'], yerr=df['std_gflops'], 
                fmt='o-', capsize=3, markersize=4, alpha=0.8, linewidth=1.5,
                color='#1f77b4', label='Mean Performance')
    
    # グリッド線を追加
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # タイトルと軸ラベル
    plt.title(f'DGEMM Performance on {cpu_info}', fontsize=16)
    plt.xlabel('Matrix Size (N×N)', fontsize=14)
    plt.ylabel('Performance (GFLOPS)', fontsize=14)
    
    # ピーク性能をハイライト
    peak_idx = df['mean_gflops'].idxmax()
    peak_size = df.loc[peak_idx, 'm']
    plt.scatter([peak_size], [df.loc[peak_idx, 'mean_gflops']], 
                color='red', s=100, zorder=5, label=f'Peak: {df.loc[peak_idx, "mean_gflops"]:.4f} GFLOPS')
    
    plt.annotate(f'Peak at size {peak_size}',
                xy=(peak_size, df.loc[peak_idx, 'mean_gflops']),
                xytext=(peak_size+50, df.loc[peak_idx, 'mean_gflops']+0.02),
                arrowprops=dict(facecolor='black', shrink=0.05, width=1),
                fontsize=12)
    
    # 凡例を追加
    plt.legend(loc='best', fontsize=12)
    
    # キャッシュサイズのラインを追加（推定）
    data_size_KB = lambda n: (n * n * 8 * 3) / 1024  # 8バイト（double型）× n^2 × 3（A, B, C行列）
    
    # 典型的なキャッシュサイズ
    l1_size_KB = 32    # 32KB
    l2_size_KB = 256   # 256KB
    l3_size_KB = 8192  # 8MB
    
    # キャッシュサイズに相当する行列サイズを計算
    l1_matrix_size = int(np.sqrt(l1_size_KB / (8 * 3 / 1024)))
    l2_matrix_size = int(np.sqrt(l2_size_KB / (8 * 3 / 1024)))
    l3_matrix_size = int(np.sqrt(l3_size_KB / (8 * 3 / 1024)))
    
    # キャッシュサイズのラインを描画（データ範囲内の場合のみ）
    y_min, y_max = plt.ylim()
    
    if df['m'].min() <= l1_matrix_size <= df['m'].max():
        plt.axvline(x=l1_matrix_size, color='r', linestyle='--', alpha=0.5, 
                    label=f'~L1 Cache ({l1_size_KB}KB)')
    
    if df['m'].min() <= l2_matrix_size <= df['m'].max():
        plt.axvline(x=l2_matrix_size, color='g', linestyle='--', alpha=0.5, 
                    label=f'~L2 Cache ({l2_size_KB}KB)')
    
    if df['m'].min() <= l3_matrix_size <= df['m'].max():
        plt.axvline(x=l3_matrix_size, color='b', linestyle='--', alpha=0.5, 
                    label=f'~L3 Cache ({l3_size_KB}KB)')
    
    plt.legend(loc='best')
    plt.tight_layout()
    
    # グラフを保存
    output_file = "dgemm_benchmark_plot.png"
    plt.savefig(output_file, dpi=300)
    print(f"Performance visualization saved to {output_file}")
    
    return df

if __name__ == "__main__":
    read_and_plot_csv()
