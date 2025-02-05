# 03 もっとも簡単なDGEMMの実装とベンチマーク

この章では、行列積の DGEMM を実装する最も基本的な方法、いわゆる「naive」な実装例を紹介します。  
この実装は、最適化手法は一切施されていないため、計算時間は長くなりますが、アルゴリズムの基本的な流れを理解するには最適です。

## naive 実装の概要

DGEMM の演算は、以下の数式で表されます:

$$
C \gets \alpha A B + \beta C
$$

より詳しくは

$$
(C)_{ij} = \alpha \sum_{k} (A)_{ik} \, (B)_{kj} + \beta \, (C)_{ij}
$$

であり、この式は、行列 \(C\) の \(i,j\) 成分が、行列 \(A\) の \(i\) 行目の成分と行列 \(B\) の \(j\) 列目の成分の積の総和に \(\alpha\) を掛けたものと、元の \(C\) の \(i,j\) 成分に \(\beta\) を掛けたものの和であることを示しています。

この naive な実装では、以下の手順で計算を進めます。

1. **初期化**: 行列 C の各要素に対して、スカラー βを乗じた値で初期化します。  
2. **積和計算**: 3 重の for ループを用いて、各要素に対して A の行と B の列の内積を計算します。  
3. **スカラー乗算**: 計算した内積にスカラー αを乗じ、その結果を初期化済みの C に加算します。

## サンプルコード

以下は、C++ で書かれた DGEMM の naive な実装例です。

```cpp
#include <iostream>
#include <vector>

// 行列を表現するための型定義
using Matrix = std::vector<std::vector<double>>;

// DGEMM の naive 実装
void dgemm_naive(const Matrix &A, const Matrix &B, Matrix &C, double alpha, double beta) {
    int m = A.size();
    int n = B[0].size();
    int k = A[0].size();

    // C = beta * C の初期化
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] *= beta;

    // C += alpha * (A * B) の計算
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) {
                sum += A[i][p] * B[p][j];
            }
            C[i][j] += alpha * sum;
        }
    }
}

int main() {
    // 例として、3x3 の行列を用意
    Matrix A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    Matrix B = {
        {9.0, 8.0, 7.0},
        {6.0, 5.0, 4.0},
        {3.0, 2.0, 1.0}
    };

    // 結果を格納する行列 C をゼロで初期化
    Matrix C = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    double alpha = 1.0;
    double beta  = 0.0;

    // naive な DGEMM の実行
    dgemm_naive(A, B, C, alpha, beta);

    // 結果の表示
    for (const auto &row : C) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
```

## 実装の注意点

- **計算量**: この実装は 3 重ループによるため、計算量は $O(m \times n \times k)$ となり、行列サイズが大きい場合には非常に時間がかかります。
- **最適化の余地**: キャッシュの活用、ループのアンローリング、ベクトル化などの最適化が行われていないため、実際のアプリケーションではより効率的な実装が求められます。
- **教育的目的**: このコードは、DGEMM の基本的な動作を理解するためのシンプルな例として位置付けており、実際の高性能計算環境では最適化技法を取り入れる必要があります。
