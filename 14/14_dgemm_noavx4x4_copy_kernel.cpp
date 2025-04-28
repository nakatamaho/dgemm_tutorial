#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <set>

#ifdef _OPENMP
#include <omp.h>
#endif

// Define block sizes
#define MR 4
#define NR 4
#define MC 256
#define NC 256
#define KC 256

#define CACHELINE 64
#if defined(__GNUC__) || defined(__clang__)
    #define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
    #define ALIGN(x) __declspec(align(x))
#else
    #define ALIGN(x)
#endif

ALIGN(CACHELINE) static double Apanel[MC * KC];
ALIGN(CACHELINE) static double Bpanel[KC * NC];
ALIGN(CACHELINE) static double C_temp[MC * NC];

// 4x4 micro kernel (no AVX)
void noavx_micro_kernel(int k, const double *A, int lda,
                         const double *B, int ldb, double *C, int ldc) {
    // Accumulate results in temporary variables
    double c00 = 0.0, c01 = 0.0, c02 = 0.0, c03 = 0.0;
    double c10 = 0.0, c11 = 0.0, c12 = 0.0, c13 = 0.0;
    double c20 = 0.0, c21 = 0.0, c22 = 0.0, c23 = 0.0;
    double c30 = 0.0, c31 = 0.0, c32 = 0.0, c33 = 0.0;
    
    // Compute matrix multiplication along k dimension
    for (int l = 0; l < k; l++) {
        // Load elements from A
        double a0 = A[0 + l * lda];
        double a1 = A[1 + l * lda];
        double a2 = A[2 + l * lda];
        double a3 = A[3 + l * lda];
        
        // Load elements from B
        double b0 = B[l + 0 * ldb];
        double b1 = B[l + 1 * ldb];
        double b2 = B[l + 2 * ldb];
        double b3 = B[l + 3 * ldb];
        
        // Compute matrix multiplication
        c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
        c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
        c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
        c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
    }
    
    // Store results to C
    C[0 + 0 * ldc] = c00; C[0 + 1 * ldc] = c01; C[0 + 2 * ldc] = c02; C[0 + 3 * ldc] = c03;
    C[1 + 0 * ldc] = c10; C[1 + 1 * ldc] = c11; C[1 + 2 * ldc] = c12; C[1 + 3 * ldc] = c13;
    C[2 + 0 * ldc] = c20; C[2 + 1 * ldc] = c21; C[2 + 2 * ldc] = c22; C[2 + 3 * ldc] = c23;
    C[3 + 0 * ldc] = c30; C[3 + 1 * ldc] = c31; C[3 + 2 * ldc] = c32; C[3 + 3 * ldc] = c33;
}

// DGEMM implementation using 4x4 micro kernel (NN version) - for multiples of MR/NR only, with buffer copying
void dgemm_noavx_kernel_nn(int m, int n, int k, double alpha, const double *A, int lda,
                            const double *B, int ldb, double beta, double *C, int ldc) {
    // Handle simple cases
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) {
        return;  // Nothing to do
    }
    
    // Check for size multiples of MR/NR
    if (m % MR != 0 || n % NR != 0 || k % 4 != 0) {
        std::cerr << "Error: Matrix dimensions must be multiples of MR/NR." << std::endl;
        return;
    }
    
    // Handle alpha == 0 case
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
    
    // Process by panels (MR x NR panels)
    for (int j = 0; j < n; j += NR) {
        for (int i = 0; i < m; i += MR) {
            // Initialize temporary buffer to zero
            for (int idx = 0; idx < MR * NR; idx++) {
                C_temp[idx] = 0.0;
            }
            
            // Copy A - MR rows x k columns panel
            for (int l = 0; l < k; l++) {
                for (int ii = 0; ii < MR; ii++) {
                    Apanel[ii + l * MR] = A[(i + ii) + l * lda];
                }
            }
            
            // Copy B and multiply by alpha - k rows x NR columns panel
            for (int jj = 0; jj < NR; jj++) {
                for (int l = 0; l < k; l++) {
                    Bpanel[l + jj * k] = alpha * B[l + (j + jj) * ldb];
                }
            }
            
            // Call micro kernel - using Apanel, Bpanel
            noavx_micro_kernel(k, Apanel, MR, Bpanel, k, C_temp, MR);
            
            // Add results to C (apply beta)
            for (int jj = 0; jj < NR; jj++) {
                for (int ii = 0; ii < MR; ii++) {
                    if (beta == 0.0) {
                        // beta = 0 case
                        C[(i + ii) + (j + jj) * ldc] = C_temp[ii + jj * MR];
                    } else {
                        // beta * C + C_temp
                        C[(i + ii) + (j + jj) * ldc] = beta * C[(i + ii) + (j + jj) * ldc] + C_temp[ii + jj * MR];
                    }
                }
            }
        }
    }
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
    dgemm_noavx_kernel_nn(m, n, k, alpha, A, lda, B, ldb, beta, C_opt.data(), ldc);
    
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
    // Parse command line arguments
    bool perform_checks = true;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--nocheck") {
            perform_checks = false;
        }
    }

    // Prepare output CSV file
    std::ofstream csv_file("dgemm_benchmark_noavx_copy_kernel_results.csv");
    
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

    // Sizes that are multiples of 4 (4 to 124)
    for (int size = 4; size <= 124; size += 4) {
        size_set.insert(size);
    }

    // Sizes that are multiples of 8 (128 to 1024)
    for (int size = 128; size <= 3500; size += 8) {
        size_set.insert(size);
    }

    // Convert set to vector (set is already sorted)
    std::vector<int> sizes(size_set.begin(), size_set.end());

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
                dgemm_noavx_kernel_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m);
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
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_noavx_copy_kernel_results.csv" << std::endl;

    return 0;
}
