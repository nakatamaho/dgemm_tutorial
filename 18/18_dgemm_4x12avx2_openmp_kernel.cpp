// 18_dgemm_4x12avx2_openmp_kernel.cpp – OpenMP‐enabled DGEMM (4×12 AVX2 micro‑kernel)
//
// Build example (GCC/Clang):
//   g++ -O3 -std=c++17 -march=native -mavx2 -mfma -fopenmp 18_dgemm_4x12avx2_openmp_kernel.cpp -o dgemm_omp
//
// Typical run‑time settings on a 32‑core Ryzen Threadripper 3970X:
//   export OMP_NUM_THREADS=32
//   export OMP_PROC_BIND=spread
//   export OMP_PLACES=cores
//
// The code keeps the packing & micro‑kernel structure you provided but exposes
// the inner‑most block computation (jr×ir) to OpenMP with a static schedule so
// that each core receives full MR×NR micro‑tiles.
//
// -----------------------------------------------------------------------------
// Standard headers
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <set>
#include <immintrin.h>   // AVX2 intrinsics

#ifdef _OPENMP
  #include <omp.h>
#endif

// --------------------------------------------------------------------------------
// Tunable blocking parameters (L3‑resident)
// --------------------------------------------------------------------------------
#define MR  4
#define NR 12
#define MC 256
#define NC 384
#define KC 128

// SIMD alignment helper
#define CACHELINE 64
#if defined(__GNUC__) || defined(__clang__)
  #define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
  #define ALIGN(x) __declspec(align(x))
#else
  #define ALIGN(x)
#endif

ALIGN(CACHELINE) static double Apanel[MC * KC];      // A‑panel (MR×KC blocks)
ALIGN(CACHELINE) static double Bpanel[KC * NC];      // B‑panel (KC×NR blocks, transposed)

// --------------------------------------------------------------------------------
// 4×12 AVX2 micro‑kernel (B tile stored in row‑major, i.e. transposed)
// --------------------------------------------------------------------------------
inline void avx2_micro_kernel_4x12_aligned(int k,
                                           const double* __restrict A, int lda,
                                           const double* __restrict B, int ldb,
                                           double*       __restrict C, int ldc)
{
    // 1) Load current C micro‑tile into vector accumulators
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

    // 2) Main inner‑product loop
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

// --------------------------------------------------------------------------------
// Cache‑blocked DGEMM (NN) using above micro‑kernel
// --------------------------------------------------------------------------------
void dgemm_avx_kernel_nn(int m, int n, int k, double alpha,
                         const double* __restrict A, int lda,
                         const double* __restrict B, int ldb,
                         double beta,
                         double* __restrict C, int ldc)
{
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) return;
    if (m % MR || n % NR || k % 12) {
        std::cerr << "Error: matrix dims must be multiples of 12." << std::endl;
        return;
    }

#ifdef _OPENMP
#pragma omp parallel
{
    // Thread-local packing buffers
    ALIGN(CACHELINE) double thread_Apanel[MC * KC];
    ALIGN(CACHELINE) double thread_Bpanel[KC * NC];
#pragma omp for schedule(static)
#endif
    for (int j = 0; j < n; j += NC) {
        int nc = std::min(NC, n - j);

        for (int p = 0; p < k; p += KC) {
            int kc = std::min(KC, k - p);
            bool first_k_block = (p == 0);

#ifdef _OPENMP
            // Use thread-local buffers
            double* Bpanel = thread_Bpanel;
#else
            // In single-threaded mode, use the global buffer
            double* Bpanel = Bpanel;
#endif

            // --- pack/alfa‑scale a KC×nc slice of B into Bpanel (row‑major) ---
            for (int jr = 0; jr < nc; jr += NR) {
                int nr = std::min(NR, nc - jr);
                for (int l = 0; l < kc; ++l) {
                    for (int jj = 0; jj < nr; ++jj)
                        Bpanel[jr * kc + l * NR + jj] = alpha * B[(p + l) + (j + jr + jj) * ldb];
                    for (int jj = nr; jj < NR; ++jj)
                        Bpanel[jr * kc + l * NR + jj] = 0.0;
                }
            }

            for (int i = 0; i < m; i += MC) {
                int mc = std::min(MC, m - i);
                double* Cpanel = &C[i + j * ldc];

                // --- beta scaling or zeroing of the current Cpanel ---
                if (first_k_block) {
                    if (beta == 0.0) {
                        for (int jj = 0; jj < nc; ++jj) {
                            double* Ccol = Cpanel + jj * ldc;
                            for (int ii = 0; ii < mc; ++ii) Ccol[ii] = 0.0;
                        }
                    } else if (beta != 1.0) {
                        for (int jj = 0; jj < nc; ++jj) {
                            double* Ccol = Cpanel + jj * ldc;
                            for (int ii = 0; ii < mc; ++ii) Ccol[ii] *= beta;
                        }
                    }
                }

#ifdef _OPENMP
                // Use thread-local buffer
                double* Apanel = thread_Apanel;
#else
                // In single-threaded mode, use the global buffer
                double* Apanel = Apanel;
#endif

                // --- pack mc×kc slice of A into Apanel ---
                for (int ir = 0; ir < mc; ir += MR) {
                    for (int l = 0; l < kc; ++l) {
                        for (int ii = 0; ii < MR; ++ii)
                            Apanel[ir * kc + l * MR + ii] = A[(i + ir + ii) + (p + l) * lda];
                    }
                }

                // --- micro‑kernel computation over jr×ir tiles ---
                // (not parallelized at this level anymore)
                for (int jr = 0; jr < nc; jr += NR) {
                    for (int ir = 0; ir < mc; ir += MR) {
                        double* Cblk = Cpanel + jr * ldc + ir;
                        avx2_micro_kernel_4x12_aligned(kc,
                                                      &Apanel[ir * kc], MR,
                                                      &Bpanel[jr * kc], NR,
                                                      Cblk, ldc);
                    }
                }
            } // i loop (MC)
        } // p loop (KC)
    } // j loop (NC)
#ifdef _OPENMP
}
#endif
}

// --------------------------------------------------------------------------------
// Naïve DGEMM for correctness checking (unchanged)
// --------------------------------------------------------------------------------
static void dgemm_naive(int m, int n, int k, double alpha,
                        const double* A, int lda,
                        const double* B, int ldb,
                        double beta,
                        double* C, int ldc)
{
    if (beta != 1.0) {
        for (int j = 0; j < n; ++j)
            for (int i = 0; i < m; ++i)
                C[i + j * ldc] = beta * C[i + j * ldc];
    }
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < m; ++i) {
            double sum = 0.0;
            for (int l = 0; l < k; ++l)
                sum += A[i + l * lda] * B[l + j * ldb];
            C[i + j * ldc] += alpha * sum;
        }
}

// --------------------------------------------------------------------------------
// Helper utilities (unchanged)
// --------------------------------------------------------------------------------
static bool verify_results(int m, int n, int k, double alpha,
                           const double* A, int lda,
                           const double* B, int ldb,
                           double beta,
                           double* C, int ldc)
{
    std::vector<double> C_ref(m * n);
    std::vector<double> C_opt(m * n);
    std::copy(C, C + m * n, C_ref.begin());
    std::copy(C, C + m * n, C_opt.begin());

    dgemm_naive(m, n, k, alpha, A, lda, B, ldb, beta, C_ref.data(), ldc);
    dgemm_avx_kernel_nn(m, n, k, alpha, A, lda, B, ldb, beta, C_opt.data(), ldc);

    const double eps = 1e-10;
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < m; ++i)
            if (std::abs(C_ref[i + j * ldc] - C_opt[i + j * ldc]) > eps) return false;
    return true;
}

static void generate_random_matrix(int rows, int cols, double* mat)
{
    std::mt19937 mt(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int j = 0; j < cols; ++j)
        for (int i = 0; i < rows; ++i)
            mat[i + j * rows] = dist(mt);
}

template <class F> static double benchmark(F f) {
    auto t0 = std::chrono::high_resolution_clock::now();
    f();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(t1 - t0).count();
}

// --------------------------------------------------------------------------------
// Entry point (prints basic info, optionally verifies, then benchmarks)
// --------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    bool perform_checks = true;
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--nocheck") perform_checks = false;

#ifdef _OPENMP
    std::cout << "OpenMP enabled – max threads: " << omp_get_max_threads() << "\n";
#else
    std::cout << "OpenMP not enabled, running single‑threaded." << "\n";
#endif

    std::ofstream csv("dgemm_benchmark_4x12avx2_kernel_results.csv");
    if (!csv.is_open()) { std::cerr << "Failed to open CSV output." << std::endl; return 1; }
    csv << "m,n,k,GFLOPS1,GFLOPS2,GFLOPS3,GFLOPS4,GFLOPS5,verified\n";

    std::set<int> size_set;
    for (int s = 24;  s <= 1200; s += 12)   size_set.insert(s);
    for (int s = 1200; s <= 9600; s += 1200) size_set.insert(s);
    std::vector<int> sizes(size_set.begin(), size_set.end());

    const int trials = 5;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int nsz : sizes) {
        int m = nsz, n = nsz, k = nsz;
        double flops_total = static_cast<double>(m) * n * (2.0 * k + 1);
        std::vector<double> A(m * k), B(k * n), C(m * n);
        double alpha = dist(mt), beta = dist(mt);
        generate_random_matrix(m, k, A.data());
        generate_random_matrix(k, n, B.data());
        generate_random_matrix(m, n, C.data());

        bool verified = true;
        if (perform_checks) verified = verify_results(m, n, k, alpha, A.data(), m, B.data(), k, beta, C.data(), m);
        csv << m << ',' << n << ',' << k;

        for (int t = 0; t < trials; ++t) {
            std::vector<double> Ctmp = C;
            double secs = benchmark([&] { dgemm_avx_kernel_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, Ctmp.data(), m); });
            csv << ',' << flops_total / secs / 1.0e9;
        }
        csv << ',' << (perform_checks ? (verified ? "True" : "False") : "Skipped") << '\n';
        std::cout << "Size " << nsz << " done." << (verified ? " OK" : " FAIL") << std::endl;
    }
    return 0;
}

