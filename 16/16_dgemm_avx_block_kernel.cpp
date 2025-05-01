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
#define MC 240
#define NC 4096
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

// 4x4 micro kernel (using AVX2) - B transposed version
void avx2_micro_kernel_4x4_aligned(int k,
                                   const double * __restrict A, int lda,
                                   const double * __restrict B, int ldb,
                                   double       * __restrict C, int ldc) {
    // 1) Load the existing C-block into accumulators
    __m256d c0 = _mm256_loadu_pd(&C[0 + 0*ldc]);
    __m256d c1 = _mm256_loadu_pd(&C[0 + 1*ldc]);
    __m256d c2 = _mm256_loadu_pd(&C[0 + 2*ldc]);
    __m256d c3 = _mm256_loadu_pd(&C[0 + 3*ldc]);

    // 2) Main loop over the shared dimension k
    for (int l = 0; l < k; ++l) {
        // Load one column of A (4 elements)
        __m256d a = _mm256_load_pd(&A[l * lda]);
        // Broadcast each of the 4 B-elements from row l
        __m256d b0 = _mm256_set1_pd(B[l * ldb + 0]);
        __m256d b1 = _mm256_set1_pd(B[l * ldb + 1]);
        __m256d b2 = _mm256_set1_pd(B[l * ldb + 2]);
        __m256d b3 = _mm256_set1_pd(B[l * ldb + 3]);
        // FMA accumulation
        c0 = _mm256_fmadd_pd(a, b0, c0);
        c1 = _mm256_fmadd_pd(a, b1, c1);
        c2 = _mm256_fmadd_pd(a, b2, c2);
        c3 = _mm256_fmadd_pd(a, b3, c3);
    }

    // 3) Store the updated accumulators back to C
    _mm256_storeu_pd(&C[0 + 0*ldc], c0);
    _mm256_storeu_pd(&C[0 + 1*ldc], c1);
    _mm256_storeu_pd(&C[0 + 2*ldc], c2);
    _mm256_storeu_pd(&C[0 + 3*ldc], c3);
}

// DGEMM implementation using 4x4 micro kernel with transposed B panel, using L3 cache blocking
void dgemm_avx_kernel_nn(int m, int n, int k, double alpha, 
                          const double * __restrict A, int lda,
                          const double * __restrict B, int ldb, 
			  double beta, double * __restrict C, int ldc)
{
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0))
        return;
    if (m % MR != 0 || n % NR != 0 || k % 4 != 0) {
        std::cerr << "Error: matrix dims must be multiples of 4." << std::endl;
        return;
    }

    // Loop over N in blocks of NC
    for (int j = 0; j < n; j += NC) {
        int nc = std::min(NC, n - j);

        // Loop over K in blocks of KC
        for (int p = 0; p < k; p += KC) {
            int kc = std::min(KC, k - p);
            const bool first_k_block = (p == 0);

            // Pack kc×nc block of B and scale by alpha
            for (int jr = 0; jr < nc; jr += NR) {
                int nr = std::min(NR, nc - jr);
                for (int l = 0; l < kc; ++l) {
                    for (int jj = 0; jj < nr; ++jj)
                        Bpanel[jr * kc + l * NR + jj]
                            = alpha * B[(p + l) + (j + jr + jj) * ldb];
                    for (int jj = nr; jj < NR; ++jj)
                        Bpanel[jr * kc + l * NR + jj] = 0.0;
                }
            }

            // Loop over M in blocks of MC
            for (int i = 0; i < m; i += MC) {
                int mc = std::min(MC, m - i);

                // Pointer to the (i,j) corner of current C-panel
                double *Cpanel = &C[i + j * ldc];

                // --- initialize or scale C only on the first k-block ---
                if (first_k_block) {
                    if (beta == 0.0) {
                        // zero out entire Cpanel
                        for (int jj = 0; jj < nc; ++jj) {
                            double *Ccol = Cpanel + jj * ldc;
                            for (int ii = 0; ii < mc; ++ii)
                                Ccol[ii] = 0.0;
                        }
                    }
                    else if (beta != 1.0) {
                        // scale existing Cpanel by beta
                        for (int jj = 0; jj < nc; ++jj) {
                            double *Ccol = Cpanel + jj * ldc;
                            for (int ii = 0; ii < mc; ++ii)
                                Ccol[ii] *= beta;
                        }
                    }
                }

                // Pack mc×kc slice of A into Apanel
                for (int ir = 0; ir < mc; ir += MR) {
                    for (int l = 0; l < kc; ++l) {
                        for (int ii = 0; ii < MR; ++ii) {
                            Apanel[ir * kc + l * MR + ii]
                                = A[(i + ir + ii) + (p + l) * lda];
                        }
                    }
                }

                // Compute micro-kernel over the C blocks
                for (int jr = 0; jr < nc; jr += NR) {
                    // define Ccol_jr once per jr
                    double *Ccol_jr = Cpanel + jr * ldc;
                    for (int ir = 0; ir < mc; ir += MR) {
                        double *Cblk = Ccol_jr + ir;
                        avx2_micro_kernel_4x4_aligned(
                            kc,
                            &Apanel[ir * kc], MR,
                            &Bpanel[jr * kc], NR,
                            Cblk, ldc);
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
