# 09 ブロッキングの数理 (Blocking)

> **狙い** — 行列積 *C ← A B* をブロック（タイル）単位で計算できる数学的根拠を示し、キャッシュ階層と結び付けたブロックサイズ選択の指針を概観します。実装テクニックは次章で扱います。

---

## 1 なぜブロッキングが必要か

行列積 `DGEMM` は演算量が $`O(N^3)`$、データ転送量が $`O(N^2)`$ なので、**要素を平均 $`O(N)`$ 回再利用できる**潜在力があります。メモリ局所性を確保する代表的手法が *ブロッキング* です。

---

## 2 行列をブロックで表す

まず、$`A \in \mathbb{R}^{m\times k}`$ と $`B \in \mathbb{R}^{k\times n}`$ を一様サイズ $`b`$ の正方ブロックに分割します。ブロック数は

```math
$$
p = \lceil m/b \rceil,\; q = \lceil k/b \rceil,\; r = \lceil n/b \rceil
$$
```

と書けます。行列全体を明示すると次のようになります。

```math
$$
A = \begin{bmatrix}
  A_{11} & A_{12} & \cdots & A_{1q} \\
  A_{21} & A_{22} & \cdots & A_{2q} \\
  \vdots & \vdots & \ddots & \vdots \\
  A_{p1} & A_{p2} & \cdots & A_{pq}
\end{bmatrix},\qquad
B = \begin{bmatrix}
  B_{11} & B_{12} & \cdots & B_{1r} \\
  B_{21} & B_{22} & \cdots & B_{2r} \\
  \vdots & \vdots & \ddots & \vdots \\
  B_{q1} & B_{q2} & \cdots & B_{qr}
\end{bmatrix}
$$
```

それらの積は

```math
$$
C=AB = \begin{bmatrix}
  C_{11} & C_{12} & \cdots & C_{1r} \\
  C_{21} & C_{22} & \cdots & C_{2r} \\
  \vdots & \vdots & \ddots & \vdots \\
  C_{p1} & C_{p2} & \cdots & C_{pr}
\end{bmatrix}
= \begin{bmatrix}
  A_{11} & A_{12} & \cdots & A_{1q} \\
  A_{21} & A_{22} & \cdots & A_{2q} \\
  \vdots & \vdots & \ddots & \vdots \\
  A_{p1} & A_{p2} & \cdots & A_{pq}
\end{bmatrix}
 \begin{bmatrix}
  B_{11} & B_{12} & \cdots & B_{1r} \\
  B_{21} & B_{22} & \cdots & B_{2r} \\
  \vdots & \vdots & \ddots & \vdots \\
  B_{q1} & B_{q2} & \cdots & B_{qr}
\end{bmatrix}
$$
```

で表せます。ここで $`b_i,b_j \le b`$ とします。

---

## 3 ブロック行列積の定理

> **定理** — 上の分割について、行列積 $`C = AB`$ の $(i,j)$ ブロックは
>
> ```math
> $$
> C_{ij} = \sum_{\ell=1}^{q} A_{i\ell} \, B_{\ell j},\qquad 1\le i\le p,\;1\le j\le r
> $$
> ```
>
> で与えられる。

*証明スケッチ* — 成分表示で展開し、積の添字をブロック番号とブロック内位置に分割すれば直ちに従います。\qed

---

## 4 キャッシュ階層とブロックサイズ (Ryzen 3970X)

ワーキングセットを

```math
$$
W = 2N^2 \times 8\;[\text{bytes}]
$$
```

とすると、Ryzen 3970X でキャッシュを飽和させる $`N_{\max}`$ は

| 階層 | 容量 $`C`$ | $`N_{\max}=\lfloor\sqrt{C/16}\rfloor`$ |
|------|-----------:|------------------------------:|
| L1 Data | 32 768 B | $`\approx45`$ |
| L2      | 524 288 B | $`\approx181`$ |
| L3 (共有) | 134 217 728 B | $`\approx2896`$ |

---

## 5 数値安定性

ブロッキングは演算順序を組み替えるだけで理論結果は同一です。丸め誤差は加算順の違いに限られ、実務的に問題ありません。

---

## 6 まとめ

1. ブロッキングは厳密な数学的同値変形。
2. キャッシュ効率の向上で算術集約度を高められる。
3. ブロックサイズはキャッシュ容量の平方根が第一近似。

