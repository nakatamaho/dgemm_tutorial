# 1. 導入 – DGEMM基礎と実装の世界  

## 行列乗算の数学的背景  

行列乗算は  

\[
C = A \times B,\qquad
C_{ij} = \sum_{k=1}^{n} A_{ik} B_{kj}
\]

という 3 重ループで表される線形代数の最基本演算です。  
ここで  

* \(A \in \mathbb{R}^{m \times n}\)  
* \(B \in \mathbb{R}^{n \times p}\)  
* \(C \in \mathbb{R}^{m \times p}\)

となります。計算量は \(O(mnp)\)――サイズが 2 倍になれば計算量は 8 倍になるため、**アルゴリズム効率と実装最適化が性能の決め手** となります。

## 科学技術計算における重要性  

* **数値線形代数** – LU/QR/SVD 分解、固有値問題はすべて行列乗算を内部に抱える  
* **機械学習・深層学習** – 前向き・後向き伝搬のコアは多バッチ行列積  
* **量子化学・物性計算** – 2 電子積分変換やテンサーネットワーク演算が DGEMM に帰着  
* **シミュレーション全般** – FEM/BEM のアセンブリや時間積分ステップで繰り返し登場  

> **つまり**：行列乗算を高速化できれば、数値計算の多くがその恩恵を受ける。

## DGEMM とは  

BLAS (Basic Linear Algebra Subprograms) で定義される汎用 **倍精度行列乗算カーネル** です。  
インターフェースは

```c
void dgemm_(const char* TRANSA, const char* TRANSB,
            const int* M, const int* N, const int* K,
            const double* ALPHA,
            const double* A, const int* LDA,
            const double* B, const int* LDB,
            const double* BETA,
            double* C, const int* LDC);
