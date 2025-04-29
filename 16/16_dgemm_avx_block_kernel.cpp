#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <set>
#include <immintrin.h> // For AVX2 intrinsics

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
ALIGN(CACHELINE) static double Bpanel[KC * NC]; // B panel with transpose orientation
ALIGN(CACHELINE) static double C_temp[MC * NC];

// 4x4 micro kernel (using AVX2) - B transposed version
void avx2_micro_kernel_4x4_aligned(int k,
                                   const double * __restrict A, int lda,
                                   const double * __restrict B, int ldb,
                                   double       * __restrict C, int ldc) {
    // Initialize AVX registers to accumulate results
    __m256d c0 = _mm256_setzero_pd(); // For row 0 of C matrix (c00, c01, c02, c03)
    __m256d c1 = _mm256_setzero_pd(); // For row 1 of C matrix (c10, c11, c12, c13)
    __m256d c2 = _mm256_setzero_pd(); // For row 2 of C matrix (c20, c21, c22, c23)
    __m256d c3 = _mm256_setzero_pd(); // For row 3 of C matrix (c30, c31, c32, c33)
    
    // Compute matrix multiplication along k dimension
    for (int l = 0; l < k; l++) {
        // Load elements from A and broadcast to all elements
        __m256d a0 = _mm256_set1_pd(A[0 + l * lda]); // Broadcast a0 to all elements
        __m256d a1 = _mm256_set1_pd(A[1 + l * lda]); // Broadcast a1 to all elements
        __m256d a2 = _mm256_set1_pd(A[2 + l * lda]); // Broadcast a2 to all elements
        __m256d a3 = _mm256_set1_pd(A[3 + l * lda]); // Broadcast a3 to all elements
        
        // Load 4 elements from B at once (transposed access pattern) with aligned load
        __m256d b = _mm256_load_pd(&B[0 + l * ldb]);
        
        // Multiply and accumulate using FMA instructions
        c0 = _mm256_fmadd_pd(a0, b, c0); // c0 += a0 * b
        c1 = _mm256_fmadd_pd(a1, b, c1); // c1 += a1 * b
        c2 = _mm256_fmadd_pd(a2, b, c2); // c2 += a2 * b
        c3 = _mm256_fmadd_pd(a3, b, c3); // c3 += a3 * b
    }
    
    // Create temporary arrays for results (aligned to 64 bytes)
    alignas(64) double c0_arr[4], c1_arr[4], c2_arr[4], c3_arr[4];
    
    // Store results from AVX registers to temporary arrays with aligned store
    _mm256_store_pd(c0_arr, c0);
    _mm256_store_pd(c1_arr, c1);
    _mm256_store_pd(c2_arr, c2);
    _mm256_store_pd(c3_arr, c3);
    
    // Store results to C matrix
    // Row 0
    C[0 + 0 * ldc] = c0_arr[0]; C[0 + 1 * ldc] = c0_arr[1]; 
    C[0 + 2 * ldc] = c0_arr[2]; C[0 + 3 * ldc] = c0_arr[3];
    
    // Row 1
    C[1 + 0 * ldc] = c1_arr[0]; C[1 + 1 * ldc] = c1_arr[1]; 
    C[1 + 2 * ldc] = c1_arr[2]; C[1 + 3 * ldc] = c1_arr[3];
    
    // Row 2
    C[2 + 0 * ldc] = c2_arr[0]; C[2 + 1 * ldc] = c2_arr[1]; 
    C[2 + 2 * ldc] = c2_arr[2]; C[2 + 3 * ldc] = c2_arr[3];
    
    // Row 3
    C[3 + 0 * ldc] = c3_arr[0]; C[3 + 1 * ldc] = c3_arr[1]; 
    C[3 + 2 * ldc] = c3_arr[2]; C[3 + 3 * ldc] = c3_arr[3];
}


// DGEMM implementation using 4x4 micro kernel with transposed B panel, using L3 cache blocking
void dgemm_avx_kernel_nn(int m, int n, int k, double alpha, 
                          const double * __restrict A, int lda,
                          const double * __restrict B, int ldb, 
			  double beta, double * __restrict C, int ldc) {
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

    // Process by blocks (MC x NC blocks)
    for (int jc = 0; jc < n; jc += NC) {
        int jb = std::min(n - jc, NC);
        
        // Copy B panel and apply alpha at this level
        for (int pc = 0; pc < k; pc += KC) {
            int pb = std::min(k - pc, KC);
            
            // Create B panel (k×NC) with transposition for better cache utilization
            for (int jp = 0; jp < jb; jp++) {
                for (int l = 0; l < pb; l++) {
                    Bpanel[jp + l * jb] = alpha * B[(pc + l) + (jc + jp) * ldb];
                }
            }
            const double beta_block = (pc == 0) ? beta : 1.0;

            // Process A panel (MC×k)
            for (int ic = 0; ic < m; ic += MC) {
                int ib = std::min(m - ic, MC);

                // Create A panel
                for (int l = 0; l < pb; l++) {
                    for (int ip = 0; ip < ib; ip++) {
                        Apanel[ip + l * ib] = A[(ic + ip) + (pc + l) * lda];
                    }
                }

                // Process by panels (MR x NR panels)
                for (int j = 0; j < jb; j += NR) {
                    int nb = std::min(jb - j, NR);
                    
                    for (int i = 0; i < ib; i += MR) {
                        int mb = std::min(ib - i, MR);
                        
                        // Initialize temporary buffer to zero
                        for (int jj = 0; jj < nb; jj++) {
                            for (int ii = 0; ii < mb; ii++) {
                                C_temp[ii + jj * MR] = 0.0;
                            }
                        }
                        
                        // Call micro kernel with transposed B panel
                        avx2_micro_kernel_4x4_aligned(pb, &Apanel[i + 0 * ib], ib, &Bpanel[j], jb, C_temp, MR);
                        
                        // Add results to C (apply beta)
                        for (int jj = 0; jj < nb; jj++) {
                            for (int ii = 0; ii < mb; ii++) {
                                C[(ic + i + ii) + (jc + j + jj) * ldc] = (beta_block == 0.0) ? C_temp[ii + jj * MR] : beta_block * C[(ic + i + ii) + (jc + j + jj) * ldc] + C_temp[ii + jj * MR];

                            }
                        }
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
    dgemm_avx_kernel_nn(m, n, k, alpha, A, lda, B, ldb, beta, C_opt.data(), ldc);
    
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
    std::ofstream csv_file("dgemm_benchmark_avx_block_kernel_results.csv");
    
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
                dgemm_avx_kernel_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, C_test.data(), m);
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
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_avx_block_kernel_results.csv" << std::endl;

    return 0;
}
