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
                # PowerShellを使用してCPU情報を取得する代替方法
                output = subprocess.check_output(["powershell", "Get-WmiObject -Class Win32_Processor | Select-Object -ExpandProperty Name"], 
                                             shell=True).decode()
                cpu_info = output.strip()
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
    
    # プロットの設定
    plt.figure(figsize=(12, 8))
    
    # メインプロット（行列サイズと性能の関係）
    plt.subplot(2, 1, 1)
    plt.errorbar(df['m'], df['mean_gflops'], yerr=df['std_gflops'], 
                 fmt='o-', capsize=3, markersize=4, alpha=0.7, linewidth=1)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.title(f'DGEMM Performance on {cpu_info}', fontsize=16)
    plt.ylabel('Performance (GFLOPS)', fontsize=14)
    plt.xscale('log')  # x軸を対数スケールに
    
    # 特定の範囲をズームイン（データに応じて調整）
    zoom_min = 100
    zoom_max = 500
    if df['m'].max() > zoom_max and df['m'].min() < zoom_min:
        plt.subplot(2, 1, 2)
        zoom_data = df[(df['m'] >= zoom_min) & (df['m'] <= zoom_max)]
        plt.plot(zoom_data['m'], zoom_data['mean_gflops'], 'o-', markersize=6)
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.title(f'Zoom in: Matrix Size {zoom_min} to {zoom_max}', fontsize=14)
        plt.xlabel('Matrix Size (N×N)', fontsize=14)
        plt.ylabel('Performance (GFLOPS)', fontsize=14)
    else:
        plt.subplot(2, 1, 2)
        # 各サイズのGFLOPS分布を箱ひげ図で表示
        sizes_to_plot = sorted(df['m'].unique())
        if len(sizes_to_plot) > 20:
            # サイズが多すぎる場合は間引き
            step = len(sizes_to_plot) // 10
            sizes_to_plot = sizes_to_plot[::step]
        
        box_data = []
        box_labels = []
        
        for size in sizes_to_plot:
            size_data = df[df['m'] == size]
            if not size_data.empty:
                box_data.append(size_data[gflops_cols].values.flatten())
                box_labels.append(str(size))
        
        plt.boxplot(box_data, labels=box_labels)
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.title('Performance Distribution by Matrix Size', fontsize=14)
        plt.xlabel('Matrix Size (N×N)', fontsize=14)
        plt.ylabel('Performance (GFLOPS)', fontsize=14)
        plt.xticks(rotation=45)
    
    plt.tight_layout()
    
    # グラフを保存
    output_file = "dgemm_benchmark_plot.png"
    plt.savefig(output_file, dpi=300)
    
    # 統計情報を出力
    print(f"CPU: {cpu_info}")
    print(f"Matrix size range: {df['m'].min()} to {df['m'].max()}")
    print(f"Average performance: {df['mean_gflops'].mean():.4f} GFLOPS")
    print(f"Peak performance: {df['mean_gflops'].max():.4f} GFLOPS (at size {df.loc[df['mean_gflops'].idxmax(), 'm']})")
    print(f"Performance visualization saved to {output_file}")
    
    plt.show()

if __name__ == "__main__":
    read_and_plot_csv()
