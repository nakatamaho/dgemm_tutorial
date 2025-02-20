# 04 もっとも簡単なDGEMMの実装

この章では、行列積の DGEMM を実装する最も基本的な方法、いわゆる「naive」な実装例を紹介します。  
この実装は、最適化手法は一切施されていないため、計算時間は長くなりますが、アルゴリズムの基本的な流れを理解するには最適です。

## naive 実装の概要

DGEMM の演算は、もっとも丁寧には以下の数式で表されます:

$$
(C)_{ij} = \alpha \sum_{k} (A)_{ik} \, (B)_{kj} + \beta \, (C)_{ij}
$$


この式は、行列 \(C\) の \(i,j\) 成分が、行列 \(A\) の \(i\) 行目の成分と行列 \(B\) の \(j\) 列目の成分の積の総和に \(\alpha\) を掛けたものと、元の \(C\) の \(i,j\) 成分に \(\beta\) を掛けたものの和であることを示しています。


この naive な実装では、以下の手順で計算を進めます。

1. **初期化**: 行列 C の各要素に対して、スカラー βを乗じた値で初期化します。  
2. **積和計算**: 3 重の for ループを用いて、各要素に対して A の行と B の列の内積を計算します。  
3. **スカラー乗算**: 計算した内積にスカラー αを乗じ、その結果を初期化済みの C に加算します。

## サンプルコード

以下は、C++ で書かれた DGEMM の naive な実装例です。

```cpp
void dgemm(int M, int N, int K, double alpha, const double *A, int lda, const double *B, int ldb, double beta, double *C, int ldc) {

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

```

残念ながらこれだけでは動きませんが、次の章で実際に動かしてみましょう。
