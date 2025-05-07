# パネル化: L2キャッシュ、L3キャッシュを上手に使う

## はじめに

DGEMMの最適化において、CPUのキャッシュ階層を効率的に活用することが性能向上の鍵となります。前回のチュートリアルでは4×4のマイクロカーネルを実装し、L1/L2キャッシュに収まる小～中規模の行列では良好なパフォーマンスを達成しました。しかし、行列サイズがL2キャッシュの容量を超える（グラフ上では約1000×1000付近）と、L2ミスが増加し、L3キャッシュやメインメモリへのアクセスが頻発するため、性能が著しく低下してしまいます。

本章では、この性能低下を抑え、L3キャッシュまで含めたより大きな作業セットを効率よく扱うためのパネル化手法をご紹介します。パネル化によりデータの連続性を保ち、TLBやプリフェッチ機構にも好影響を与えるストライドアクセスパターンを実現できます。

## キャッシュ階層とデータフローの課題

1. **L1/L2キャッシュのサイズ制限**
   L1キャッシュは数十KB、L2キャッシュは数百KB程度と小容量です。そのため、大規模な行列演算ではすぐにキャッシュ外の領域へアクセスしてしまい、キャッシュミス率が高くなります。

2. **L3キャッシュ利用時のTLB制約**
   L3キャッシュは数MB～十数MBと大容量ですが、TLBエントリは数十エントリしか持たず、ページフォールトやTLBミスによる遅延も無視できません。行列をページ境界をまたいで非連続にアクセスすると、TLBミスでさらなるレイテンシ悪化が発生します。

3. **メモリバンド幅の壁**
   メインメモリへのアクセスは数十サイクルの遅延を伴うため、キャッシュミスが増えると演算パフォーマンスがメモリバウンドとなり、マイクロカーネルの最適化効果が十分に発揮されなくなります。

## 現在の実装の限界

現在の実装では、行列を4×4の小さなブロックに分割して処理しています：

```cpp
#define MR 4
#define NR 4

// Process by blocks (MR x NR blocks)
for (int j = 0; j < n; j += NR) {
    for (int i = 0; i < m; i += MR) {
        // 小さなブロックをコピーして処理
        // ...
    }
}
```

この方法は、小さなブロックがL1/L2キャッシュに収まる場合には効果的ですが、大きな行列では以下の問題が発生します：

1. **頻繁なキャッシュミス**: 行列が大きいと、行列A、Bから繰り返しデータを読み込む際にL2キャッシュミスが頻発する
2. **メインメモリへの過剰なアクセス**: L3キャッシュが効果的に活用されず、メインメモリからの低速な読み込みが増加する
3. **データの局所性の低下**: 大きな行列では、時間的・空間的局所性が十分に活用されない

## パネル化技術の導入

* **パネルサイズの選定**

  * 行方向（M軸）にMR×KBのAパネル、列方向（N軸）にKR×NCのBパネルという単位でコピーを行います。
  * コピーを行うことでキャッシュに乗ります(載りやすくなります)
  * KBはL2キャッシュ容量の1/3～1/2程度に設定し、1パネル分のAをL2にと1パネル分のBをL3に常駐させつつ、マイクロカーネルを連続実行できるようにします。

* **コピーのストライド**

  * 元の行列からパネルへのコピーは連続アクセスで行います。
  * パネル内のメモリアクセスはマイクロカーネルの計算アクセスと同様に連続的で、TLBミスを抑制します。

* **ペナルティのペイオフ**

  * コピー操作自体に追加コストは発生しますが、パネル化によりキャッシュミスが大幅に削減され、トータルで性能が向上します。

* ナイーブなパネル化のコード
  以下のようになります。

```cpp
#define CACHELINE 64
#if defined(__GNUC__) || defined(__clang__)
    #define ALIGN(x) __attribute__((aligned(x)))
#elif defined(_MSC_VER)
    #define ALIGN(x) __declspec(align(x))
#else
    #define ALIGN(x)
#endif

ALIGN(CACHELINE) static double Apanel[MC * KC];
ALIGN(CACHELINE) static double Bpanel[KC * NC];

    // Process by panels (MR x NR panels)
    for (int j = 0; j < n; j += NR) {
        int nr = std::min(NR, n - j);  // Handle boundary for partial blocks

        // Pack the B panel (k × NR) and scale by alpha once per j-block
        for (int jj = 0; jj < nr; ++jj) {
            for (int l = 0; l < k; ++l) {
                Bpanel[l + jj * k] = alpha * B[l + (j + jj) * ldb];
            }
        }
        // Zero-pad remaining columns in the B panel
        for (int jj = nr; jj < NR; ++jj) {
            for (int l = 0; l < k; ++l) {
                Bpanel[l + jj * k] = 0.0;
            }
        }
        // Loop over M dimension (rows of C/A) with MR blocks
        for (int i = 0; i < m; i += MR) {
            // Pack the A panel (MR × k, column-major)
            for (int l = 0; l < k; ++l) {
                for (int ii = 0; ii < mr; ++ii) {
                    Apanel[ii + l * MR] = A[(i + ii) + l * lda];
                }
                // Zero-pad remaining rows
                for (int ii = mr; ii < MR; ++ii) {
                    Apanel[ii + l * MR] = 0.0;
                }
            }
```

パネル化により、L2キャッシュに収まるサイズのデータブロックを作成し、そのブロック内での計算を最適化できます。

## Bパネルの転置

### 転置によるメモリアクセスの最適化

ここからは行列の転置がDGEMM性能に与える影響について説明します。C/C++では配列は行優先（row-major）で格納されるため、行列乗算のような処理では、メモリアクセスパターンが非効率になる場合があります。

### 行列Bをそのまま使用する実装

最初の実装では、行列Bをそのままの形で使用しています：

```cpp
// Allocate temporary buffers
double Apanel[MC * KC];


        // Pack the B panel (k × NR) and scale by alpha once per j-block
        for (int jj = 0; jj < nr; ++jj) {
            for (int l = 0; l < k; ++l) {
                Bpanel[l + jj * k] = alpha * B[l + (j + jj) * ldb];
            }
        }

// マイクロカーネルでのアクセス
double b0 = B[l + 0 * ldb];
double b1 = B[l + 1 * ldb];
double b2 = B[l + 2 * ldb];
double b3 = B[l + 3 * ldb];
```
この方法では、マイクロカーネル内で行列Bの要素にアクセスする際、`l`が変化すると大きなメモリアドレスの変化（ストライド）が生じます。このような非連続的なメモリアクセスはキャッシュミスを引き起こし、パフォーマンスを低下させる原因となります。

### そのままの場合の計算結果
![DGEMM ベンチマークプロット](14/dgemm_benchmark_comparison_plot.png)


### 行列Bを転置して使用する実装

提供されたコードサンプルの2つ目の実装では、行列Bをコピーする際に転置を行っています：

```cpp
// Allocate temporary buffers - 転置するので NCxKC
double Bpanel[NC*KC];
...
    // Process by column panels (MR x NR)
    for (int j = 0; j < n; j += NR) {
        int nr = std::min(NR, n - j);  // columns in this panel

        // Pack transposed B panel (NR × k) and scale by alpha
        for (int jj = 0; jj < nr; ++jj) {
            for (int l = 0; l < k; ++l) {
                Bpanel[jj + l * NR] = alpha * B[l + (j + jj) * ldb];
            }
        }
        // Zero-pad remaining rows in B^T panel
        for (int jj = nr; jj < NR; ++jj) {
            for (int l = 0; l < k; ++l) {
                Bpanel[jj + l * NR] = 0.0;
            }
        }
...
        // Load elements from B (transposed access pattern)
        double b0 = B[0 + l * ldb];
        double b1 = B[1 + l * ldb];
        double b2 = B[2 + l * ldb];
        double b3 = B[3 + l * ldb];
```
この実装ではカーネル内ではコード自体は同じになってます。しかし転置をとったことで、重要な変更点が2つあります：

1. **カーネルの変更点の少なさ**: 元のB行列では離れた位置にあるデータが、Bpanelでは隣接して配置されるため、カーネル内のコードを変更せずにパフォーマンスが向上します。
2. **コピー時の転置**: 内側と外側のループが入れ替わり、データを転置しながらコピーします。ついでにαもかけておきます。

### 転置した場合の計算結果

![DGEMM ベンチマークプロット](14/dgemm_benchmark_comparison_plot.png)

1. パネルへのコピーが入るためにどうしても遅くなります。そして転置操作が入るため小さい領域ではさらに遅くなります。4x4のカーネルは、コピーおよび転置が入らないため、その分のパフォーマンス落ちません。
2. 行列のサイズが64, 128, 256の倍数のときにパフォーマンスが落ちます。これは、キャッシュのL1/L2 キャッシュの セット衝突 (conflict-miss)が起こるのが主な原因です。少しずらすと改善します。対処は後に行う予定です。たとえば、IntelMKLでも[Tip 6: Avoid Leading Dimensions that are Multiples of 256](https://www.intel.com/content/www/us/en/developer/articles/technical/a-simple-example-to-measure-the-performance-of-an-intel-mkl-function.html)となってます。
3. どのようなサイズでもnaive実装、4x4カーネル版より遅くなります。心理的には苦しいところです。

### 転置による性能への影響

転置を行うことで以下のメリットがあります：

1. **カーネルの一元化**: 転置をカーネル外に追い出すことにより、プログラミングの手間が減ります。
2. **連続メモリアクセス**: マイクロカーネル内での計算時、lが変化しても連続したメモリ領域にアクセスできる
3. **キャッシュヒット率の向上**: 連続メモリアクセスによりキャッシュラインの利用効率が上がる
4. **プリフェッチの効率化**: 連続アクセスパターンはCPUのハードウェアプリフェッチャが効率的に機能する

一般に、行列サイズが大きくなるほど、転置によるメリットが顕著になります。

## 結論

行列乗算の最適化では、キャッシュ階層を考慮したブロック分割と、効率的なメモリアクセスパターンを実現するための転置が重要です。L3キャッシュへの最適化を進める際には、多段階のブロック分割を導入し、各レベルでのデータアクセスパターンを最適化することで、大規模な行列でも高いパフォーマンスを達成できます。ここでは、マイクロカーネルの一つ上のパネル化について実装しました。

実際の実装では、様々なブロックサイズや転置の有無をベンチマークしながら、最適な構成を見つけることが重要です。
