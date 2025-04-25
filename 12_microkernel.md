# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

## 4. サイズ候補と評価（Zen 2）

| 候補 # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

m_r\times n_r`$ | レジスタ消費 (# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

\overline{C}`$) | (★) の満足度 | 実用コメント |
|----------------------|---------------|------------|------------|
| 4 × 4                | 4 YMM (25 %)   | ◎          | 教材・移植性に優れています |
| 6 × 6                | 9 YMM (56 %)   | ◎          | 計算密度が向上し，TLB/帯域がボトルネックになりにくいです |
| 6 × 8                | 12 YMM (75 %)  | ◎          | BLIS が採用しています。プリフェッチの残り枠がぎりぎりです |
| 8 × 8                | 16 YMM (100 %) | ○          | レジスタが逼迫します。ロード先行命令を工夫すれば可能です |

**推奨事項** : **4 × 4** でまず実装し，プロファイル結果を確認した上で **6 × 6** へ拡張するのが安全です。

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
* **レジスタ半分ルール** と **(★) ロード隠蔽条件** が マイクロカーネルサイズ選定の基本指針となります。
* Zen 2 アーキテクチャでは **# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

## 4. サイズ候補と評価（Zen 2）

| 候補 # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

m_r\times n_r`$ | レジスタ消費 (# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

## 3. "ロード隠蔽" 条件
演算 1 回に要する時間で **次のデータを L2→レジスタへ運ぶ** 必要があります。Roofline 風に表しますと

```math
$ n_r \;\ge\; \frac{R_\text{comp}}{2\,R_\text{load}}\tag{★} $
```

* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}`$ … 1 コアあたり倍精度 FLOP/cycle
* # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}`$ … L2→レジスタのロード帯域（double/cycle）
* 分母の "2" は 1 回の rank-1 update に **A,B ⇒ 合計 ≈2 ロード** が必要なためです

### Zen 2 プロセッサの特性
* **計算能力** : 2 × 256-bit FMA/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{comp}=16\;\text{FLOP/cycle}`$
* **ロード帯域** : L2→L1 32 B/clk = 4 double/clk → # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

R_\text{load}=4\;\text{double/cycle}`$

したがいまして
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

## 2. レジスタ制約
* Zen 2 では 256-bit YMM レジスタが **16 本／コア** あります
* 通例 **半数を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{C}`$ の保持** に使用し，残りで *# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{A}`$ と # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

\overline{B}`$ のプリフェッチ*＋*ループ変数* を処理します
# 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

## 1. 概観
最内ループ（マイクロカーネル）では # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C_{\overline{m_r\times n_r}}\;+\!=\;A_{\overline{m_r\times1}}\;B_{\overline{1\times n_r}}`$ という **rank-1 update** を # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

k`$ 回繰り返して # 6.2 bis ― マイクロカーネル (m_r\times n_r`$) の最適選定

C`$ を構築いたします。性能は **(i) レジスタ数** と **(ii) 演算／ロード重畳** のバランスで決まります。

m_r\times n_r \;\le\; \tfrac{1}{2}\times (レジスタ本数) \quad\Longrightarrow\quad m_r\times n_r \le 8`$ 

n_r \;\ge\; \frac{16}{2\times4}=2`$ 

**列幅 2 以上** であれば、理論上ロード遅延は完全に隠蔽できます。

\overline{C}`$) | (★) の満足度 | 実用コメント |
|----------------------|---------------|------------|------------|
| 4 × 4                | 4 YMM (25 %)   | ◎          | 教材・移植性に優れています |
| 6 × 6                | 9 YMM (56 %)   | ◎          | 計算密度が向上し，TLB/帯域がボトルネックになりにくいです |
| 6 × 8                | 12 YMM (75 %)  | ◎          | BLIS が採用しています。プリフェッチの残り枠がぎりぎりです |
| 8 × 8                | 16 YMM (100 %) | ○          | レジスタが逼迫します。ロード先行命令を工夫すれば可能です |

**推奨事項** : **4 × 4** でまず実装し，プロファイル結果を確認した上で **6 × 6** へ拡張するのが安全です。

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

n_r\ge2`$** を満たせば帯域律速を回避できます。
* 実際の測定では **4 × 4 → 6 × 6/8** へ段階的に拡大しながら、レジスタ逼迫とロードポート飽和のトレードオフを調整していくことをお勧めします。
