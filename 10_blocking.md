# 10 ブロッキングの数理 (Blocking)

> **狙い** — 行列積 *C ← A B* をブロック(タイル)単位で計算できる数学的根拠を示し、キャッシュ階層と結び付けたブロックサイズ選択の指針を概観します。実装テクニックは次章で扱います。


## 1 なぜブロッキングが必要か

行列積 `DGEMM` は(正方行列のとき)演算量が $`O(N^3)`$、データ転送量が $`O(N^2)`$ なので、**要素を平均 $`O(N)`$ 回再利用できる**潜在力があります。メモリ局所性を確保する代表的手法が *ブロッキング* です。

## 2 行列をブロックで表す

まず、$`A \in \mathbb{R}^{m\times k}`$ と $`B \in \mathbb{R}^{k\times n}`$ を一様サイズ $`b`$ の正方ブロックに分割します。
$`A_{ij}`$と、$`B_{ij}`$は$`b\times b`$の正方行列になります。ブロック数は

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

で表せます。
つまり各$`C_{ij}`$ブロックは、ブロック行列$`A_{i\ell}`$と、$`B_{\ell j}`$たちでとる内積のような方法で計算できるということです。
```math
$$
C_{ij} = \sum_{\ell=1}^{q} A_{i\ell} \, B_{\ell j}=
\begin{bmatrix}
  A_{i1} & A_{i2} & \cdots & A_{iq}
\end{bmatrix}
\begin{bmatrix}
  B_{1j} \\
  B_{2j} \\
  \vdots \\
  B_{qj}
\end{bmatrix}
$$
```

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

### 証明

各 \$`C_{ij}`\$ の \$(\alpha,\beta)\$ 成分 ($`0\le\alpha<b_i`$, $`0\le\beta<b_j`$) を展開すると

```math
$$
  (C_{ij})_{\alpha\beta} = \sum_{x=0}^{k-1} A_{i\,\alpha,\,x}\;B_{x,\,j\,\beta}
$$
```

しかし添字 \$`x`\$ をブロック単位 \$`b`\$ ごとに分解し、\$`x=\ell b+\gamma`\$ と置くと

```math
$$
  (C_{ij})_{\alpha\beta}
  = \sum_{\ell=0}^{q-1} \sum_{\gamma=0}^{b_{\ell}-1}
     A_{i\ell,\alpha\gamma}\;B_{\ell j,\gamma\beta}
  = \bigl(A_{i\ell} B_{\ell j}\bigr)_{\alpha\beta}.
$$
```
証明終わり

注意: このブロック化定理は行列のサイズが$`b`$で割り切れないときも成り立ちます。

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

## 5 まとめ

1. ブロッキングは厳密な数学的同値変形。
2. キャッシュ効率の向上で算術集約度を高められる。
3. ブロックサイズはキャッシュ容量の平方根が第一近似。

