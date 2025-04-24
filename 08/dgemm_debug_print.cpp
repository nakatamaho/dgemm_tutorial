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
#include <iomanip>
#include <random>
extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n, const int *k, const double *alpha, const double *A, const int *lda, const double *B, const int *ldb, const double *beta, double *C, const int *ldc);
}
void dgemm_NN_blocked_omp(int m, int n, int k, double alpha, double *A, int lda, double *B, int ldb, double beta, double *C, int ldc);

static void print_matrix_octave(const char *name, const double *M, int m, int n, int ldm) {
    std::cout << name << " = [\n";
    for (int i = 0; i < m; i++) {
        std::cout << "    ";
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(5) << M[i + j * ldm];
            if (j < n - 1) {
                std::cout << " ";
            }
        }
        if (i < m - 1) {
            std::cout << ";\n";
        }
    }
    std::cout << "\n];\n\n";
}

int main() {
    int m = 4;
    int n = 4;
    int k = 4;

    int lda = m + 4;
    int ldb = k + 8;
    int ldc = m + 10;

    double *A = new double[lda * k];
    double *B = new double[ldb * n];
    double *C = new double[ldc * n];
    double *Cref = new double[ldc * n];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-9, 9);

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < lda; i++) {
            A[i + j * lda] = -1000.0;
        }
    }
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < ldb; i++) {
            B[i + j * ldb] = -1000.0;
        }
    }
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < ldc; i++) {
            C[i + j * ldc] = -1000.0;
            Cref[i + j * ldc] = -1000.0;
        }
    }

    for (int j = 0; j < k; j++) {
        for (int i = 0; i < m; i++) {
            A[i + j * lda] = dis(gen);
        }
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < k; i++) {
            B[i + j * ldb] = dis(gen);
        }
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            C[i + j * ldc] = dis(gen);
            Cref[i + j * ldc] = C[i + j * ldc];
        }
    }

    double alpha = dis(gen);
    double beta = dis(gen);

    print_matrix_octave("A", A, m, k, lda);
    print_matrix_octave("B", B, k, n, ldb);
    print_matrix_octave("C", C, m, n, ldc);

    std::cout << "alpha = " << alpha << ";\n";
    std::cout << "beta  = " << beta << ";\n\n";

    dgemm_("N", "N", &m, &n, &k, &alpha, A, &lda, B, &ldb, &beta, C, &ldc);
    print_matrix_octave("C_blas", C, m, n, ldc);

    std::cout << "C = alpha * A * B + beta * C; " << ";\n";

    std::cout << "C_blas - C " << "\n";

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] Cref;
    return 0;
}
