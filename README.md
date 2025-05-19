# DGEMM 高速化のチュートリアル

C++ を用いて AMD64 (x86_64) アーキテクチャ上で動作する倍精度行列演算(DGEMM)を「ナイーブ実装」から「最先端実装」へ段階的にチューニングし、現代 CPU の理論ピーク性能に迫る方法を学びます。
GEMMとは、general matrix multiplicationの略で、Dはdouble precision(binary64)を表す接頭辞です。DGEMMというのはFortranで書かれたBLASという行列-行列演算のライブラリの一つのルーチンです。
 
- **対象読者**:数値線形代数や HPC の初学者から中級者  
- **ゴール**:  
  1. 純粋な3重ループ実装からの性能差を体感し  
  2. キャッシュ最適化・SIMD ベクトル化・並列化の要点を理解  
  3. 自作コードで 1 TeraFLOPS クラスの行列積を実現  

1. **ステップバイステップで実行可能**  
   - 各章ごとに動くサンプルコード付き

2. **デバッグしやすい工夫**  
   - ブロック化した行列を可視化プリント
     
3. **汎用行列対応**  
   - 正方行列だけでなく任意寸法に対応  
   - 端数処理まで考慮したブロック化実装はできず...  

## 必要なもの
執筆に利用した環境です。CPUによってパラメータなどは変わります。

- **開発環境**:Linux, Ubuntu 
- **ハードウェア**:  
  - amd64(x86_64) AVX2搭載CPU: Ryzen 3970X Threadripper

## おしながき

| Chapter | Title                                          |
|:-------:|------------------------------------------------|
| 0️⃣     | [扉ページ](README.md)                           |
| 1️⃣     | [DGEMM の基礎とインターフェース](01_introduction.md)  |
| 2️⃣     | [Ryzen 3970Xの理論性能値の計算方法](02_flops.md)     |
| 3️⃣     | [行列-行列積の初歩的なプログラム:プレDGEMM](03_pre_dgemm.md)     |    
| 4️⃣     | [行列データの取り扱いとメモリ配置](04_Matrix_Memory_Layout.md)     |
| 5️⃣     | [先導次元(leading dimension)の意味と重要性](05_Leading_Dimension.md)| 
| 6️⃣     | [最も簡単な DGEMM 実装](06_dgemm_naive.md)  |
| 7️⃣     | [OpenBLASの結果との比較](07_openblas.md)|
| 8️⃣     | [DGEMM デバッグガイド - 最小構成と検証テクニック](08_dgemm_debugging_knowhow.md)  |
| 9️⃣    | [CPU メモリ階層](09_memory_hierarchy.md) |
| 1️⃣0️⃣    | [ブロッキングの数理](10_blocking.md) |
| 1️⃣1️⃣   | [データの再利用:ランク-1アップデート法](11_rank_one_update.md) |
| 1️⃣2️⃣   | [マイクロカーネルの設計](12_microkernel.md) |
| 1️⃣3️⃣ | [AVX2(SIMD)を使わない4x4マイクロカーネルの実装](13_microkernel_4x4_noAVX.md) |
| 1️⃣4️⃣  | [パネル化:L2キャッシュからL3キャッシュへの拡張への布石](14_panelization.md) |
| 1️⃣5️⃣  | [DGEMM実装におけるブロック化技術](15_blocked.md) | 
| 1️⃣6️⃣  | [激闘!AVX2マイクロカーネル解説](16_4x4_kernel_avx2.md)|
| 1️⃣7️⃣    |[DGEMM カーネルサイズ比較の詳細分析 - Ryzen Threadripper 3970X](17_avx2_kernels.md)|
| 1️⃣8️⃣    |[OpenMPによるマルチコア活用 on Ryzen Threadripper 3970X](18_openmp.md)|
| 1️⃣9️⃣   |[まとめ](19_summary.md)|
