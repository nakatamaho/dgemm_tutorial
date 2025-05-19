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
#define NR 12
#define MC 96
#define NC 2304
#define KC 480

#define CACHELINE 64
#if defined(__GNUC__) || defined(__clang__)
    #define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
    #define ALIGN(x) __declspec(align(x))
#else
    #define ALIGN(x)
#endif

ALIGN(CACHELINE) static double Apanel[MC * KC] __attribute__((aligned(4096)));
ALIGN(CACHELINE) static double Bpanel[KC * NC] __attribute__((aligned(4096))); // B panel with transpose orientation

// --------------------------------------------------------------------------------
// 4×12 AVX2 micro-kernel with zero initialization (for first KC block)
// --------------------------------------------------------------------------------
inline void avx2_micro_kernel_4x12_zero_init(int k,
                                           const double* __restrict A, int lda,
                                           const double* __restrict B, int ldb,
                                           double*       __restrict C, int ldc)
{
    // 1) Initialize accumulators to zero
    __m256d c0  = _mm256_setzero_pd();
    __m256d c1  = _mm256_setzero_pd();
    __m256d c2  = _mm256_setzero_pd();
    __m256d c3  = _mm256_setzero_pd();
    __m256d c4  = _mm256_setzero_pd();
    __m256d c5  = _mm256_setzero_pd();
    __m256d c6  = _mm256_setzero_pd();
    __m256d c7  = _mm256_setzero_pd();
    __m256d c8  = _mm256_setzero_pd();
    __m256d c9  = _mm256_setzero_pd();
    __m256d c10 = _mm256_setzero_pd();
    __m256d c11 = _mm256_setzero_pd();
    
    // 2) Main inner-product loop
    for (int l = 0; l < k; ++l) {
        __m256d a  = _mm256_load_pd(&A[l * lda]);
        __m256d b0  = _mm256_set1_pd(B[l * ldb + 0]);
        __m256d b1  = _mm256_set1_pd(B[l * ldb + 1]);
        __m256d b2  = _mm256_set1_pd(B[l * ldb + 2]);
        __m256d b3  = _mm256_set1_pd(B[l * ldb + 3]);
        __m256d b4  = _mm256_set1_pd(B[l * ldb + 4]);
        __m256d b5  = _mm256_set1_pd(B[l * ldb + 5]);
        __m256d b6  = _mm256_set1_pd(B[l * ldb + 6]);
        __m256d b7  = _mm256_set1_pd(B[l * ldb + 7]);
        __m256d b8  = _mm256_set1_pd(B[l * ldb + 8]);
        __m256d b9  = _mm256_set1_pd(B[l * ldb + 9]);
        __m256d b10 = _mm256_set1_pd(B[l * ldb + 10]);
        __m256d b11 = _mm256_set1_pd(B[l * ldb + 11]);
        c0  = _mm256_fmadd_pd(a, b0,  c0);
        c1  = _mm256_fmadd_pd(a, b1,  c1);
        c2  = _mm256_fmadd_pd(a, b2,  c2);
        c3  = _mm256_fmadd_pd(a, b3,  c3);
        c4  = _mm256_fmadd_pd(a, b4,  c4);
        c5  = _mm256_fmadd_pd(a, b5,  c5);
        c6  = _mm256_fmadd_pd(a, b6,  c6);
        c7  = _mm256_fmadd_pd(a, b7,  c7);
        c8  = _mm256_fmadd_pd(a, b8,  c8);
        c9  = _mm256_fmadd_pd(a, b9,  c9);
        c10 = _mm256_fmadd_pd(a, b10, c10);
        c11 = _mm256_fmadd_pd(a, b11, c11);
    }
    
    // 3) Store result tile
    _mm256_storeu_pd(&C[0  + 0 * ldc],  c0);
    _mm256_storeu_pd(&C[0  + 1 * ldc],  c1);
    _mm256_storeu_pd(&C[0  + 2 * ldc],  c2);
    _mm256_storeu_pd(&C[0  + 3 * ldc],  c3);
    _mm256_storeu_pd(&C[0  + 4 * ldc],  c4);
    _mm256_storeu_pd(&C[0  + 5 * ldc],  c5);
    _mm256_storeu_pd(&C[0  + 6 * ldc],  c6);
    _mm256_storeu_pd(&C[0  + 7 * ldc],  c7);
    _mm256_storeu_pd(&C[0  + 8 * ldc],  c8);
    _mm256_storeu_pd(&C[0  + 9 * ldc],  c9);
    _mm256_storeu_pd(&C[0  + 10 * ldc], c10);
    _mm256_storeu_pd(&C[0  + 11 * ldc], c11);
}

// --------------------------------------------------------------------------------
// 4×12 AVX2 micro-kernel with accumulation (for subsequent KC blocks)
// --------------------------------------------------------------------------------
inline void avx2_micro_kernel_4x12_load_accum(int k,
                                           const double* __restrict A, int lda,
                                           const double* __restrict B, int ldb,
                                           double*       __restrict C, int ldc)
{
    // 1) Load current C micro-tile into vector accumulators
    __m256d c0  = _mm256_loadu_pd(&C[0  + 0 * ldc]);
    __m256d c1  = _mm256_loadu_pd(&C[0  + 1 * ldc]);
    __m256d c2  = _mm256_loadu_pd(&C[0  + 2 * ldc]);
    __m256d c3  = _mm256_loadu_pd(&C[0  + 3 * ldc]);
    __m256d c4  = _mm256_loadu_pd(&C[0  + 4 * ldc]);
    __m256d c5  = _mm256_loadu_pd(&C[0  + 5 * ldc]);
    __m256d c6  = _mm256_loadu_pd(&C[0  + 6 * ldc]);
    __m256d c7  = _mm256_loadu_pd(&C[0  + 7 * ldc]);
    __m256d c8  = _mm256_loadu_pd(&C[0  + 8 * ldc]);
    __m256d c9  = _mm256_loadu_pd(&C[0  + 9 * ldc]);
    __m256d c10 = _mm256_loadu_pd(&C[0  + 10 * ldc]);
    __m256d c11 = _mm256_loadu_pd(&C[0  + 11 * ldc]);
    
    // 2) Main inner-product loop
    for (int l = 0; l < k; ++l) {
        __m256d a  = _mm256_load_pd(&A[l * lda]);
        __m256d b0  = _mm256_set1_pd(B[l * ldb + 0]);
        __m256d b1  = _mm256_set1_pd(B[l * ldb + 1]);
        __m256d b2  = _mm256_set1_pd(B[l * ldb + 2]);
        __m256d b3  = _mm256_set1_pd(B[l * ldb + 3]);
        __m256d b4  = _mm256_set1_pd(B[l * ldb + 4]);
        __m256d b5  = _mm256_set1_pd(B[l * ldb + 5]);
        __m256d b6  = _mm256_set1_pd(B[l * ldb + 6]);
        __m256d b7  = _mm256_set1_pd(B[l * ldb + 7]);
        __m256d b8  = _mm256_set1_pd(B[l * ldb + 8]);
        __m256d b9  = _mm256_set1_pd(B[l * ldb + 9]);
        __m256d b10 = _mm256_set1_pd(B[l * ldb + 10]);
        __m256d b11 = _mm256_set1_pd(B[l * ldb + 11]);
        c0  = _mm256_fmadd_pd(a, b0,  c0);
        c1  = _mm256_fmadd_pd(a, b1,  c1);
        c2  = _mm256_fmadd_pd(a, b2,  c2);
        c3  = _mm256_fmadd_pd(a, b3,  c3);
        c4  = _mm256_fmadd_pd(a, b4,  c4);
        c5  = _mm256_fmadd_pd(a, b5,  c5);
        c6  = _mm256_fmadd_pd(a, b6,  c6);
        c7  = _mm256_fmadd_pd(a, b7,  c7);
        c8  = _mm256_fmadd_pd(a, b8,  c8);
        c9  = _mm256_fmadd_pd(a, b9,  c9);
        c10 = _mm256_fmadd_pd(a, b10, c10);
        c11 = _mm256_fmadd_pd(a, b11, c11);
    }
    
    // 3) Store updated tile
    _mm256_storeu_pd(&C[0  + 0 * ldc],  c0);
    _mm256_storeu_pd(&C[0  + 1 * ldc],  c1);
    _mm256_storeu_pd(&C[0  + 2 * ldc],  c2);
    _mm256_storeu_pd(&C[0  + 3 * ldc],  c3);
    _mm256_storeu_pd(&C[0  + 4 * ldc],  c4);
    _mm256_storeu_pd(&C[0  + 5 * ldc],  c5);
    _mm256_storeu_pd(&C[0  + 6 * ldc],  c6);
    _mm256_storeu_pd(&C[0  + 7 * ldc],  c7);
    _mm256_storeu_pd(&C[0  + 8 * ldc],  c8);
    _mm256_storeu_pd(&C[0  + 9 * ldc],  c9);
    _mm256_storeu_pd(&C[0  + 10 * ldc], c10);
    _mm256_storeu_pd(&C[0  + 11 * ldc], c11);
}

// Optimized packing functions - force inlining to reduce call overhead
inline void pack_blockB(double alpha, const double * __restrict B, int ldb,
                int p, int j, int nc, int kc, double * __restrict Bpanel) {
    for (int jr = 0; jr < nc; jr += NR) {
        int nr = std::min(NR, nc - jr);
        // Avoid intermediate pointers and const_cast
        for (int l = 0; l < kc; ++l) {
            for (int jj = 0; jj < nr; ++jj)
                Bpanel[jr * kc + l * NR + jj]
                    = alpha * B[(p + l) + (j + jr + jj) * ldb];
            for (int jj = nr; jj < NR; ++jj)
                Bpanel[jr * kc + l * NR + jj] = 0.0;
        }
    }
}

// Optimized packing function for A - force inlining
inline void pack_blockA(const double * __restrict A, int lda,
                int i, int p, int mc, int kc, double * __restrict Apanel) {
    for (int ir = 0; ir < mc; ir += MR) {
        // Eliminate nested function call
        for (int l = 0; l < kc; ++l) {
            for (int ii = 0; ii < MR; ++ii) {
                Apanel[ir * kc + l * MR + ii]
                    = A[(i + ir + ii) + (p + l) * lda];
            }
        }
    }
}

// DGEMM implementation using 4x12 micro kernel with transposed B panel, using L3 cache blocking
void dgemm_avx_kernel_nn(int m, int n, int k, double alpha, 
                          const double * __restrict A, int lda,
                          const double * __restrict B, int ldb, 
			  double beta, double * __restrict C, int ldc)
{
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) return;
    if (m % MR || n % NR || k % 12) {
        std::cerr << "Error: matrix dims must be multiples of 12." << std::endl;
        return;
    }

    // Process matrix in tiles
    for (int j = 0; j < n; j += NC) {
        int nc = std::min(NC, n - j);
        int kc = std::min(KC, k);
        
        // Pack first KC block of B with alpha scaling
        pack_blockB(alpha, B, ldb, 0, j, nc, kc, Bpanel);
        
        for (int i = 0; i < m; i += MC) {
            int mc = std::min(MC, m - i);
            
            // Pack first KC block of A
            pack_blockA(A, lda, i, 0, mc, kc, Apanel);
            
            // Apply beta scaling (only for the first k block)
            if (beta == 0.0) {
                // zero out the entire block
                for (int jr = 0; jr < nc; ++jr) {
                    double *Ccol = &C[(j + jr) * ldc + i]; // pointer to the start of column
                    for (int ir = 0; ir < mc; ++ir)
                        Ccol[ir] = 0.0;
                }
            }
            else if (beta != 1.0) {
                // scale existing block by beta
                for (int jr = 0; jr < nc; ++jr) {
                    double *Ccol = &C[(j + jr) * ldc + i]; // pointer to the start of column
                    for (int ir = 0; ir < mc; ++ir)
                        Ccol[ir] *= beta;
                }
            }
            
            // Micro-kernel computation for the first KC block
            for (int jr = 0; jr < nc; jr += NR) {
                int nr = std::min(NR, nc - jr);
                for (int ir = 0; ir < mc; ir += MR) {
                    int mr = std::min(MR, mc - ir);
                    avx2_micro_kernel_4x12_load_accum(kc,
                                                   &Apanel[ir * kc], MR,
                                                   &Bpanel[jr * kc], NR,
                                                   &C[(j + jr) * ldc + (i + ir)], ldc);

/*
		    if (beta == 0.0) {
                        // For beta=0, use zero-init kernel (C = alpha*A*B)
                        avx2_micro_kernel_4x12_zero_init(kc,
                                                      &Apanel[ir * kc], MR,
                                                      &Bpanel[jr * kc], NR,
                                                      &C[(j + jr) * ldc + (i + ir)], ldc);
                    } else {
                        // For beta!=0, use accumulation kernel (C = alpha*A*B + beta*C)
                        avx2_micro_kernel_4x12_load_accum(kc,
                                                       &Apanel[ir * kc], MR,
                                                       &Bpanel[jr * kc], NR,
                                                       &C[(j + jr) * ldc + (i + ir)], ldc);
                    }
*/
                }
            }
        }
        
        // Process remaining KC blocks
        for (int p = kc; p < k; p += KC) {
            int kc_next = std::min(KC, k - p);
            
            // Pack next KC block of B
            pack_blockB(alpha, B, ldb, p, j, nc, kc_next, Bpanel);
            
            for (int i = 0; i < m; i += MC) {
                int mc = std::min(MC, m - i);
                
                // Pack next KC block of A
                pack_blockA(A, lda, i, p, mc, kc_next, Apanel);
                
                // Micro-kernel computation for subsequent KC blocks
                for (int jr = 0; jr < nc; jr += NR) {
                    int nr = std::min(NR, nc - jr);
                    for (int ir = 0; ir < mc; ir += MR) {
                        int mr = std::min(MR, mc - ir);
                        // Use accumulation micro-kernel
                        avx2_micro_kernel_4x12_load_accum(kc_next,
                                                      &Apanel[ir * kc_next], MR,
                                                      &Bpanel[jr * kc_next], NR,
                                                      &C[(j + jr) * ldc + (i + ir)], ldc);
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
    std::ofstream csv_file("dgemm_benchmark_4x12avx2_kernel_results.4.csv");
    
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

    // Sizes that are multiples of 96
    for (int size = 1000; size <= 4000; size += 96) {
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
    std::cout << "Benchmark complete. Results saved to dgemm_benchmark_4x12avx2_kernel_results.4.csv" << std::endl;

    return 0;
}
