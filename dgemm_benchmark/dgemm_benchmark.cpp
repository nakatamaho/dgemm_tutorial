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
#include <cstring>
#include <set>
#include <memory>
#include <vector>
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double *alpha, const double *A, const int *lda, const double *B, const int *ldb, const double *beta, double *C, const int *ldc);
}

void dgemm_ref(const char *transa, const char *transb, int m, int n, int k, double alpha, const double *A, int lda, const double *B, int ldb, double beta, double *C, int ldc);

#define DIM_START 5000
#define DIM_END 20000
#define NUMTRIALS 10
#define STEP 13

// cf. https://netlib.org/lapack/lawnspdf/lawn41.pdf p.120
double flops_gemm(int k_i, int m_i, int n_i) {
    double adds, muls, flops;
    double k, m, n;
    m = (double)m_i;
    n = (double)n_i;
    k = (double)k_i;
    muls = m * (k + 2) * n;
    adds = m * k * n;
    flops = muls + adds;
    return flops;
}

void generate_random_matrix(int rows, int cols, double *matrix, int ld) {
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            matrix[i + j * ld] = dist(mt);
        }
    }
}

double compute_max_abs_diff(const double *ref, const double *test, int size) {
    double max_diff = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = fabs(ref[i] - test[i]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}

int main(int argc, char *argv[]) {
    bool no_check = false;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--nocheck") == 0) {
            no_check = true;
        }
    }

    if (!no_check) {
        std::cout << "m,n,k,maxflops,minflops,maxdiff" << std::endl;
    } else {
        std::cout << "m,n,k,maxflops,minflops" << std::endl;
    }
    const int num_trials = NUMTRIALS;
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    std::set<int> n_values;
    for (int n = DIM_START; n <= DIM_END; n += STEP) {
        n_values.insert(n);
    }
    for (int m = 0; (1 << m) <= DIM_END; ++m) {
        int pow2 = 1 << m;
        if (pow2 >= DIM_START && pow2 <= DIM_END)
            n_values.insert(pow2);
        if (pow2 - 1 >= DIM_START && pow2 - 1 <= DIM_END)
            n_values.insert(pow2 - 1);
        if (pow2 + 1 >= DIM_START && pow2 + 1 <= DIM_END)
            n_values.insert(pow2 + 1);
    }

    std::vector<int> sorted_n_values(n_values.begin(), n_values.end());
    std::sort(sorted_n_values.begin(), sorted_n_values.end());

    for (int n : sorted_n_values) {
        int m = n, k = n, lda = m, ldb = k, ldc = m;
        double flop_count = flops_gemm(m, n, k);

        double *A = new double[lda * k];
        double *B = new double[ldb * n];
        double *C = new double[ldc * n];
        double *Corg = new double[ldc * n];
        double *Cref = new double[ldc * n];

        double alpha = dist(mt);
        double beta = dist(mt);

        generate_random_matrix(m, k, A, lda);
        generate_random_matrix(k, n, B, ldb);
        generate_random_matrix(m, n, C, ldc);

        memcpy(Corg, C, sizeof(double) * ldc * n);
        if (!no_check) {
            memcpy(Cref, C, sizeof(double) * ldc * n);
            dgemm_ref("N", "N", m, n, k, alpha, A, lda, B, ldb, beta, Cref, ldc);
        }

        double max_flops = 0.0;
        double min_flops = std::numeric_limits<double>::max();
        for (int trial = 0; trial < num_trials; trial++) {
            memcpy(C, Corg, sizeof(double) * ldc * n);
            auto start = std::chrono::high_resolution_clock::now();
            dgemm_("N", "N", &m, &n, &k, &alpha, A, &lda, B, &ldb, &beta, C, &ldc);
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed_time = std::chrono::duration<double>(end - start).count();
            double flops = flop_count / elapsed_time / 1.0e6;
            if (flops > max_flops) {
                max_flops = flops;
            }
            if (flops < min_flops) {
                min_flops = flops;
            }
        }

        double max_diff = no_check ? 0.0 : compute_max_abs_diff(C, Cref, m * n);
        if (!no_check) {
            std::cout << m << "," << n << "," << k << "," << max_flops << "," << min_flops << "," << max_diff << std::endl;
        } else {
            std::cout << m << "," << n << "," << k << "," << max_flops << "," << min_flops << std::endl;
        }

        delete[] A;
        delete[] B;
        delete[] C;
        delete[] Corg;
        delete[] Cref;
    }
    return 0;
}
