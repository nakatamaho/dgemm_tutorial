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

void dgemm_ref(const char *transa, const char *transb, int m, int n, int k, double alpha, const double *A, int lda, const double *B, int ldb, double beta, double *C, int ldc)

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

// 平均と分散を計算（配列は n 個の要素を持つとする）
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

// 2つの行列（全 size 個の要素）間の最大絶対誤差を計算
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

    // 乱数生成器（alpha, beta 用）
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    // 正方行列のサイズを 1 から 1000 までループ
    for (int n = 1; n <= 1000; n++) {
        // 正方行列なので m = n, k = n
        int m = n;
        int k = n;
        // 列優先格納のための先行サイズ
        int lda = m;
        int ldb = k;
        int ldc = m;

        // FLOP 数 (n x n 行列の場合、概算で: n * n * (2*n + 1))
        double flop_count = static_cast<double>(n) * n * (2.0 * n + 1.0);

        // 行列領域の動的確保
        double *A = new double[lda * k];
        double *B = new double[ldb * n];
        double *C = new double[ldc * n];
        double *C_ref = new double[ldc * n];

        // alpha, beta の値（乱数に小さなオフセットを加える）
        double alpha = dist(mt) + dist(mt) * 1.0e-16;
        double beta = dist(mt) + dist(mt) * 1.0e-16;

        // 行列の生成
        generate_random_matrix(m, k, A, lda);
        generate_random_matrix(k, n, B, ldb);
        generate_random_matrix(m, n, C, ldc);

        // 参照解 C_ref = alpha * A*B + beta * C を BLAS で計算
        {
            int i, j;
            for (j = 0; j < n; j++) {
                for (i = 0; i < m; i++) {
                    C_ref[i + j * ldc] = C[i + j * ldc];
                }
            }
        }
        Rgemm_ref("N", "N", m, n, k, alpha, A, lda, B, ldb, beta, C_ref, ldc);

        // 各試行の結果格納用
        double *flops_results = new double[num_trials];
        double *diff_results = new double[num_trials];

        // 各サイズにつき複数回試行して計測
        for (int trial = 0; trial < num_trials; trial++) {
            // C_test に C の初期値をコピー
            double *C_test = new double[ldc * n];
            {
                int i, j;
                for (j = 0; j < n; j++) {
                    for (i = 0; i < m; i++) {
                        C_test[i + j * ldc] = C[i + j * ldc];
                    }
                }
            }

            // 計測開始
            std::chrono::high_resolution_clock::time_point start, end;
            start = std::chrono::high_resolution_clock::now();

            // OpenMP 並列 DGEMM 実行（全コア使用）
            Rgemm_NN_omp(m, n, k, alpha, A, lda, B, ldb, beta, C_test, ldc);

            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double elapsed_time = elapsed.count();

            // MFLOPS（10^6 FLOPS 単位）に換算
            double flops = flop_count / elapsed_time / 1.0e6;
            flops_results[trial] = flops;

            // 参照解との誤差計算
            double max_diff = compute_max_abs_diff(C_ref, C_test, m * n);
            diff_results[trial] = max_diff;

            delete[] C_test;
        }

        // 平均と分散の計算
        double mean_flops, var_flops;
        calculate_mean_and_variance(flops_results, num_trials, mean_flops, var_flops);
        double mean_diff, var_diff;
        calculate_mean_and_variance(diff_results, num_trials, mean_diff, var_diff);

        // 結果の出力
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
