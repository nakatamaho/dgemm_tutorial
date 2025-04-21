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
            with open("/proc/cpuinfo", "r") as f:
                for line in f:
                    if "model name" in line:
                        cpu_info = line.split(":")[1].strip()
                        break
        except:
            pass
            
    elif platform.system() == "Darwin":  # macOS
        try:
            output = subprocess.check_output(["sysctl", "-n", "machdep.cpu.brand_string"]).decode().strip()
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
    """CSVファイルを読み込み、1-1000までの単一グラフを描画する関数"""
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
    
    # 単一グラフの作成
    plt.figure(figsize=(12, 8))
    
    # 行列サイズとGFLOPSの関係をプロット
    plt.plot(df['m'], df['mean_gflops'], 'o-', markersize=4, linewidth=1.5, label='Mean Performance')
    
    # グリッド線を追加
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # タイトルと軸ラベル
    plt.title(f'DGEMM Performance (1-1000 Matrix Size) on {cpu_info}', fontsize=16)
    plt.xlabel('Matrix Size (N×N)', fontsize=14)
    plt.ylabel('Performance (GFLOPS)', fontsize=14)
    
    # ピーク性能をハイライト
    peak_idx = df['mean_gflops'].idxmax()
    peak_size = df.loc[peak_idx, 'm']
    peak_gflops = df.loc[peak_idx, 'mean_gflops']
    
    plt.scatter([peak_size], [peak_gflops], color='red', s=100, zorder=5, 
                label=f'Peak: {peak_gflops:.4f} GFLOPS at size {peak_size}')
    
    # 凡例を追加
    plt.legend(loc='best', fontsize=12)
    
    # グラフを保存
    output_file = "dgemm_benchmark_simple_plot.png"
    plt.savefig(output_file, dpi=300)
    print(f"Performance visualization saved to {output_file}")
    
    return df

if __name__ == "__main__":
    read_and_plot_csv()
