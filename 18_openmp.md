# OpenMPによるマルチコア活用 on Ryzen Threadripper 3970X

> **TL;DR** — AVX2 4 × 12 マイクロカーネル ＋ `#pragma omp parallel for collapse(2)` による **N → M → K ループ**（NMK）は、32 core / 64 thread の Threadripper 3970X で **1.3 TFLOPS 超**を達成し、同条件の OpenBLAS (0.3.29, ZENP) に対して平均 5–10 % 程度の性能差まで詰められる。M → K → N（MKN）との理論比較・実測差は、キャッシュ再利用と OpenMP の負荷分散に起因する。以下では実装の勘所と測定結果を GitHub 用に整理する。

---

## 1. ハードウェアとビルド環境

| Item     | Value                                                             |
| -------- | ----------------------------------------------------------------- |
| CPU      | AMD Ryzen Threadripper 3970X (32C / 64T, **Zen 2**)               |
| ISA      | AVX2 + FMA                                                        |
| メモリ      | 4-ch DDR4-3200 @ 128 GB                                           |
| OS       | Ubuntu 22.04 LTS                                                  |
| Compiler | GCC 13.2 `-O3 -march=znver2 -mfma -mavx2 -fopenmp -funroll-loops` |
| OpenBLAS | 0.3.29 (ZENP kernel)                                              |
| Threads  | `OMP_NUM_THREADS=32`, `OMP_PROC_BIND=spread`, `OMP_PLACES=cores`  |

ビルド & ベンチ一括実行スクリプトは `go.sh` に配置している 。

---

## 2. 実装ハイライト

### 2.1 4 × 12 AVX2 マイクロカーネル

* 4 行（MR=4） × 12 列（NR=12）を一度に計算し、YMM レジスタ 16 本をほぼフル活用
* **B パネルは転置格納**；列方向 gather を避け、`_mm256_set1_pd` で broadcast
* 2 step unroll で L1 からの連続ロード / FMA パイプを維持
* 詳細実装は `avx2_micro_kernel_4x12_aligned` を参照 。

### 2.2 OpenMP 並列化

```cpp
#pragma omp parallel
{
    double Apanel[MC*KC] __attribute__((aligned(4096)));
    double Bpanel[KC*NC] __attribute__((aligned(4096)));

    #pragma omp for collapse(2) schedule(static)
    for (int j = 0; j < n; j += NC)          // N-block
        for (int i = 0; i < m; i += MC) {    // M-block
            ⋯        // K-block 内で A/B pack→kernel
        }
}
```

* **collapse(2)** で N×M ブロックを一次元に平坦化し，64 thread へ均等分配
* 各スレッドが **スレッドローカルのパネルバッファ**を持ち false-sharing を回避
* K-ループは最内側。共有軸なので還元不要（C はスレッド毎に固有ブロック）

実装全体は `18_dgemm_4x12avx2_openmp_kernel.cpp` に収録 。

---

## 3. ループ順序の理論比較

### 3. ループ順序の理論比較

| 順序               | キャッシュ再利用                       | A/B パック回数                                | OpenMP collapse での負荷分散             | 備考                   |
|--------------------|----------------------------------------|-----------------------------------------------|-------------------------------------------|------------------------|
| **NMK**<br>(N→M→K) | B パネルを N ブロック全体で再利用しやすい | A ≈ B                                         | collapse(2) で N×M を均等分配可           | 採用                   |
| **MNK**<br>(M→N→K) | A パネルを M ブロック全体で再利用しやすい | A < B (A のパック回数少・B のパック回数多)    | collapse(2) しても負荷分散が偏りやすい     | A パネル重視時向き     |
| **MKN**<br>(M→K→N) | 最外 M で C ブロック固定。B 再利用良好    | A > B (B パック回数は少)                      | M を並列化するとスレッド間還元が発生       | 単スレッド向き         |
| **NKM**<br>(N→K→M) | A パネル使い回し悪い                   | B ≫ A で帯域浪費                             | collapse 不向き                            | 不採用                 |

* **Threadripper 3970X** は 16 × 64 kB L1D + 16 × 512 kB L2 + 128 MB L3。
  128 × 384 (=49 kB) の B パネルは **L2 にすっぽり**入るため NMK の利が大きい。
* `collapse(2)` によりスケジューラが (j,i) ペアをチャンク単位で配布。
  MKN では K ループ外側で競合が起き、スケールが頭打ちになった。

## 3.1 ループ順序 NMK or MNK?
| 観点             | **NMK**<br>(N-block → M-block → K-block)                                      | **MNK**<br>(M-block → N-block → K-block)                                              |
| -------------- | ----------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| **キャッシュ局所性**   | *C* は列優先行列なので、N ブロックを外側に置くと<br>**列まとまりで順に更新**でき、書込みストライドが一定でヒット率が高い。          | *C* は行方向にジャンプしにくいが、列方向に大きく跳ぶため<br>L1/L2 ミスが増えがち。                                      |
| **B パネルの再利用**  | K が最内なので **N-block 全体**で `B(kc×nc)` を再利用できる。<br>パッキング回数は *A ≈ B*。             | 同じ (i,p) を処理しているあいだ `B` はほぼ使い回せない。<br>*B* のパッキングが **N-block ごと**に発生し帯域を浪費。            |
| **A パネルの再利用**  | `A(mc×kc)` は (i,p) ごとに一度だけパック。<br>パッキング対称。                                    | `A` は (i,p) 固定のあいだ使い回せるので<br>*A* のパッキングは少なめ。                                          |
| **OpenMP 並列化** | `collapse(2)` で **N×M** ペアを一次元化しやすい。<br>K は還元不要なので同期が少ない。                     | 典型的には *M* 方向で並列化。<br>N 方向を共有するためスレッド間 Reduction が必要になりやすい。                            |
| **負荷分散**       | N と M をまとめて割り振れるのでチャンクの粒度を自由に調整でき、<br>32 C/64 T など多コアで偏りが少ない。                 | M 固定で N を内側に持つため、行数が M ブロックの倍数でないと負荷が偏りやすい。                                           |
| **適する状況**      | *• マルチスレッド (collapse) でのスケールを重視*<br>*• B パネルサイズが L2 に十分収まる*<br>*• Dense GEMM* | *• 単スレッド / スレッド数が少ない*<br>*• A パネルが極端に大きく、A のパッキングコストを抑えたい*<br>*• K が小さい (スパース・バンド行列)* |



---

## 4. ベンチマーク結果概要

| Matrix size (square) | OpenBLAS 0.3.29  | 本実装 (NMK)        | Ratio |
| -------------------- | ---------------- | ---------------- | ----- |
| 3 072                | ≈ 440 GFLOPS     | ≈ 234 GFLOPS     | 0.53× |
| 6 144                | ≈ 875 GFLOPS     | ≈ 460 GFLOPS     | 0.53× |
| 12 288               | **1 476 GFLOPS** | **1 322 GFLOPS** | 0.90× |
| 21 744               | 1 418 GFLOPS     | 1 321 GFLOPS     | 0.93× |

* **ピーク性能** : OpenBLAS 1.48 TFLOPS @ 15 792, 本実装 1.32 TFLOPS @ 21 744
* **平均 (≥3 k)** : OpenBLAS 1 344 GFLOPS, 本実装 1 267 GFLOPS (-6 %)
* NMK 版は **ループ一本化による並列効率**が高く、中-大サイズで OpenBLAS に肉薄
* 小サイズ (≤4 k) はマイクロカーネル数が不足し命令DP 100 % ヒットせず差が開く

![openmp](18/dgemm_kernels_comparison.png.png)



> **再現方法**: `go.sh` を実行すると両実装を自動ビルドし、
> `dgemm_benchmark_*.csv` に 5 回計測の GFLOPS が追記される 。

---

## 5. 今後の改善ポイント

1. **Prefetch 調整** — Zen 2 は L2 帯域に余裕があり、
   `prefetchw` を過度に入れると競合; kernel 内 prefetch 行数を動的チューニング。
2. **SIMD 幅アップ** — Zen 4+ で AVX-512 が解禁されたら 8 × 16 Kernel を検討。
3. **Dynamic scheduling** — 行列サイズが素数系などブロック割り切れないケースでは
   `schedule(dynamic, 1)` も試す価値あり。
4. **NUMA 最適化** — TR 3990X 以上の 8-CCD 構成では
   CCD ごとに `OMP_PLACES` を細分割し、ローカルメモリ比率を上げる。

---

### 引用

* コード全体 – `18_dgemm_4x12avx2_openmp_kernel.cpp`&#x20;
* ビルド & 実行スクリプト – `go.sh`&#x20;

---

本ドキュメントが Threadripper ユーザの DGEMM 最適化・OpenMP チューニングの参考になれば幸いです。
