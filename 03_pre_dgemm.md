# 行列-行列積の初歩的なプログラム:プレDGEMM

この節では、2つの行列の積を計算する基本的なCプログラムを紹介します。これはDGEMM（Dense General Matrix Multiply）の前段階としての素朴な実装です。

## 行列積の数学的定義

$C = A \times B$ という行列積において、$A$ が $m \times k$ 行列、$B$ が $k \times n$ 行列である場合、結果の $C$ は $m \times n$ 行列となります。

行列 $C$ の各要素 $C_{ij}$ は次のように計算されます：

$$C_{ij} = \sum_{p=0}^{k-1} A_{ip} \times B_{pj}$$

## C言語による基本実装（二次元配列版）

```c
#include <stdio.h>
#include <stdlib.h>

// 行列の表示関数
void print_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 行列積を計算する関数
void matrix_multiply(double **A, double **B, double **C, int m, int n, int k) {
    // A: m x k行列
    // B: k x n行列
    // C: m x n行列（結果）
    
    // 結果行列Cを0で初期化
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
        }
    }
    
    // 行列積の計算
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int p = 0; p < k; p++) {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
    }
}

// 使用例
int main() {
    int m = 3; // 行列Aの行数
    int k = 2; // 行列Aの列数、行列Bの行数
    int n = 4; // 行列Bの列数
    
    // 行列のメモリ確保
    double **A = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) {
        A[i] = (double *)malloc(k * sizeof(double));
    }
    
    double **B = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++) {
        B[i] = (double *)malloc(n * sizeof(double));
    }
    
    double **C = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) {
        C[i] = (double *)malloc(n * sizeof(double));
    }
    
    // 行列Aの初期化
    A[0][0] = 1.0; A[0][1] = 2.0;
    A[1][0] = 3.0; A[1][1] = 4.0;
    A[2][0] = 5.0; A[2][1] = 6.0;
    
    // 行列Bの初期化
    B[0][0] = 1.0; B[0][1] = 2.0; B[0][2] = 3.0; B[0][3] = 4.0;
    B[1][0] = 5.0; B[1][1] = 6.0; B[1][2] = 7.0; B[1][3] = 8.0;
    
    printf("行列 A (%dx%d):\n", m, k);
    print_matrix(A, m, k);
    
    printf("行列 B (%dx%d):\n", k, n);
    print_matrix(B, k, n);
    
    // 行列積の計算
    matrix_multiply(A, B, C, m, n, k);
    
    printf("行列 C = A x B (%dx%d):\n", m, n);
    print_matrix(C, m, n);
    
    // メモリ解放
    for (int i = 0; i < m; i++) {
        free(A[i]);
    }
    free(A);
    
    for (int i = 0; i < k; i++) {
        free(B[i]);
    }
    free(B);
    
    for (int i = 0; i < m; i++) {
        free(C[i]);
    }
    free(C);
    
    return 0;
}
```

このプログラムでは、行列を二次元配列として表現しています。各要素には `matrix[i][j]` のようにアクセスできます。これにより、コードが数学的な表記により近く、直感的になります。

上記の実装は教育目的の基本的なものであり、実用的な行列計算ではBLASやLAPACKなどの最適化されたライブラリを使用することが推奨されます。
