# 1. DGEMMの基礎とインターフェース  

## 行列乗算の数学的背景  

行列乗算は  

$$
C = A \times B,\qquad
C_{ij} = \sum_{k=1}^{n} A_{ik} B_{kj}
$$

という 3 重ループで表される線形代数の最基本演算です。  
ここで  

- $`A \in \mathbb{R}^{m \times k}`$
- $`B \in \mathbb{R}^{k \times n}`$
- $`C \in \mathbb{R}^{m \times n}`$

となります。もしすべてを$`n \times n`$行列とすると、計算量は $`O(n^3)`$――サイズが 2 倍になれば計算量は 8 倍になるため、**アルゴリズム効率と実装最適化が性能の決め手** となります。

## 科学技術計算における重要性  

- **数値線形代数** – LU/QR/SVD 分解、固有値問題はすべて行列乗算を内部に抱える  
- **機械学習・深層学習** – 前向き・後向き伝搬のコアは多バッチ行列積
- **人工知能（AI）** - 性能が行列積に強く依存し、リアルタイム推論や大規模トレーニングの性能を左右  
- **量子化学・物性計算** – 2 電子積分変換やテンサーネットワーク演算が DGEMM に帰着  
- **シミュレーション全般** – FEM/BEM のアセンブリや時間積分ステップで繰り返し登場  

> **つまり**：行列乗算を高速化できれば、数値計算の多くがその恩恵を受ける。

## DGEMM とは  
BLAS (Basic Linear Algebra Subprograms) で定義される汎用倍精度行列乗算カーネル です。

DGEMM の計算式で、このチュートリアルでつかうものは

$$
C \gets \alpha\ * A * B + \beta\ * C
$$

です。
ただ実際に提供されているものは、転置操作を指定しなければなりません。

$$
C \gets \alpha\ * \mathrm{op}(A) * \mathrm{op}(B) + \beta\ * C
$$

DGEMM における `op(A)` および `op(B)` は、行列に適用する演算を指定する引数です。  
`TRANSA`／`TRANSB` の値で以下を選択できます:

- `N` (No transpose):

$$
\mathrm{op}(A) = A
$$

- `T` (Transpose):

$$
\mathrm{op}(A) = A^{T}
$$

- `C` (Conjugate transpose):

$$
\mathrm{op}(A) = A^{H}
\quad(\text{実数行列では }A^{T}\text{ と同じ})
$$

さらに、先導次元(leading dimension)も指定しなければなりません。

最終的にCでのインターフェースは

```c
#include <cblas.h>

/* CBLAS のシグネチャ */
void cblas_dgemm(
    const CBLAS_LAYOUT Layout,       /* 行列のメモリレイアウト */
    const CBLAS_TRANSPOSE TransA,     /* A の転置設定 */
    const CBLAS_TRANSPOSE TransB,     /* B の転置設定 */
    const int M,                      /* C の行数 */
    const int N,                      /* C の列数 */
    const int K,                      /* 演算の内積長 */
    const double alpha,               /* α スカラー */
    const double *A,                  /* A 行列（ポインタ） */
    const int lda,                    /* A の先頭要素から次行（または次列）までのオフセット */
    const double *B,                  /* B 行列（ポインタ） */
    const int ldb,                    /* B の先頭要素から次行（または次列）までのオフセット */
    const double beta,                /* β スカラー */
    double *C,                        /* C 行列（ポインタ、出力） */
    const int ldc                     /* C の先頭要素から次行（または次列）までのオフセット */
);
```

となってます。（CからDGEMMを利用する時はcblasインターフェースを用いるのがより良いようです。

## なぜDGEMMで、SGEMMやAI向けの半精度などではないのか？ ##

DGEMMが最も簡単だからです。精度が低くなるとデータ転送の部分がよりシビアに見えて難易度が高くなります。
