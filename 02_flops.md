# Ryzen 3970Xの理論性能値とチップの詳細（アーキテクチャ）の調べ方

Ryzen Threadripper 3970X の理論性能（theoretical peak performance）の計算方法とチップの詳細(アーキテクチャ)の調べ方を解説します。理論性能は、コンピュータやプロセッサ、スーパーコンピュータなどが理想的な条件下で達成可能な最大の計算性能を指します。これは一般に FLOPS（Floating Point Operations Per Second：毎秒浮動小数点演算回数）という単位で表されます。
なお、ここでの計算はあくまで**理論上のピーク値**であり、実際のアプリケーションでの性能は違う場合が多々あることに注意してください。アーキテクチャ情報は、なるべくオフィシャルサイトから引用すべきですが、あまり細かくなると不必要に難しくなりすぎたり、そもそも出てなかったりなので解説サイトに頼ることになります。

## 1. FLOP と FLOPSと理論性能

**FLOP (Floating Point Operation)** は一回の浮動小数点演算を表す単位です。例えば、`a + b`のような加算や`a × b`のような乗算がそれぞれ1 FLOPに相当します。

**FLOPS (Floating Point Operations Per Second)** は、1秒間に実行できる浮動小数点演算（FLOP）の回数を示す指標です。数値計算の性能を評価する際に広く用いられています。

**理論性能（theoretical peak performance）** は、CPU内部のすべての演算ユニットが完全に並列動作し、命令がパイプラインに滞りなく流れ、かつデータの供給も途切れないという**理想的な条件下**で達成可能な最大のFLOPS値を指します。言い換えれば、**極めて単純な浮動小数点演算（たとえば「1 + 1」のような処理）をひたすら繰り返した場合に、理論上1秒間に実行できる演算回数**と考えることもできます。

以降の文書では、単一の演算数を表す場合には「FLOP」を、1秒あたりの演算回数を表す場合には「FLOPS」を使用します。また、「FLOP/サイクル」は1クロックサイクルあたりに実行できる浮動小数点演算の回数を表します。

## 2. CPU情報の調べ方

まずは CPU の性能を正確に把握しましょう。使っているマシンに搭載されている CPU の型番は、Linux 環境であれば /proc/cpuinfo から確認できます。ターミナルで以下のコマンドを実行してください：

```bash
cat /proc/cpuinfo | grep 'model name' | uniq
```

こちらの環境では以下のような結果が得られました。

```
$ cat /proc/cpuinfo | grep 'model name' | uniq
model name      : AMD Ryzen Threadripper 3970X 32-Core Processor
```

なお、別の Intel 製マシンでも同様に CPU モデルを確認できました。

```bash
$ cat /proc/cpuinfo | grep 'model name' | uniq
model name      : Intel(R) Core(TM) i5-9600 CPU @ 3.10GHz
```

取得したモデル名をもとに、AMD や Intel の公式サイトに掲載されている製品カタログ（一次情報）を参照すれば、CPU の基本仕様を正確に把握できます。製品カタログは検索エンジンで容易に入手可能です。検索時にはモデル名とともに `site:amd.com` や `site:intel.com` といったサイト指定オプションを付け、必ず製造元の公式ページから情報を取得してください。

- [AMD Ryzen Threadripper 3970Xの仕様書](https://www.amd.com/ja/support/downloads/drivers.html/processors/ryzen-threadripper/ryzen-threadripper-3000-series/amd-ryzen-threadripper-3970x.html)（「さらに一般的な仕様」もクリックすること）
- [AMD EPYC™ 7002 tuning guide](https://www.amd.com/content/dam/amd/en/documents/epyc-technical-docs/tuning-guides/amd-epyc-7002-tg-hpc-56827.pdf)
- [AMDのプロセッサの仕様のページ](https://www.amd.com/ja/products/specifications/processors.html)
- [IntelのCore i5-9600の仕様のページ](https://www.intel.co.jp/content/www/jp/ja/products/sku/134900/intel-core-i59600-processor-9m-cache-up-to-4-60-ghz/specifications.html)
-  [AMD CPU仕様 Google検索例](https://www.google.com/search?q=site%3Aamd.com+Ryzen+3970X)
-  [Intel CPU仕様 Google検索例](https://www.google.com/search?q=Intel(R)+Core(TM)+i5-9600+CPU+%40+3.10GHz+site%3Aintel.com)
  
## 3. Ryzen Threadripper 3970X の基本スペック

Ryzen Threadripper 3970X の基本スペックは以下のようになります。

- **コア数**：32 コア  
- **スレッド数**：64 スレッド  
- **基本クロック周波数**：3.7 GHz ターボ時最大4.5GHz  
- **SIMD 命令セット**：AVX2 をサポート（256ビット幅）
- **L1 キャッシュ**：2048 kB (データ32KB/core; 命令32KB/core)
- **L2 キャッシュ**：16 MB (512 KiB/core)
- **L3 キャッシュ**：128 MB (32コアで共有)
- **L1 データキャッシュ内部のポート性能**: 2 回のロード + 1 回のストア / cycle (ロード帯域は 64 バイト/サイクル、ストア帯域は 32 バイト/サイクル)
- **L2 キャッシュ->L1キャッシュ帯域** 32 bytes/cycle

これらの情報は[AMDの公式プロセッサの仕様ページ](https://www.amd.com/ja/support/downloads/drivers.html/processors/ryzen-threadripper/ryzen-threadripper-3000-series/amd-ryzen-threadripper-3970x.html)で確認できます。
ただ、キャッシュの更なる詳細を公式サイトから探すのは難しく、[Wikichips Zen2](https://en.wikichip.org/wiki/amd/microarchitectures/zen_2)や[Wikichips Zen](https://en.wikichip.org/wiki/amd/microarchitectures/zen)で確認しました。Zen 2 世代の L2 キャッシュは、L1/L3 間で「32 bytes per cycle」の帯域を持っています。この 32 B/cycle = 4 double/cycle という値が、大域的な設計パラメータとなります。

## 4. Ryzen Threadripper 3970X の基本スペック詳細

これ以上詳しい仕様を探すのは結構骨がいります。Ryzenシリーズはコンシューマ系なので公式から詳しい情報はあまりでてきません。同じアーキテクチャのZen2のサーバー版である、EPYCで検索するとよいでしょう。
たとえば、
- [AMD EPYC™ 7002 tuning guide](https://www.amd.com/content/dam/amd/en/documents/epyc-technical-docs/tuning-guides/amd-epyc-7002-tg-hpc-56827.pdf)
も役に立ちます。EPYCはRyzenのサーバ向けの強化されたCPUです。Zen2であることは同じです。Chap9のDGEMMの章は特に役に立ちます。

- CPUがどんな構成になっているかは、[AMD Ryzen Threadripper 3960X & 3970X CPU Review](https://www.kitguru.net/components/cpu/luke-hill/amd-ryzen-threadripper-3960x-3970x-cpu-review/2/) が役に立ちます。それによると、Ryzen 3970Xは、CCD(Core Complex Die; 小型ダイ。実際のCPUコアとL3キャッシュを内蔵)が4個あり、各CCDは2個 CCX(Core Complex; CCD内部で4個のコアとL3を共有する)を内包します。CXXには4個コアが存在します。コアごとに 32 KB L1 D、32 KB L1 I、512 KB L2を備え、CCX単位、つまり4コアで16 MB L3を共有します。従って、全体としては16*2*4=128MBのL3キャッシュがあります。

```
CPUパッケージ
├─ I/O Die（12 nm）
└─ CCD (7 nm) × 4
    ├─ CCX 0（4 コア + 16 MB L3）
    └─ CCX 1（4 コア + 16 MB L3）
```

- [ChatGPTで検索](https://chatgpt.com/share/680f60cb-063c-800e-92c4-62f410668525)すると情報をまとめてくれます。

## 5. Ryzen Threadripper 3970X の 1コアあたり 1サイクルでの最大演算回数

CPU の理論演算性能を評価する際には、以下３つの要素を組み合わせて考えます：  
**FMA、SIMD、命令発行の並列度**です。

### 1. FMA 命令

- **FMA (Fused Multiply‑Add)** 命令は、1命令で「乗算１回＋加算１回」を同時に実行します。  
- これにより、同じ命令を使う場合の演算回数が２倍になります（通常であれば乗算と加算でそれぞれ1 FLOPずつ、計2 FLOPですが、FMA命令では1命令で2 FLOPを実行できます）

### 2. SIMD 命令

- **SIMD (Single Instruction Multiple Data)** 命令はSSE4、AVX2、AVX‑512 などの命令セットを用いると、1命令で複数のデータ（レーン）を同時に演算できます。  
- **レーン数** は、1命令あたりに処理できる要素数を示します。

### 3. 命令発行の並列度（パイプライン数）

- 最近の CPUは、同一コア内に複数の FMA/SIMD パイプラインを備えています。  
- これにより、1サイクル内に複数の FMA 命令を同時発行（スループット向上）できます。1サイクル内に同時に実行できるFMA命令の数が命令発行の並列度です。

### Ryzen Threadripper 3970X（Zen 2）の主な仕様

- **FMA パイプライン数**：2本（実行ポート 0 と 1）
- **SIMD レーン数**：256bit（AVX2）  
  
### 演算回数の具体的計算

1. **FMA を使った 1命令あたりの FLOP数**  
   - FMA（Fused Multiply-Add）命令は1命令で乗算と加算を同時に行うため、1命令で2演算となります
   - これを各データ要素に対して適用すると、データ要素数×2が1命令あたりのFLOP数となります

2. **SIMD(AVX2)あたりのレーン数**  
   - 倍精度（64bit）：256 bit ÷ 64 bit = **4 要素**  
   - 単精度（32bit）：256 bit ÷ 32 bit = **8 要素**
   
   - 倍精度では4要素×2演算=**8 FLOP/命令**  
   - 単精度では8要素×2演算=**16 FLOP/命令**となります

3. **サイクルあたりの FLOP数（2本のパイプラインを考慮）**  
   - Ryzen 3970X（Zen 2）のコアには2本のFMAパイプラインがあります
     
   - 倍精度：8 FLOP/命令 × 2 パイプライン ＝ **16 FLOP/サイクル**  
   - 単精度：16 FLOP/命令 × 2 パイプライン ＝ **32 FLOP/サイクル**

> **補足・注意点**  
> - 上記はあくまで**理想的条件**での理論値です。実際には命令依存性やメモリ帯域、分岐予測ミスなどで性能が低下します。  
> - FMA 以外の単独乗算／加算命令では、理論値の半分程度となる場合があります。  
> - 実際の命令セットサポート状況やパイプライン構成の詳細は、AMD 社公式ドキュメントを参照すべきですが、正しい情報を得るのはかなり面倒です。また非常に専門的になっていて、必要な情報がわかりにくい場合もあります。
> - ChatGPTなどで、WebSearchをonにして[下調べ](https://chatgpt.com/share/680467fd-acd8-800e-a567-e7e7f13b9f74)してから[WikiChip](https://fuse.wikichip.org/news/2458/a-look-at-the-amd-zen-2-core/2/) のような解説を読むと正しい情報を得るときの参考資料となります。

## 6. Ryzen Threadripper 3970X の理論 FLOPS 計算例

### (1) 倍精度演算の場合（64 bit）

1. **サイクルあたりの FLOP数**  
   - 1 命令あたり：4 要素 × 2 演算（乗算＋加算）＝ 8 FLOP  
   - FMA パイプライン数：2 本 ⇒ 8 FLOP × 2 = **16 FLOP/サイクル**

2. **理論 FLOPS の式**  

Ryzen 3970Xの倍精度演算の理論性能について、まず1コアから計算した上で全コアの理論性能を導出する流れでまとめます。

## Ryzen 3970Xの倍精度演算（64 bit）理論性能

### 1コアあたりの演算性能の基本計算

#### サイクルあたりのFLOP数
- 1命令あたり：4要素 × 2演算（乗算＋加算）= 8 FLOP
- FMAパイプライン数：2本
- 1サイクルあたり：8 FLOP × 2 = **16 FLOP/サイクル**

#### 1コアの理論FLOPS
理論FLOPSの基本式：
$`\text{FLOPS} = \text{FLOP/cycle} \times \text{Clock Frequency (Hz)}`$

**通常動作時（Base Clock: 3.7 GHz）**
$`16 \times 3.7 \times 10^9 = 59.2 \times 10^9 = 59.2 \text{ GFLOPS}`$

**ターボブースト時（Max Boost: 4.5 GHz）**
$`16 \times 4.5 \times 10^9 = 72 \times 10^9 = 72 \text{ GFLOPS}`$

### 全コア（32コア）の理論性能

理論FLOPSの完全な式：
$`\text{FLOPS} = \text{コア数} \times \text{FLOP/cycle} \times \text{Clock Frequency (Hz)}`$

**通常動作時（3.7 GHz）**
$`32 \times 16 \times 3.7 \times 10^9 = 512 \times 3.7 \times 10^9 = 1.8944 \times 10^{12} \text{ FLOPS} = 1.89 \text{ TFLOPS}`$

**ターボブースト時（4.5 GHz）**
$`32 \times 16 \times 4.5 \times 10^9 = 512 \times 4.5 \times 10^9 = 2.304 \times 10^{12} \text{ FLOPS} = 2.30 \text{ TFLOPS}`$

### まとめ

Ryzen 3970Xの倍精度（binary64）演算での理論性能：

| 動作モード | 1コアあたり | 全32コア | 
|------------|------------|----------|
| ノーマル動作<br>(3.7 GHz) | 59.2 GFLOPS | **1.89 TFLOPS** |
| ターボブースト<br>(4.5 GHz) |  72 GFLOPS| - |

※ 実際のターボブーストは温度やTDPにより変動するため、この値は理論的な最大値となります。

Ryzen 3970Xの単精度演算の理論性能について、まず1コアから計算した上で全コアの理論性能を導出する流れでまとめます。

## Ryzen 3970Xの単精度演算（32 bit）理論性能

### 1コアあたりの演算性能の基本計算

#### サイクルあたりのFLOP数
- 1命令あたり：8要素 × 2演算（乗算＋加算）= 16 FLOP
- FMAパイプライン数：2本
- 1サイクルあたり：16 FLOP × 2 = **32 FLOP/サイクル**

#### 1コアの理論FLOPS
理論FLOPSの基本式：
$`\text{FLOPS} = \text{FLOP/cycle} \times \text{Clock Frequency (Hz)}`$

**通常動作時（Base Clock: 3.7 GHz）**
$`32 \times 3.7 \times 10^9 = 118.4 \times 10^9 = 118.4 \text{ GFLOPS}`$

**ターボブースト時（Max Boost: 4.5 GHz）**
$`32 \times 4.5 \times 10^9 = 144 \times 10^9 = 144 \text{ GFLOPS}`$

### 全コア（32コア）の理論性能

理論FLOPSの完全な式：
$`\text{FLOPS} = \text{コア数} \times \text{FLOP/cycle} \times \text{Clock Frequency (Hz)}`$

**通常動作時（3.7 GHz）**
$`32 \times 32 \times 3.7 \times 10^9 = 1024 \times 3.7 \times 10^9 = 3.7888 \times 10^{12} \text{ FLOPS} = 3.79 \text{ TFLOPS}`$

**ターボブースト時（4.5 GHz）**
$`32 \times 32 \times 4.5 \times 10^9 = 1024 \times 4.5 \times 10^9 = 4.608 \times 10^{12} \text{ FLOPS} = 4.61 \text{ TFLOPS}`$

### まとめ

Ryzen 3970Xの単精度（binary32）演算での理論性能：

| 動作モード | 1コアあたり | 全32コア | 
|------------|------------|----------|
| ノーマル動作<br>(3.7 GHz) | 118.4 GFLOPS | **3.79 TFLOPS** |
| ターボブースト<br>(4.5 GHz) | 144.0 GFLOPS | - |

※ 実際のターボブーストは温度やTDPにより変動するため、この値は理論的な最大値となります。

## 7. 注意点と実際の性能

- **周波数の違い**：実際にはターボブーストや省電力状態などで動作周波数は変動します。また、全てのコアが同時に最高周波数で動作する状況は非常に稀です。
- **命令スケジューリングやデータ供給**：理論値は全ての演算ユニットが常にデータを受け取り、フル活用される前提ですが、現実のプログラムではメモリ帯域やキャッシュミスなどの影響でこの理想状態は達成できません。
- **SIMD の活用**：すべてのコードが AVX2 命令を効果的に使えるわけではなく、汎用的なコードではこれほどの FLOPS は発揮できないことが多いです。

## 8. まとめ

Ryzen Threadripper 3970X の理論 FLOPS は、**倍精度**で約 **1.89 TFLOPS**、**単精度**で約 **3.79 TFLOPS** と計算できます。  
これらの値は、CPU 内のすべての演算ユニットが理想的な条件下でフル稼働した場合の理論上の最大値です。
実際の性能は、使用するアルゴリズムやコードの最適化の度合い、さらにクロックの状況、システム全体のボトルネックなどにより大きく左右されます。

このように、理論性能の計算は非常にシンプルな数式で表せますが、実際にその性能を引き出すのはまた別の技術と工夫が必要となります。  
CPU や GPU の性能評価を行う際は、ベンチマークテストなど実際の計測も重要な情報となります。

