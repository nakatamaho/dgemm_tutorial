import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 結果ファイルの読み込み
# simple_results.csvが存在しない場合に対応
try:
    simple_results = pd.read_csv('simple_results.csv')
    has_simple_results = True
except FileNotFoundError:
    print("simple_results.csv が見つかりません。OpenBLASの結果のみを表示します。")
    has_simple_results = False

try:
    openblas_results = pd.read_csv('openblas_results.csv')
except FileNotFoundError:
    print("openblas_results.csv が見つかりません。ファイルが生成されているか確認してください。")
    exit(1)

# データの準備
sizes = openblas_results['Size']
openblas_flops = openblas_results['Mean_GFLOPS']

# プロット
plt.figure(figsize=(12, 8))

if has_simple_results:
    simple_flops = simple_results['Mean_GFLOPS']
    plt.plot(sizes, simple_flops, 'b-', label='Simple Implementation')
    
plt.plot(sizes, openblas_flops, 'r-', label='OpenBLAS')
plt.xlabel('Matrix Size (N x N)')
plt.ylabel('Performance (GFLOPS)')
plt.title('DGEMM Performance Comparison')
plt.legend()
plt.grid(True)

# y軸を対数スケールで表示
plt.yscale('log')

# 結果を保存
plt.savefig('dgemm_performance.png', dpi=300)

# スピードアップ比を計算（シンプル実装の結果がある場合のみ）
if has_simple_results:
    speedup = openblas_flops / simple_flops
    
    # スピードアップのプロット
    plt.figure(figsize=(12, 8))
    plt.plot(sizes, speedup, 'g-')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('Speedup (OpenBLAS / Simple)')
    plt.title('OpenBLAS Speedup over Simple Implementation')
    plt.grid(True)
    plt.savefig('dgemm_speedup.png', dpi=300)
    
    # 統計情報の出力
    print(f"Average speedup: {np.mean(speedup):.2f}x")
    print(f"Maximum speedup: {np.max(speedup):.2f}x at size {sizes[np.argmax(speedup)]}")
    print(f"Minimum speedup: {np.min(speedup):.2f}x at size {sizes[np.argmin(speedup)]}")

# OpenBLASの性能情報表示
print("\nOpenBLAS Performance Statistics:")
print(f"Average GFLOPS: {np.mean(openblas_flops):.2f}")
print(f"Maximum GFLOPS: {np.max(openblas_flops):.2f} at size {sizes[np.argmax(openblas_flops)]}")
print(f"Minimum GFLOPS: {np.min(openblas_flops):.2f} at size {sizes[np.argmin(openblas_flops)]}")

# 結果の表示
plt.show()
