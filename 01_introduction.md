# 1. DGEMMの基礎とインターフェース  

## 行列乗算の数学的背景  

行列乗算は  

$$
C = A \times B,\qquad
C_{ij} = \sum_{k=1}^{n} A_{ik} B_{kj}
$$

という 3 重ループで表される線形代数の最基本演算です。  
ここで  

- $A \in \mathbb{R}^{m \times k}$
- $B \in \mathbb{R}^{k \times n}$
- $C \in \mathbb{R}^{m \times n}$

となります。計算量は $O(mnk)$――サイズが 2 倍になれば計算量は 8 倍になるため、**アルゴリズム効率と実装最適化が性能の決め手** となります。

## 科学技術計算における重要性  

- **数値線形代数** – LU/QR/SVD 分解、固有値問題はすべて行列乗算を内部に抱える  
- **機械学習・深層学習** – 前向き・後向き伝搬のコアは多バッチ行列積  
- **量子化学・物性計算** – 2 電子積分変換やテンサーネットワーク演算が DGEMM に帰着  
- **シミュレーション全般** – FEM/BEM のアセンブリや時間積分ステップで繰り返し登場  

> **つまり**：行列乗算を高速化できれば、数値計算の多くがその恩恵を受ける。

## DGEMM とは  
BLAS (Basic Linear Algebra Subprograms) で定義される汎用倍精度行列乗算カーネル です。

DGEMM の計算式は

$$
C \gets \alpha\,\mathrm{op}(A)\,\mathrm{op}(B) + \beta\,C
$$

で、Cでのインターフェースは

```c
void dgemm_(const char* TRANSA, const char* TRANSB,
            const int* M, const int* N, const int* K,
            const double* ALPHA,
            const double* A, const int* LDA,
            const double* B, const int* LDB,
            const double* BETA,
            double* C, const int* LDC);
```

となってます。

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
