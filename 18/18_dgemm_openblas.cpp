#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <set>
#include <cblas.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define CACHELINE 64
#if defined(__GNUC__) || defined(__clang__)
    #define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
    #define ALIGN(x) __declspec(align(x))
#else
    #define ALIGN(x)
#endif

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
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,m, n, k, alpha, A, lda, B, ldb, beta, C_opt.data(), ldc);
    
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

// Generate random matrix
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

// Benchmark function
template <typename Func>
double benchmark(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main(int argc, char *argv[]) {
#ifdef OPENBLAS_THREADPOOL
    openblas_set_num_threads( omp_get_max_threads() );
#endif

    // Parse command line arguments
    bool perform_checks = true;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--nocheck") {
            perform_checks = false;
        }
    }

    // Prepare output CSV file
    std::ofstream csv_file("dgemm_benchmark_openblas_results.csv");
    
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

    // Output CSV header
    csv_file << "m,n,k,GFLOPS1,GFLOPS2,GFLOPS3,GFLOPS4,GFLOPS5,Verified" << std::endl;
    
    std::set<int> size_set;

    // Sizes that are multiples of 12
    for (int size = 12; size <= 1000; size += 12) {
        size_set.insert(size);
    }

    // Sizes that are multiples of 192
    for (int size = 1000; size <= 16000; size += 192) {
        size_set.insert(size);
    }

    // Add specific sizes of interest (cache boundaries, power of 2, etc.)
    std::vector<int> special_sizes = {
        512, 1024, 2048, 3072, 3456  // Powers of 2 and potential cache boundaries
    };

    // Convert set to vector (set is already sorted)
    std::vector<int> sizes(size_set.begin(), size_set.end());

    for (auto& size : sizes) {
        if (size % 12 != 0) {
            size = ((size + 11) / 12) * 12;  // Fix it by rounding up
        }
    }

    const int num_trials = 5;  // 5 trials

    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (auto size : sizes) {
        // Show progress
        std::cout << "Benchmarking size " << size << "..." << std::endl;
        
        // Square matrices where m = n = k = size
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

        // Verify correctness (unless --nocheck is specified)
        bool verified = true; // Default to true if skipping verification
        if (perform_checks) {
            verified = verify_results(m, n, k, alpha, A.data(), m, B.data(), k, beta, C.data(), m);
            if (!verified) {
                std::cout << " FAILED" << std::endl;
                exit(-1);
            }
        }

        // Start CSV row: m,n,k
        csv_file << m << "," << n << "," << k;

        // Benchmark the MR x NR kernel implementation
        for (int trial = 0; trial < num_trials; ++trial) {
            std::vector<double> C_test = C;
            double elapsed = benchmark([&]() {
                cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m);
            });
            double flops = flop_count / elapsed / 1.0e9; // GFLOPS
            
            // Add GFLOPS value to CSV
            csv_file << "," << flops;
        }
        
        // Add verification result
        if (perform_checks) {
            csv_file << "," << (verified ? "True" : "False");
        }
        
        // End row
        csv_file << std::endl;
    }

    csv_file.close();
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_openblas_results.csv" << std::endl;

    return 0;
}
