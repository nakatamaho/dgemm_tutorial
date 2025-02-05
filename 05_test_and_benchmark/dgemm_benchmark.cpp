#include <chrono>
#include <cmath>
#include <iostream>
#include <mpblas_dd.h>
#include <random>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

void Rgemm_NN_omp(mplapackint m, mplapackint n, mplapackint k, dd_real alpha, dd_real *A, mplapackint lda, dd_real *B, mplapackint ldb, dd_real beta, dd_real *C, mplapackint ldc);
void Rgemm_ref(const char *transa, const char *transb, mplapackint m, mplapackint n, mplapackint k, dd_real alpha, dd_real *A, mplapackint lda, dd_real *B, mplapackint ldb, dd_real beta, dd_real *C, mplapackint ldc);

void generate_random_matrix(mplapackint rows, mplapackint cols, dd_real *matrix) {
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (mplapackint j = 0; j < cols; ++j) {
        for (mplapackint i = 0; i < rows; ++i) {
            double random_value1 = dist(mt);
            double random_value2 = dist(mt);
            matrix[i + j * rows] = dd_real(random_value1) + dd_real(random_value2) * 1.0e-16;
        }
    }
}

template <typename Func> double benchmark(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

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

double compute_max_abs_diff(const dd_real *ref, const dd_real *test, mplapackint size) {
    double max_diff = 0.0;
    for (mplapackint i = 0; i < size; i++) {
        double diff = abs((ref[i] - test[i])).x[0];
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}

int main() {
#ifdef _OPENMP
    std::cout << "OpenMP is enabled.\n";
    std::cout << "Number of threads (max): " << omp_get_max_threads() << "\n";
#else
    std::cout << "OpenMP is not enabled.\n";
#endif
    std::vector<mplapackint> sizes = {256, 512};

#ifdef _OPENMP
    int num_cores = omp_get_num_procs();
#else
    int num_cores = 1;
#endif
    std::vector<int> thread_counts = {num_cores, std::max(1, num_cores / 2)};

    const int num_trials = 10;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (auto m : sizes) {
        for (auto n : sizes) {
            for (auto k : sizes) {
                double flop_count = static_cast<double>(m) * n * (2.0 * k + 1);

                std::vector<dd_real> A(m * k);
                std::vector<dd_real> B(k * n);
                std::vector<dd_real> C(m * n);
                std::vector<dd_real> C_ref(m * n);

                dd_real alpha = dd_real(dist(mt)) + dd_real(dist(mt)) * 1.0e-16;
                dd_real beta = dd_real(dist(mt)) + dd_real(dist(mt)) * 1.0e-16;

                generate_random_matrix(m, k, A.data());
                generate_random_matrix(k, n, B.data());
                generate_random_matrix(m, n, C.data());

                C_ref = C;
                Rgemm_ref("n", "n", m, n, k, alpha, A.data(), m, B.data(), k, beta, C_ref.data(), m);

                std::cout << "Benchmarking m=" << m << ", n=" << n << ", k=" << k << ":\n";

                for (auto threads : thread_counts) {
#ifdef _OPENMP
                    omp_set_num_threads(threads);
#endif
                    std::vector<double> flops_results;
                    std::vector<double> diff_results;

                    for (int trial = 0; trial < num_trials; ++trial) {
                        std::vector<dd_real> C_test = C;

                        double elapsed = benchmark([&]() { Rgemm_NN_omp(m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m); });

                        double flops = flop_count / elapsed / 1.0e6;
                        flops_results.push_back(flops);

                        double max_diff = compute_max_abs_diff(C_ref.data(), C_test.data(), m * n);
                        diff_results.push_back(max_diff);
                    }
                    auto [mean_flops, var_flops] = calculate_mean_and_variance(flops_results);
                    auto [mean_diff, var_diff] = calculate_mean_and_variance(diff_results);

                    std::cout << "Threads: " << threads << "\n";
                    std::cout << "  FLOPS for each trial [MFLOPS]: ";
                    for (const auto &val : flops_results) {
                        std::cout << val << " ";
                    }
                    std::cout << "\n";
                    std::cout << "  Mean FLOPS: " << mean_flops << " MFLOPS, Variance: " << var_flops << "\n";

                    std::cout << "  Max Abs Diff for each trial:   ";
                    for (const auto &dval : diff_results) {
                        std::cout << dval << " ";
                    }
                    std::cout << "\n";
                    std::cout << "  Mean of Max Diff: " << mean_diff << ", Variance of Max Diff: " << var_diff << "\n";
                }
                std::cout << "---------------------------------\n";
            }
        }
    }

    return 0;
}
