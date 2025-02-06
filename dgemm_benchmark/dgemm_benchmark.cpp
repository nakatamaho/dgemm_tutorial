/*
 * Copyright (c) 2025
 *     Nakata, Maho
 *     All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <random>

#ifdef _OPENMP
#include <omp.h>
#endif

extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double *alpha, const double *A, const int *lda, const double *B, const int *ldb, const double *beta, double *C, const int *ldc);
}

void dgemm_ref(const char *transa, const char *transb, int m, int n, int k, double alpha, const double *A, int lda, const double *B, int ldb, double beta, double *C, int ldc);

void generate_random_matrix(int rows, int cols, double *matrix, int ld) {
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    int i, j;
    for (j = 0; j < cols; j++) {
        for (i = 0; i < rows; i++) {
            matrix[i + j * ld] = dist(mt);
        }
    }
}

void calculate_mean_and_variance(const double *values, int n, double &mean, double &variance) {
    int i;
    mean = 0.0;
    for (i = 0; i < n; i++) {
        mean += values[i];
    }
    mean /= n;
    variance = 0.0;
    for (i = 0; i < n; i++) {
        double diff = values[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
}

double compute_max_abs_diff(const double *ref, const double *test, int size) {
    int i;
    double max_diff = 0.0;
    for (i = 0; i < size; i++) {
        double diff = fabs(ref[i] - test[i]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}

//
// メイン関数
//
int main(void) {
#ifdef _OPENMP
    std::cout << "OpenMP is enabled." << std::endl;
    std::cout << "Using maximum available threads: " << omp_get_max_threads() << std::endl;
#else
    std::cout << "OpenMP is not enabled." << std::endl;
#endif

    const int num_trials = 10;

    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int n = 1; n <= 2000; n = n + 13) {
        int m = n;
        int k = n;
        int lda = m;
        int ldb = k;
        int ldc = m;

        double flop_count = static_cast<double>(n) * n * (2.0 * n + 1.0);

        double *A = new double[lda * k];
        double *B = new double[ldb * n];
        double *C = new double[ldc * n];
        double *C_ref = new double[ldc * n];

        double alpha = dist(mt);
        double beta = dist(mt);

        generate_random_matrix(m, k, A, lda);
        generate_random_matrix(k, n, B, ldb);
        generate_random_matrix(m, n, C, ldc);
        {
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C_ref[i + j * ldc] = C[i + j * ldc];
                }
            }
        }
        dgemm_ref("N", "N", (int)m, (int)n, (int)k, alpha, A, (int)lda, B, (int)ldb, beta, C_ref, (int)ldc);

        double *flops_results = new double[num_trials];
        double *diff_results = new double[num_trials];

        for (int trial = 0; trial < num_trials; trial++) {
            double *C_test = new double[ldc * n];
            {
                int i, j;
                for (j = 0; j < n; j++) {
                    for (i = 0; i < m; i++) {
                        C_test[i + j * ldc] = C[i + j * ldc];
                    }
                }
            }

            std::chrono::high_resolution_clock::time_point start, end;
            start = std::chrono::high_resolution_clock::now();

            dgemm_("N", "N", &m, &n, &k, &alpha, A, &lda, B, &ldb, &beta, C_test, &ldc);

            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double elapsed_time = elapsed.count();

            double flops = flop_count / elapsed_time / 1.0e6;
            flops_results[trial] = flops;

            double max_diff = compute_max_abs_diff(C_ref, C_test, m * n);
            diff_results[trial] = max_diff;

            delete[] C_test;
        }

        double mean_flops, var_flops;
        calculate_mean_and_variance(flops_results, num_trials, mean_flops, var_flops);
        double mean_diff, var_diff;
        calculate_mean_and_variance(diff_results, num_trials, mean_diff, var_diff);

        std::cout << "Matrix size " << n << "x" << n << ":\n";
        std::cout << "  FLOPS for each trial [MFLOPS]: ";
        for (int trial = 0; trial < num_trials; trial++) {
            std::cout << flops_results[trial] << " ";
        }
        std::cout << "\n";
        std::cout << "  Mean FLOPS: " << mean_flops << " MFLOPS, Variance: " << var_flops << "\n";
        std::cout << "  Max Abs Diff for each trial: ";
        for (int trial = 0; trial < num_trials; trial++) {
            std::cout << diff_results[trial] << " ";
        }
        std::cout << "\n";
        std::cout << "  Mean of Max Diff: " << mean_diff << ", Variance of Max Diff: " << var_diff << "\n";
        std::cout << "---------------------------------\n";

        delete[] A;
        delete[] B;
        delete[] C;
        delete[] C_ref;
        delete[] flops_results;
        delete[] diff_results;
    }

    return 0;
}
