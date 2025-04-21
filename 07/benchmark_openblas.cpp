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

    // OpenBLASのスレッド数設定(必要に応じて)
    // openblas_set_num_threads(4);
    
    // 1から1000まで7ずつのサイズを生成(正方行列用)
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
