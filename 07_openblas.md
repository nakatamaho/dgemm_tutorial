# OpenBLASの結果との比較

この章では、前述のシンプルな実装とOpenBLASライブラリを使用したDGEMM実装の性能を比較します。OpenBLASは高度に最適化された行列演算ライブラリであり、様々なアーキテクチャに対して最適化されています。

## OpenBLASを使用したベンチマーク

OpenBLASを使用するには、まずシステムにインストールする必要があります。主要なプラットフォームでは以下のコマンドでインストールできます：

### Install OpenBLAS
```bash
cd dgemm_tutorial/07
wget https://github.com/OpenMathLib/OpenBLAS/releases/download/v0.3.29/OpenBLAS-0.3.29.tar.gz
cd OpenBLAS-0.3.29/
make target=ZEN2
```

少々遅くても構わない場合は
```bash
sudo apt-get install libopenblas-dev
```
でもokです。

###
```Makefile
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -fopenmp
LDFLAGS = OpenBLAS-0.3.29/libopenblas.a -lpthread -fopenmp

all: benchmark_openblas benchmark_openblas_small

benchmark_openblas: benchmark_openblas.cpp
        $(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

benchmark_openblas_small: benchmark_openblas_small.cpp
        $(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
        rm -f benchmark_openblas benchmark_openblas_small openblas_results.csv
```

以下はOpenBLASを使用したDGEMMのベンチマークコードです。このコードはシンプルなDGEMM実装と同様の方法でパフォーマンスを測定します。

```cpp

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
```cpp
/*
 * Copyright (c) 2025
 *     Nakata, Maho
 *     All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <random>
#include <cstring>
#include <set>
#include <memory>
#include <vector>
#include <algorithm>
#include <mm_malloc.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define ALIGNMENT 64

extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double *alpha, const double *A, const int *lda, const double *B, const int *ldb, const double *beta, double *C, const int *ldc);
}


#define DIM_START 1
#define DIM_END 30000
#define NUMTRIALS 10
#define STEP 1

// 最も単純なDGEMM実装(NN版のみ)
void dgemm_simple_nn(int m, int n, int k, double alpha, const double *A, int lda,
                     const double *B, int ldb, double beta, double *C, int ldc) {
    // 簡単なケースの処理
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) {
        return;  // 何もしない
    }
    // alpha == 0の場合
    if (alpha == 0.0) {
        if (beta == 0.0) {
            // C = 0
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = 0.0;
                }
            }
        } else {
            // C = beta * C
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = beta * C[i + j * ldc];
                }
            }
        }
        return;
    }
    // メインの行列積計算: C = alpha * A * B + beta * C
    for (int j = 0; j < n; j++) {
        // betaによるCの初期化
        if (beta == 0.0) {
            for (int i = 0; i < m; i++) {
                C[i + j * ldc] = 0.0;
            }
        } else if (beta != 1.0) {
            for (int i = 0; i < m; i++) {
                C[i + j * ldc] = beta * C[i + j * ldc];
            }
        }
        // 行列積の計算
        for (int l = 0; l < k; l++) {
            if (B[l + j * ldb] != 0.0) {
                double temp = alpha * B[l + j * ldb];
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] += temp * A[i + l * lda];
                }
            }
        }
    }
}

// cf. https://netlib.org/lapack/lawnspdf/lawn41.pdf p.120
double flops_gemm(int k_i, int m_i, int n_i) {
    double adds, muls, flops;
    double k, m, n;
    m = (double)m_i;
    n = (double)n_i;
    k = (double)k_i;
    muls = m * (k + 2) * n;
    adds = m * k * n;
    flops = muls + adds;
    return flops;
}

void generate_random_matrix(int rows, int cols, double *matrix, int ld) {
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            matrix[i + j * ld] = dist(mt);
        }
    }
}

double compute_max_abs_diff(const double *ref, const double *test, int size) {
    double max_diff = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = fabs(ref[i] - test[i]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}

int main(int argc, char *argv[]) {
    bool no_check = false;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--nocheck") == 0) {
            no_check = true;
        }
    }

    if (!no_check) {
        std::cout << "m,n,k,maxflops,minflops,maxdiff" << std::endl;
    } else {
        std::cout << "m,n,k,maxflops,minflops" << std::endl;
    }
    const int num_trials = NUMTRIALS;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::set<int> n_values;
    for (int n = DIM_START; n <= DIM_END; n += STEP) {
        n_values.insert(n);
    }
    for (int m = 0; (128 * m) <= DIM_END; ++m) {
        int multiple128 = 128 * m;
        if (multiple128 >= DIM_START && multiple128 <= DIM_END)
            n_values.insert(multiple128);
        if (multiple128 - 128 >= DIM_START && multiple128 - 128 <= DIM_END)
            n_values.insert(multiple128 - 128);
        if (multiple128 + 128 >= DIM_START && multiple128 + 128 <= DIM_END)
            n_values.insert(multiple128 + 128);
    }

    std::vector<int> sorted_n_values(n_values.begin(), n_values.end());
    std::sort(sorted_n_values.begin(), sorted_n_values.end());

    for (int n : sorted_n_values) {
        int m = n, k = n, lda = m, ldb = k, ldc = m;
        double flop_count = flops_gemm(m, n, k);

        double *A = static_cast<double *>(_mm_malloc(lda * k * sizeof(double), ALIGNMENT));
        double *B = static_cast<double *>(_mm_malloc(ldb * n * sizeof(double), ALIGNMENT));
        double *C = static_cast<double *>(_mm_malloc(ldc * n * sizeof(double), ALIGNMENT));
        double *Corg = static_cast<double *>(_mm_malloc(ldc * n * sizeof(double), ALIGNMENT));
        double *Cref = static_cast<double *>(_mm_malloc(ldc * n * sizeof(double), ALIGNMENT));

        double alpha = dist(mt);
        double beta = dist(mt);

        generate_random_matrix(m, k, A, lda);
        generate_random_matrix(k, n, B, ldb);
        generate_random_matrix(m, n, C, ldc);

        memcpy(Corg, C, sizeof(double) * ldc * n);
        if (!no_check) {
            memcpy(Cref, C, sizeof(double) * ldc * n);
            dgemm_simple_nn(m, n, k, alpha, A, lda, B, ldb, beta, Cref, ldc);
        }

        double max_flops = 0.0;
        double min_flops = std::numeric_limits<double>::max();
        for (int trial = 0; trial < num_trials; trial++) {
            memcpy(C, Corg, sizeof(double) * ldc * n);
            auto start = std::chrono::high_resolution_clock::now();
            dgemm_("N", "N", &m, &n, &k, &alpha, A, &lda, B, &ldb, &beta, C, &ldc);
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed_time = std::chrono::duration<double>(end - start).count();
            double flops = flop_count / elapsed_time / 1.0e6;
            if (flops > max_flops) {
                max_flops = flops;
            }
            if (flops < min_flops) {
                min_flops = flops;
            }
        }

        double max_diff = no_check ? 0.0 : compute_max_abs_diff(C, Cref, m * n);
        if (!no_check) {
            std::cout << m << "," << n << "," << k << "," << max_flops << "," << min_flops << "," << max_diff << std::endl;
        } else {
            std::cout << m << "," << n << "," << k << "," << max_flops << "," << min_flops << std::endl;
        }

        _mm_free(A);
        _mm_free(B);
        _mm_free(C);
        _mm_free(Corg);
        _mm_free(Cref);
    }
    return 0;
}
```
コンパイルと実行は以下のコマンドで行います：

```bash
make
OMP_NUM_THREADS=32 GOMP_CPU_AFFINITY=0-31:1 ./benchmark_openblas 2>&1 | tee openblas_results.csv
python3 plot.py
```
nが1から20000までの正方行列を用い、ベンチマークは10回行い、最大FLOPSと最小FLOPSを見ています。

## 注意点

- 行列のメモリ確保64バイトのアラインメントでmallocしています
  ```cpp
  #define ALIGNMENT 64
  ...
  static_cast<double *>(_mm_malloc(lda * k * sizeof(double), ALIGNMENT))
  ```
  AVX2では256bits=32bytesでアラインメントするのが正しいですが、余裕(?)を持って64バイトのアラインメントでメモリを確保します。
  
- ハイパースレッドを使わず、実行時にCPUのコアとスレッドを固定します
  ```
  OMP_NUM_THREADS=32 GOMP_CPU_AFFINITY=0-31:1 
  ```
演算器をフルに活用する場合、ハイパースレッドを使う意味はありません。逆に遅くなる場合もあります。もしハイパースレッドで高速化する場合は他にボトルネックがあるからです。
また、CPUコアとスレッドを固定することで、無駄なスレッド割り当てが起こらず、若干性能が安定化する場合があります。
cf. [OpenBLASのIssue](https://github.com/OpenMathLib/OpenBLAS/issues/3435)

## OpenBLAS での最適化

1. **アーキテクチャ最適化**
   - SIMD命令（SSE、AVX、AVX2）を効率的に活用
   - 各CPU世代に特化したアセンブリコードの使用
   - パイプラインの最適化

2. **キャッシュ最適化**: 
   - ブロック化アルゴリズムによるキャッシュ効率の向上
   - メモリアクセスパターンの最適化
   - プリフェッチの活用

3. **マルチスレッド最適化**:
   - DGEMMではマルチスレッドにする、程度でパフォーマンスはほぼコア数に比例するように性能が出やすくまります。

## OpenBLAS での dgemmのベンチマーク結果と分析

![DGEMM FLOPS](https://raw.githubusercontent.com/nakatamaho/dgemm_tutorial/main/07/dgemm_flops.png)

1. **最大で理論ピーク性能の80%程度が出た**:
   - 行列サイズ N=19961（m=n=k=19961）のときに、最大で 約1. 536 TFLOPSが得られています。これは理論ピーク（約1.894 TFLOPS）の約81 %に相当します。
2. **性能を出すには大きな行列サイズが必要**
   - L1, L2程度に入るサイズだと、(N=360程度、N=1024程度)理論ピークになりにくいようです。
   - L3キャッシュが飽和するサイズにならないと、性能が高くならない。
3. **性能のブレが大きい**
   - メモリの使い方が悪いのか、意外と性能がぶれやすい。10回測定して、最大と最小FLOPSを得ますが、

4. **LINPACKとの比較**
   [AMD Threadripper 3970x Compute Performance Linpack and NAMD](https://www.pugetsystems.com/labs/hpc/amd-threadripper-3970x-compute-performance-linpack-and-namd-1631/)によると、AMD BLIS library v 2.0を用いて、1.3233TFLOPSが得られています。なのでOpenBLASは、AMD BLISライブラリより良く性能を引き出せているようです。

## naive DGEMMと比較



