import pandas as pd
import matplotlib.pyplot as plt

# CSVファイルの読み込み
df = pd.read_csv("dgemm_benchmark_results.csv")

# 最後の列の値がすべて0の場合、その列を削除
df = df.loc[:, (df != 0).any(axis=0)]

# FLOPS を GFLOPS に変換
df["maxflops"] /= 1000
df["minflops"] /= 1000

# プロットの作成
plt.figure(figsize=(10, 6))
plt.plot(df["n"], df["minflops"], label="Min FLOPS", marker='s', markersize=0.5, linestyle='None')
plt.plot(df["n"], df["maxflops"], label="Max FLOPS", marker='o', markersize=0.5, linestyle='None')

# Theoretical Peak Performance
peak_performance = 1894.4
plt.axhline(y=peak_performance, color='red', linestyle='--', linewidth=1, label='Theoretical Peak Performance')
plt.text(df["n"].max(), peak_performance, f"{peak_performance} GFLOPS", color='red', verticalalignment='bottom', horizontalalignment='right')


plt.xlabel("Matrix Size (n x n)")
plt.ylabel("FLOPS (GFLOPS)")
plt.title("DGEMM Performance")
plt.legend()
plt.grid()
plt.xlim(left=0)
plt.ylim(bottom=0)

# PDFとPNGで保存
plt.savefig("dgemm_flops.pdf")
plt.savefig("dgemm_flops.png")

plt.show()

