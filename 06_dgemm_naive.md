#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

// 最も単純なDGEMM実装（NN版のみ）
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
    std::vector<int> sizes = {256, 512, 1024};

    const int num_trials = 5;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (auto m : sizes) {
        for (auto n : sizes) {
            for (auto k : sizes) {
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

                // シンプル実装のベンチマーク
                std::vector<double> simple_flops_results;
                for (int trial = 0; trial < num_trials; ++trial) {
                    std::vector<double> C_test = C;
                    double elapsed = benchmark([&]() {
                        dgemm_simple_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m);
                    });
                    double flops = flop_count / elapsed / 1.0e9; // GFLOPS
                    simple_flops_results.push_back(flops);
                }

                auto [mean_simple_flops, var_simple_flops] = calculate_mean_and_variance(simple_flops_results);

                std::cout << "Simple Implementation:\n";
                std::cout << "  FLOPS for each trial [GFLOPS]: ";
                for (const auto &val : simple_flops_results) {
                    std::cout << val << " ";
                }
                std::cout << "\n";
                std::cout << "  Mean FLOPS: " << mean_simple_flops << " GFLOPS, Variance: " << var_simple_flops << "\n";
                std::cout << "------------------------------------------------\n";
            }
        }
    }

    return 0;
}
