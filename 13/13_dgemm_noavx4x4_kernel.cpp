#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

// 4x4 マイクロカーネル (AVX2なし)
void noavx_micro_kernel_4x4(int k,
                            const double *A, int lda,
                            const double *B, int ldb,
                            double *C, int ldc,
                            double alpha, double beta)
{
    /* temporary registers that will finally hold
       beta*C + alpha*Σ_l a(il)*b(lj)                                  */
    double c00 = beta * C[0 + 0 * ldc];
    double c01 = beta * C[0 + 1 * ldc];
    double c02 = beta * C[0 + 2 * ldc];
    double c03 = beta * C[0 + 3 * ldc];

    double c10 = beta * C[1 + 0 * ldc];
    double c11 = beta * C[1 + 1 * ldc];
    double c12 = beta * C[1 + 2 * ldc];
    double c13 = beta * C[1 + 3 * ldc];

    double c20 = beta * C[2 + 0 * ldc];
    double c21 = beta * C[2 + 1 * ldc];
    double c22 = beta * C[2 + 2 * ldc];
    double c23 = beta * C[2 + 3 * ldc];

    double c30 = beta * C[3 + 0 * ldc];
    double c31 = beta * C[3 + 1 * ldc];
    double c32 = beta * C[3 + 2 * ldc];
    double c33 = beta * C[3 + 3 * ldc];

    for (int l = 0; l < k; ++l) {
        double a0 = A[0 + l * lda];
        double a1 = A[1 + l * lda];
        double a2 = A[2 + l * lda];
        double a3 = A[3 + l * lda];

        double b0 = B[l + 0 * ldb];
        double b1 = B[l + 1 * ldb];
        double b2 = B[l + 2 * ldb];
        double b3 = B[l + 3 * ldb];

        /* accumulate alpha*A*B directly */
        c00 += alpha * a0 * b0;   c01 += alpha * a0 * b1;
        c02 += alpha * a0 * b2;   c03 += alpha * a0 * b3;

        c10 += alpha * a1 * b0;   c11 += alpha * a1 * b1;
        c12 += alpha * a1 * b2;   c13 += alpha * a1 * b3;

        c20 += alpha * a2 * b0;   c21 += alpha * a2 * b1;
        c22 += alpha * a2 * b2;   c23 += alpha * a2 * b3;

        c30 += alpha * a3 * b0;   c31 += alpha * a3 * b1;
        c32 += alpha * a3 * b2;   c33 += alpha * a3 * b3;
    }

    /* store results */
    C[0 + 0 * ldc] = c00;   C[0 + 1 * ldc] = c01;
    C[0 + 2 * ldc] = c02;   C[0 + 3 * ldc] = c03;

    C[1 + 0 * ldc] = c10;   C[1 + 1 * ldc] = c11;
    C[1 + 2 * ldc] = c12;   C[1 + 3 * ldc] = c13;

    C[2 + 0 * ldc] = c20;   C[2 + 1 * ldc] = c21;
    C[2 + 2 * ldc] = c22;   C[2 + 3 * ldc] = c23;

    C[3 + 0 * ldc] = c30;   C[3 + 1 * ldc] = c31;
    C[3 + 2 * ldc] = c32;   C[3 + 3 * ldc] = c33;
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
    for (int j = 0; j < n; j += 4)
        for (int i = 0; i < m; i += 4)
            noavx_micro_kernel_4x4(k,
                                   &A[i],        lda,
                                   &B[j * ldb],  ldb,
                                   &C[i + j * ldc], ldc,
                                   alpha, beta);
}

// Naive DGEMM implementation for verification
void dgemm_naive(int m, int n, int k, double alpha, const double *A, int lda,
                 const double *B, int ldb, double beta, double *C, int ldc) {
    // beta * C
    if (beta != 1.0) {
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < m; i++) {
                C[i + j * ldc] = beta * C[i + j * ldc];
            }
        }
    }
    
    // alpha * A * B + C
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            double sum = 0.0;
            for (int l = 0; l < k; l++) {
                sum += A[i + l * lda] * B[l + j * ldb];
            }
            C[i + j * ldc] += alpha * sum;
        }
    }
}

// Verify results by comparing with naive implementation
bool verify_results(int m, int n, int k, double alpha, const double *A, int lda,
                   const double *B, int ldb, double beta, double *C, int ldc) {
    // Create copies of C for both implementations
    std::vector<double> C_naive(m * n);
    std::vector<double> C_opt(m * n);
    std::copy(C, C + m * n, C_naive.data());
    std::copy(C, C + m * n, C_opt.data());
    
    // Run both implementations
    dgemm_naive(m, n, k, alpha, A, lda, B, ldb, beta, C_naive.data(), ldc);
    dgemm_noavx4x4_kernel_nn(m, n, k, alpha, A, lda, B, ldb, beta, C_opt.data(), ldc);
    
    // Compare results
    const double epsilon = 1e-10;  // Tolerance for floating-point comparison
    bool match = true;
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            double diff = std::abs(C_naive[i + j * ldc] - C_opt[i + j * ldc]);
            if (diff > epsilon) {
                std::cout << "Mismatch at (" << i << ", " << j << "): " 
                          << C_naive[i + j * ldc] << " vs " << C_opt[i + j * ldc] 
                          << " (diff: " << diff << ")" << std::endl;
                match = false;
                // Continue checking but remember that we found a mismatch
            }
        }
    }
    
    return match;
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

int main(int argc, char *argv[]) {
    // コマンドライン引数の解析
    bool perform_checks = true;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--nocheck") {
            perform_checks = false;
        }
    }

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
    csv_file << "m,n,k,GFLOPS1,GFLOPS2,GFLOPS3,GFLOPS4,GFLOPS5,Verified" << std::endl;
    
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

        // 正確性の検証 (--nocheckが指定されていない場合のみ)
        bool verified = true; // 検証をスキップする場合はtrueとする
        if (perform_checks) {
            verified = verify_results(m, n, k, alpha, A.data(), m, B.data(), k, beta, C.data(), m);
            if (!verified) {
                std::cout << " FAILED" << std::endl;
		exit(-1);
            }
        }

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
        
        // 検証結果を追加
        if (perform_checks) {
            csv_file << "," << (verified ? "True" : "False");
        }
        
        // 行の終了
        csv_file << std::endl;
    }

    csv_file.close();
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_noavx4x4kernel_results.csv" << std::endl;

    return 0;
}
