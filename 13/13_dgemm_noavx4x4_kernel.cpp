#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

#ifdef _OPENMP
#include <omp.h>
#endif

// 4x4 マイクロカーネル (AVX2なし)
void noavx_micro_kernel_4x4(int k, const double *A, int lda,
                           const double *B, int ldb, double *C, int ldc) {
    // 一時変数に計算結果を蓄積
    double c00 = 0.0, c01 = 0.0, c02 = 0.0, c03 = 0.0;
    double c10 = 0.0, c11 = 0.0, c12 = 0.0, c13 = 0.0;
    double c20 = 0.0, c21 = 0.0, c22 = 0.0, c23 = 0.0;
    double c30 = 0.0, c31 = 0.0, c32 = 0.0, c33 = 0.0;
    
    // k方向に行列積を計算
    for (int l = 0; l < k; l++) {
        // Aの要素をロード
        double a0 = A[0 + l * lda];
        double a1 = A[1 + l * lda];
        double a2 = A[2 + l * lda];
        double a3 = A[3 + l * lda];
        
        // Bの要素をロード
        double b0 = B[l + 0 * ldb];
        double b1 = B[l + 1 * ldb];
        double b2 = B[l + 2 * ldb];
        double b3 = B[l + 3 * ldb];
        
        // 行列積の計算
        c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
        c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
        c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
        c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
    }
    
    // 結果をCに格納
    C[0 + 0 * ldc] = c00; C[0 + 1 * ldc] = c01; C[0 + 2 * ldc] = c02; C[0 + 3 * ldc] = c03;
    C[1 + 0 * ldc] = c10; C[1 + 1 * ldc] = c11; C[1 + 2 * ldc] = c12; C[1 + 3 * ldc] = c13;
    C[2 + 0 * ldc] = c20; C[2 + 1 * ldc] = c21; C[2 + 2 * ldc] = c22; C[2 + 3 * ldc] = c23;
    C[3 + 0 * ldc] = c30; C[3 + 1 * ldc] = c31; C[3 + 2 * ldc] = c32; C[3 + 3 * ldc] = c33;
}

// 4x4マイクロカーネルを使用したDGEMM実装(NN版) - 4の倍数サイズのみ対応
void dgemm_noavx4x4_kernel_nn(int m, int n, int k, double alpha, const double *A, int lda,
                            const double *B, int ldb, double beta, double *C, int ldc) {
    // 簡単なケースの処理
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) {
        return;  // 何もしない
    }
    
    // 4の倍数サイズのチェック
    if (m % 4 != 0 || n % 4 != 0 || k % 4 != 0) {
        std::cerr << "Error: Matrix dimensions must be multiples of 4." << std::endl;
        return;
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
    
    // 一時バッファの確保
    std::vector<double> C_temp(4 * 4);
    // A, B用の一時バッファを追加
    std::vector<double> Ablock(4 * k);  // 4行 x k列
    std::vector<double> Bblock(k * 4);  // k行 x 4列
    
    // ブロックごとに処理（4x4ブロック単位）
    for (int j = 0; j < n; j += 4) {
        for (int i = 0; i < m; i += 4) {
            // 一時バッファをゼロ初期化
            for (int idx = 0; idx < 16; idx++) {
                C_temp[idx] = 0.0;
            }
            
            // Aをコピー - 4行 x k列のブロック
            for (int l = 0; l < k; l++) {
                for (int ii = 0; ii < 4; ii++) {
                    Ablock[ii + l * 4] = A[(i + ii) + l * lda];
                }
            }
            
            // Bをコピーして同時にα倍 - k行 x 4列のブロック
            for (int jj = 0; jj < 4; jj++) {
                for (int l = 0; l < k; l++) {
                    Bblock[l + jj * k] = alpha * B[l + (j + jj) * ldb];
                }
            }
            
            // マイクロカーネルを呼び出し - Ablock, Bblockを使用
            noavx_micro_kernel_4x4(k, Ablock.data(), 4, Bblock.data(), k, C_temp.data(), 4);
            
            // 結果をCに加算 (すでにBblockはalpha倍されているので、ここではalpha倍しない)
            // betaも同時に適用する 
            for (int jj = 0; jj < 4; jj++) {
                for (int ii = 0; ii < 4; ii++) {
                    if (beta == 0.0) {
                        // beta = 0の場合は単に結果を代入
                        C[(i + ii) + (j + jj) * ldc] = C_temp[ii + jj * 4];
                    } else {
                        // FMA可能な形式: C = beta * C + C_temp
                        C[(i + ii) + (j + jj) * ldc] = beta * C[(i + ii) + (j + jj) * ldc] + C_temp[ii + jj * 4];
                    }
                }
            }
        }
    }
}

// ランダム行列の生成
void generate_random_matrix(int rows, int cols, double *matrix) {
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            matrix[i + j * rows] = dist(mt);
        }
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

int main() {
    // 結果をCSVファイルに出力するための準備
    std::ofstream csv_file("dgemm_benchmark_noavx4x4kernel_results.csv");
    
    if (!csv_file.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }

#ifdef _OPENMP
    std::cout << "OpenMP is enabled.\n";
    std::cout << "Number of threads (max): " << omp_get_max_threads() << "\n";
#else
    std::cout << "OpenMP is not enabled.\n";
#endif

    // CSV形式のヘッダーを出力
    csv_file << "m,n,k,GFLOPS1,GFLOPS2,GFLOPS3,GFLOPS4,GFLOPS5" << std::endl;
    
    std::vector<int> sizes;

    // 4の倍数サイズ（4～124）
    for (int size = 4; size <= 124; size += 4) {
        sizes.push_back(size);
    }

    // 8の倍数サイズ（128以上）
    for (int size = 128; size <= 3500; size += 8) {
        sizes.push_back(size);
    }

    const int num_trials = 5;  // 5回の試行
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (auto size : sizes) {
        // 進捗表示
        std::cout << "Benchmarking size " << size << "..." << std::endl;
        
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

        // CSV行の開始: m,n,k
        csv_file << m << "," << n << "," << k;

        // 4x4ノンAVXカーネル実装のベンチマーク
        for (int trial = 0; trial < num_trials; ++trial) {
            std::vector<double> C_test = C;
            double elapsed = benchmark([&]() {
                dgemm_noavx4x4_kernel_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m);
            });
            double flops = flop_count / elapsed / 1.0e9; // GFLOPS
            
            // GFLOPSの値をCSV形式で追加
            csv_file << "," << flops;
        }
        
        // 行の終了
        csv_file << std::endl;
    }

    csv_file.close();
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_noavx4x4kernel_results.csv" << std::endl;

    return 0;
}
