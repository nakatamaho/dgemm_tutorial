import pandas as pd
import matplotlib.pyplot as plt
import platform

# グローバル設定をリセット
plt.rcdefaults()

# CPUモデル名を取得する関数
def get_cpu_model():
    try:
        if platform.system() == "Linux":
            # Linuxの場合
            with open('/proc/cpuinfo', 'r') as f:
                for line in f:
                    if line.startswith('model name'):
                        return line.split(':')[1].strip()
            return "Unknown CPU"
        elif platform.system() == "Darwin":
            # macOSの場合
            command = "sysctl -n machdep.cpu.brand_string"
            return subprocess.check_output(command.split()).decode().strip()
        elif platform.system() == "Windows":
            # Windowsの場合
            command = "wmic cpu get name"
            output = subprocess.check_output(command.split()).decode()
            return output.strip().split('\n')[1]
        else:
            return "Unknown CPU"
    except:
        return "Unknown CPU"

# CPUモデル名を取得
cpu_model = get_cpu_model()
# 長すぎる場合は短くする
if len(cpu_model) > 50:
    cpu_model = re.sub(r'\([^)]*\)', '', cpu_model)  # カッコ内の情報を削除
    cpu_model = re.sub(r'\s+', ' ', cpu_model).strip()  # 複数の空白を1つに

print(f"検出されたCPU: {cpu_model}")

# CSVファイルの読み込み
df = pd.read_csv("openblas_results.csv")

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
plt.title(f"DGEMM Performance on {cpu_model}")
plt.legend()
plt.grid()
plt.xlim(left=0)
plt.ylim(bottom=0)

# PDFとPNGで保存
plt.savefig("dgemm_flops.pdf")
plt.savefig("dgemm_flops.png")

plt.show()

