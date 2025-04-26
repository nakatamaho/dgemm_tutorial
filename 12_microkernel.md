# マイクロカーネルの設計

大きいものを小さいものに段階的にブロック行列に分解してゆき、キャッシュ階層をうまくつかいつつ、メモリアクセスを極力避けるのがDGEMMの高速化の重要なところです。プログラミングの観点からゆくと、小さいものから大きいものを作ったほうが見通しがよくなります。ここでは一番内側のループとなる、マイクロカーネルの設計をやってみましょう。

## 1. 概要
最内ループ（マイクロカーネル）では $`C = A\,B`$そして、大きさは$`C\in\mathbb{R}^{m_R\times n_R},\quad A\in\mathbb{R}^{m_R\times k_R},\quad B\in\mathbb{R}^{k_R\times n_R}`$です。 
```math
C_{m_R\times n_R}\;+\!=\;A_{m_R\times1}\;B_{1\times n_R}
```
という **rank-1 update** を $`k`$ 回繰り返して行列 $`C`$ を計算します。性能を左右する要因は主に **(i) レジスタ数** と **(ii) 演算とロードの並行処理** のバランスです。

## 2. レジスタ制約
* Zen 2 アーキテクチャでは 256-bit YMM レジスタが **16 本/コア** 利用可能です
* 一般的には **半数のレジスタを $`C`$ の保持** に割り当て、残りを *$`A`$ と $`B`$ のプリフェッチ* およびループ制御変数に使用します
$`m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算１回分の実行時間内に **次のデータをL2からレジスタへ転送** する必要があります。Rooflineモデル的に表現すると：

```math
$$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $$
```

* $`R_\text{comp}`$ … 1コアあたりの倍精度浮動小数点演算性能（FLOP/cycle）
* $`R_\text{load}`$ … L2→レジスタへのロード帯域（double/cycle）
* 分母の "2" は、1回のrank-1 updateで **A,B から約2要素** のロードが必要なためです

### Zen 2 プロセッサの特性
* **計算性能** : 2 × 256-bit FMA/clk → $`R_\text{comp}=16\;\text{FLOP/cycle}`$
* **メモリ帯域** : L2→L1 32 B/clk = 4 double/clk → $`R_\text{load}=4\;\text{double/cycle}`$

これより
$`n_r \;\ge\; \frac{16}{2\times4}=2`$ 

つまり、**列幅が2以上** であれば、理論上はロード遅延を完全に隠蔽できます。

## 4. サイズ候補と評価

| 候補 $`m_r\times n_r`$ | レジスタ使用量 ($`\overline{C}`$) | (★)条件の充足度 | 実用上の特徴 |
|----------------------|---------------|------------|------------|
| 4 × 4                | 4 YMM (25%)   | ◎          | 実装が容易で移植性に優れています |
| 6 × 6                | 9 YMM (56%)   | ◎          | 計算密度が高く、TLB/帯域がボトルネックになりにくいです |
| 6 × 8                | 12 YMM (75%)  | ◎          | BLISライブラリが採用。プリフェッチ用のレジスタ残量はぎりぎりです |
| 8 × 8                | 16 YMM (100%) | ○          | レジスタ使用率が最大。ロード先行命令の工夫が必要です |

**推奨アプローチ**: まずは **4 × 4** で実装し、性能プロファイルを確認した上で **6 × 6** へ拡張するのが安全です。

## 5. 教育用 4 × 4 AVX2 カーネル（抜粋）

```cpp
#include <immintrin.h>

inline void micro_kernel_4x4(
    const double* A, const double* B, double* C,
    std::size_t ldc, std::size_t k)
{
    __m256d c0 = _mm256_loadu_pd(C + 0*ldc);
    __m256d c1 = _mm256_loadu_pd(C + 1*ldc);
    __m256d c2 = _mm256_loadu_pd(C + 2*ldc);
    __m256d c3 = _mm256_loadu_pd(C + 3*ldc);

    for (std::size_t p = 0; p < k; ++p) {
        __m256d a   = _mm256_loadu_pd(A + 4*p);
        __m256d bp0 = _mm256_broadcast_sd(B + p*4 + 0);
        __m256d bp1 = _mm256_broadcast_sd(B + p*4 + 1);
        __m256d bp2 = _mm256_broadcast_sd(B + p*4 + 2);
        __m256d bp3 = _mm256_broadcast_sd(B + p*4 + 3);

        c0 = _mm256_fmadd_pd(a, bp0, c0);
        c1 = _mm256_fmadd_pd(a, bp1, c1);
        c2 = _mm256_fmadd_pd(a, bp2, c2);
        c3 = _mm256_fmadd_pd(a, bp3, c3);
    }

    _mm256_storeu_pd(C + 0*ldc, c0);
    _mm256_storeu_pd(C + 1*ldc, c1);
    _mm256_storeu_pd(C + 2*ldc, c2);
    _mm256_storeu_pd(C + 3*ldc, c3);
}
```

## 6. まとめ
* **レジスタ半分ルール** と **(★) ロード隠蔽条件** がマイクロカーネルサイズ選定の基本指針です。
* Zen 2 アーキテクチャでは **$`n_r\ge2`$** を満たすことで帯域律速を回避できます。
* 実装では **4 × 4** から始め、必要に応じて **6 × 6** または **6 × 8** へ拡張し、レジスタ使用率とロードポート飽和のバランスを調整するとよいでしょう。
