# OpenBLASの結果との比較

この章では、前述のシンプルな実装とOpenBLASライブラリを使用したDGEMM実装の性能を比較します。OpenBLASは高度に最適化された行列演算ライブラリであり、様々なアーキテクチャに対して最適化されています。

## OpenBLASを使用したベンチマーク

OpenBLASを使用するには、まずシステムにインストールする必要があります。主要なプラットフォームでは以下のコマンドでインストールできます：

### Ubuntu/Debian:
```bash
sudo apt-get install libopenblas-dev
```

以下はOpenBLASを使用したDGEMMのベンチマークコードです。このコードはシンプルなDGEMM実装と同様の方法でパフォーマンスを測定します。

```cpp
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <cblas.h>
#ifdef _OPENMP
#include <omp.h>
#endif

// ランダム行列生成関数
void generate_random_matrix(int rows, int cols, double* matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = dist(gen);
    }
}

// ベンチマーク関数
template <typename Func>
double benchmark(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

// 平均と分散の計算
std::pair<double, double> calculate_mean_and_variance(const std::vector<double> &values) {
    double mean = 0.0;
    double variance = 0.0;

    for (double value : values) {
        mean += value;
    }
    mean /= values.size();

    for (double value : values) {
        double diff = (value - mean);
        variance += diff * diff;
    }
    variance /= values.size();

    return {mean, variance};
}

int main() {
#ifdef _OPENMP
    std::cout << "OpenMP is enabled.\n";
    std::cout << "Number of threads (max): " << omp_get_max_threads() << "\n";
#else
    std::cout << "OpenMP is not enabled.\n";
#endif

    // OpenBLASのスレッド数設定（必要に応じて）
    // openblas_set_num_threads(4);
    
    // 1から1000まで7ずつのサイズを生成（正方行列用）
    std::vector<int> sizes;
    for (int size = 1; size <= 1000; size += 7) {
        sizes.push_back(size);
    }
    
    const int num_trials = 5;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    // 結果を保存するファイルを開く
    std::ofstream result_file("openblas_results.csv");
    result_file << "Size,Mean_GFLOPS,Variance\n";

    for (auto size : sizes) {
        // m = n = k = size の正方行列
        int m = size;
        int n = size;
        int k = size;
        double flop_count = static_cast<double>(m) * n * (2.0 * k + 1);

        std::vector<double> A(m * k);
        std::vector<double> B(k * n);
        std::vector<double> C(m * n);

        double alpha = dist(mt);
        double beta = dist(mt);

        generate_random_matrix(m, k, A.data());
        generate_random_matrix(k, n, B.data());
        generate_random_matrix(m, n, C.data());

        std::cout << "Benchmarking m=" << m << ", n=" << n << ", k=" << k << ":\n";

        // OpenBLAS実装のベンチマーク
        std::vector<double> openblas_flops_results;
        for (int trial = 0; trial < num_trials; ++trial) {
            std::vector<double> C_test = C;
            double elapsed = benchmark([&]() {
                // OpenBLASのdgemm呼び出し
                // C = alpha * A * B + beta * C
                // CblasRowMajor: 行優先
                // CblasNoTrans: 転置なし
                cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                           m, n, k, alpha, A.data(), k, B.data(), n, 
                           beta, C_test.data(), n);
            });
            double flops = flop_count / elapsed / 1.0e9; // GFLOPS
            openblas_flops_results.push_back(flops);
        }

        auto [mean_openblas_flops, var_openblas_flops] = calculate_mean_and_variance(openblas_flops_results);

        std::cout << "OpenBLAS Implementation:\n";
        std::cout << "  FLOPS for each trial [GFLOPS]: ";
        for (const auto &val : openblas_flops_results) {
            std::cout << val << " ";
        }
        std::cout << "\n";
        std::cout << "  Mean FLOPS: " << mean_openblas_flops << " GFLOPS, Variance: " << var_openblas_flops << "\n";
        std::cout << "------------------------------------------------\n";
        
        // 結果をCSVに書き込む
        result_file << size << "," << mean_openblas_flops << "," << var_openblas_flops << "\n";
    }
    
    result_file.close();
    return 0;
}
```

## コンパイルと実行

以下のMakefileを使用してコンパイルと実行を行います：

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -fopenmp
LDFLAGS = -lopenblas -lpthread -fopenmp

all: benchmark_openblas

benchmark_openblas: benchmark_openblas.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f benchmark_openblas openblas_results.csv
```

コンパイルと実行は以下のコマンドで行います：

```bash
make
./benchmark_openblas
```
## OpenBLASの性能向上要因

OpenBLASがシンプルな実装と比較して大幅な性能向上を実現できる理由は以下の通りです：

1. **アーキテクチャ最適化**: 
   - SIMD命令（SSE、AVX、AVX2、AVX-512など）を効率的に活用
   - 各CPU世代に特化したアセンブリコードの使用
   - パイプラインの最適化

2. **キャッシュ最適化**: 
   - ブロック化アルゴリズムによるキャッシュ効率の向上
   - メモリアクセスパターンの最適化
   - プリフェッチの活用

3. **マルチスレッド最適化**:
   - 効率的なスレッド並列処理
   - 負荷バランスの最適化
   - NUMA（Non-Uniform Memory Access）アーキテクチャへの対応

4. **アルゴリズムの最適化**:
   - 数値的に安定した実装
   - 特殊なケース（三角行列、対称行列など）に対する最適化
   - 自動チューニング機能

## 予想される結果の考察

一般的に、OpenBLASとシンプル実装の性能比較では、以下のような傾向が見られます：

1. **行列サイズと性能スケーリング**:
   - 小さな行列（N < 100）: OpenBLASの初期化オーバーヘッドにより、相対的な性能差は小さい
   - 中規模行列（100 < N < 500）: キャッシュ最適化の効果が現れ始め、性能差が拡大
   - 大規模行列（N > 500）: 並列処理とブロック化の効果が最大限に発揮され、最も大きな性能差

2. **スレッド数と性能**:
   - マルチコアCPUでは、OpenBLASは自動的に利用可能なコアを活用
   - シングルスレッド実装との比較では、コア数が多いほど性能差が拡大

3. **プラットフォームによる差異**:
   - IntelプロセッサとAMDプロセッサで最適化の効果が異なる
   - AVXやAVX2などの拡張命令セットをサポートするプロセッサでより高い性能

