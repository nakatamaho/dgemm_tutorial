# DGEMM デバッグガイド - 最小構成と検証テクニック

この節では、C++プログラムでBLAS dgemmと自作実装を比較検証する際の「デバッグ最小構成」を紹介します。DGEMMの高速な実装ではブロック化というテクニックが基本となります。
ブロック化自身は簡単なアイディアです。しかし、ブロック化を正しく実装したり、デバッグするのは意外にも非常に困難です。特にブロックの範囲が行列のサイズを超えていると、すぐにsegmentation faultで処理が中断します。この原因を特定するのは大変厄介です。
ここでは、なるべくデバッグしやすくするためのノウハウを紹介します。

## デバッグの基本ポイント

1. **Octave/Matlab形式で出力**: 計算結果を確認するため、行列をOctave/Matlab互換フォーマットで標準出力に表示します。
   - 出力結果をコピペまたはパイプで流すことでOctaveの結果と直接比較できます。
2. **小さな整数値の使用**: -9〜9までの整数乱数を用いて、ターミナルで目視確認しやすいデータに限定します。
   - 小さな整数値を使うことで、結果が正確かつ画面表示が見やすくなります。
   - 演算結果の行列は整数行列となるため、正解と誤差ゼロで比較できます。
3. **番兵値の活用**: 行列外アクセスを検知するため、未使用領域を番兵値（-1000）で埋めます。
   - 大きな値が出た時にバグだとすぐわかります。
4. **小さな行列サイズ**: 行列サイズを意図的に4×4に固定し、手でも検証できるようにします。
5. **leading dimensionに余裕を持たせる**: 意図的にleading-dimensionを大きくし、ブロック化で発生しやすいleading dimension周りのバグを検出しやすくします。
   - バグがある場合、番兵値の影響で計算結果は大きく異常な値になりやすくなります。

## 準備

### BLASとOctaveのインストール

Debian/Ubuntu系の場合:

```bash
$ sudo apt update
$ sudo apt install libopenblas-dev octave
```

* **libopenblas-dev**: BLAS (Basic Linear Algebra Subprograms) のライブラリを使うための開発用パッケージ
* **octave**: MATLAB互換の数値計算ソフトウェア

## テストプログラム
```cpp
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
```


## テストプログラムの実行

```bash
$ cd 08
$ make
$ ./dgemm_debug_print
```

実行例:

```
A = [
        7     4     1     7;
        2     9     8     6;
       -1     9    -4    -2;
       -3    -7    -4    -6
];

B = [
       -9     9     1    -8;
        2    -6     0     4;
        2     8    -2     4;
       -7     0     3     3
];

C = [
        3     6     5     7;
       -7    -7     0    -1;
        4     9    -7     8;
        7     0    -7    -2
];

alpha = -5;
beta  = -3;

C_blas = [
      501  -253  -145    54;
      151  -119   -20  -347;
     -177   448    16  -134;
     -256    85    86   196
];

C = alpha * A * B + beta * C;
C_blas - C
```

## Octaveでの検証

出力結果をそのままOctaveにコピー＆ペーストして検証できます:

```bash
$ ./dgemm_debug_print | octave
ans =

   0   0   0   0
   0   0   0   0
   0   0   0   0
   0   0   0   0
```

**全て0の行列**が表示されれば、実装したDGEMMがBLASの計算結果と一致していることを意味します。

## まとめ

- **検証方法**: 同一の行列・スカラーを使って、実装したDGEMMとBLAS/Octaveでの結果を比較します
- **使用データ**: 小さめのランダムな整数値を使うことで視認性と検証精度が向上します
- **出力形式**: Octave互換フォーマットで出力し、直接Octaveで検証可能です
- **バグ検出**: `C_blas - C` がゼロ行列なら実装が正しい可能性が高いです

このアプローチにより、DGEMMの実装が正しく動作しているかを簡単かつ効率的に検証できます。
