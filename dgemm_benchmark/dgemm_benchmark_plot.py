import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("dgemm_benchmark_results.csv")

# 
plt.figure(figsize=(10, 6))
plt.plot(df["n"], df["maxflops"], label="Max FLOPS", marker='o', markersize=1, linestyle='None')
plt.plot(df["n"], df["minflops"], label="Min FLOPS", marker='s', markersize=1, linestyle='None')
plt.xlabel("Matrix Size (n x n)")
plt.ylabel("FLOPS (MFLOPS)")
plt.title("DGEMM Performance")
plt.legend()
plt.grid()

# PDFPNG
plt.savefig("dgemm_flops.pdf")
plt.savefig("dgemm_flops.png")

plt.show()

