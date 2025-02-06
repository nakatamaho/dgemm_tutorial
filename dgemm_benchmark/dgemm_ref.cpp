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

#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <algorithm>

void Mxerbla(const char *srname, int info) {
    fprintf(stderr, " ** On entry to %s parameter number %2d had an illegal value\n", srname, info);
    exit(info);
}

bool Mlsame(const char *a, const char *b) {
    if (toupper(*a) == toupper(*b))
        return true;
    return false;
}

void dgemm_ref(const char *transa, const char *transb, int m, int n, int k, double alpha, double *A, int lda, double *B, int ldb, double beta, double *C, int ldc) {
    int nota, notb;
    int nrowa, ncola;
    int nrowb;
    int info;

    double Zero = 0.0, One = 1.0;
    double temp;

    nota = Mlsame(transa, "N");
    notb = Mlsame(transb, "N");

    if (nota) {
        nrowa = m;
        ncola = k;
    } else {
        nrowa = k;
        ncola = m;
    }
    if (notb) {
        nrowb = k;
    } else {
        nrowb = n;
    }

    // Test the input parameters.
    info = 0;
    if (!nota && (!Mlsame(transa, "C")) && (!Mlsame(transa, "T")))
        info = 1;
    else if (!notb && (!Mlsame(transb, "C")) && (!Mlsame(transb, "T")))
        info = 2;
    else if (m < 0)
        info = 3;
    else if (n < 0)
        info = 4;
    else if (k < 0)
        info = 5;
    else if (lda < std::max((int)1, nrowa))
        info = 8;
    else if (ldb < std::max((int)1, nrowb))
        info = 10;
    else if (ldc < std::max((int)1, m))
        info = 13;
    if (info != 0) {
        Mxerbla("dgemm_ref ", info);
        return;
    }
    // Quick return if possible.
    if ((m == 0) || (n == 0) || (((alpha == Zero) || (k == 0)) && (beta == One)))
        return;

    // And when alpha == 0.0
    if (alpha == Zero) {
        if (beta == Zero) {
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = Zero;
                }
            }
        } else {
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = beta * C[i + j * ldc];
                }
            }
        }
        return;
    }
    // Start the operations.
    if (notb) {
        if (nota) {
            // Form C := alpha*A*B + beta*C.
            for (int j = 0; j < n; j++) {
                if (beta == Zero) {
                    for (int i = 0; i < m; i++) {
                        C[i + j * ldc] = Zero;
                    }
                } else if (beta != One) {
                    for (int i = 0; i < m; i++) {
                        C[i + j * ldc] = beta * C[i + j * ldc];
                    }
                }
                for (int l = 0; l < k; l++) {
                    if (B[l + j * ldb] != Zero) {
                        temp = alpha * B[l + j * ldb];
                        for (int i = 0; i < m; i++) {
                            C[i + j * ldc] = C[i + j * ldc] + temp * A[i + l * lda];
                        }
                    }
                }
            }
        } else {
            // Form  C := alpha*A'*B + beta*C.
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    temp = Zero;
                    for (int l = 0; l < k; l++) {
                        temp = temp + A[l + i * lda] * B[l + j * ldb];
                    }
                    if (beta == Zero)
                        C[i + j * ldc] = alpha * temp;
                    else
                        C[i + j * ldc] = alpha * temp + beta * C[i + j * ldc];
                }
            }
        }
    } else {
        if (nota) {
            // Form  C := alpha*A*B' + beta*C.
            for (int j = 0; j < n; j++) {
                if (beta == Zero) {
                    for (int i = 0; i < m; i++) {
                        C[i + j * ldc] = Zero;
                    }
                } else if (beta != One) {
                    for (int i = 0; i < m; i++) {
                        C[i + j * ldc] = beta * C[i + j * ldc];
                    }
                }
                for (int l = 0; l < k; l++) {
                    if (B[j + l * ldb] != Zero) {
                        temp = alpha * B[j + l * ldb];
                        for (int i = 0; i < m; i++) {
                            C[i + j * ldc] = C[i + j * ldc] + temp * A[i + l * lda];
                        }
                    }
                }
            }
        } else {
            // Form  C := alpha*A'*B' + beta*C.
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    temp = Zero;
                    for (int l = 0; l < k; l++) {
                        temp = temp + A[l + i * lda] * B[j + l * ldb];
                    }
                    if (beta == Zero)
                        C[i + j * ldc] = alpha * temp;
                    else
                        C[i + j * ldc] = alpha * temp + beta * C[i + j * ldc];
                }
            }
        }
    }
    return;
}
