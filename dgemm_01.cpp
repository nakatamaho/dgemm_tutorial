void dgemm_NN(int M, int N, int K, double alpha, const double *A, int lda, const double *B, int ldb, double beta, double *C, int ldc) {
    //まずCをbeta倍する
    for (int j = 0; j < N; j++) {
        // C の j 列を beta 倍する
        for (int i = 0; i < M; i++) {
            C[i + j * ldc] *= beta;
        }
    }
    //Cij <- αΣ(A)ik(B)kj + βCij
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                C[i + j * ldc] += alpha * A[i + k * lda] * B[k + j * ldb];
            }
        }
    }
}
