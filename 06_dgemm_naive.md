# 最も簡単なDGEMM実装 (NN版)

行列積（Matrix Multiplication）は線形代数において基本的な演算であり、科学計算、機械学習、グラフィックスなど様々な分野で利用されています。DGEMM（Double precision General Matrix Multiplication）はBLAS（Basic Linear Algebra Subprograms）で定義されている関数の一つで、倍精度浮動小数点数を使った一般的な行列積を計算します。

## 実装の説明

以下に、最も単純なDGEMM実装を示します。この実装では、行列AとBが転置されていない（NN: Not transposed）場合のみを考慮しています。

```c++
#include <iostream>
#include <algorithm>

// 最も単純なDGEMM実装（NN版のみ）
void dgemm_simple_nn(int m, int n, int k, double alpha, const double *A, int lda,
                     const double *B, int ldb, double beta, double *C, int ldc) {
    // m: 行列Cの行数
    // n: 行列Cの列数
    // k: 行列Aの列数 = 行列Bの行数
    // alpha, beta: スカラー係数
    // A: m×k行列
    // B: k×n行列
    // C: m×n行列
    // lda, ldb, ldc: 各行列の先頭次元（leading dimension）

    // パラメータチェック
    if (m < 0 || n < 0 || k < 0) {
        std::cerr << "Error: Negative dimensions are not allowed." << std::endl;
        return;
    }
    if (lda < std::max(1, m) || ldb < std::max(1, k) || ldc < std::max(1, m)) {
        std::cerr << "Error: Invalid leading dimensions." << std::endl;
        return;
    }

    // 簡単なケースの処理
    if (m == 0 || n == 0 || ((alpha == 0.0 || k == 0) && beta == 1.0)) {
        return;  // 何もしない
    }

    // alpha == 0の場合
    if (alpha == 0.0) {
        if (beta == 0.0) {
            // C = 0
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = 0.0;
                }
            }
        } else {
            // C = beta * C
            for (int j = 0; j < n; j++) {
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] = beta * C[i + j * ldc];
                }
            }
        }
        return;
    }

    // メインの行列積計算: C = alpha * A * B + beta * C
    for (int j = 0; j < n; j++) {
        // betaによるCの初期化
        if (beta == 0.0) {
            for (int i = 0; i < m; i++) {
                C[i + j * ldc] = 0.0;
            }
        } else if (beta != 1.0) {
            for (int i = 0; i < m; i++) {
                C[i + j * ldc] = beta * C[i + j * ldc];
            }
        }

        // 行列積の計算
        for (int l = 0; l < k; l++) {
            if (B[l + j * ldb] != 0.0) {
                double temp = alpha * B[l + j * ldb];
                for (int i = 0; i < m; i++) {
                    C[i + j * ldc] += temp * A[i + l * lda];
                }
            }
        }
    }
}
```

## 実装の詳細解説

この実装は以下の特徴を持ちます：

1. **シンプルさ**: 最も基本的なトリプルループ構造を使用しています。
2. **制限**: 転置なし（NN）のケースのみを扱います。
3. **特殊ケースの処理**: 計算効率を上げるために特殊ケースを事前に処理します。

### パラメータの説明

- `m`, `n`, `k`: 行列の次元
  - `m`: 行列Cの行数 = 行列Aの行数
  - `n`: 行列Cの列数 = 行列Bの列数
  - `k`: 行列Aの列数 = 行列Bの行数
- `alpha`, `beta`: スカラー係数
- `A`: m×k行列
- `B`: k×n行列
- `C`: m×n行列
- `lda`, `ldb`, `ldc`: 各行列のメモリ配置における行の長さ（leading dimension）

### アルゴリズムの説明

この実装は数学的に以下の操作を行います：
```
C = alpha * A * B + beta * C
```

具体的なステップは以下の通りです：

1. **パラメータ検証**: 次元やleading dimensionが有効であることを確認します。
2. **特殊ケースの処理**:
   - 計算結果に影響がない場合（m=0, n=0など）は早期リターン
   - alpha=0の場合、行列積の計算は不要で、betaによるCの更新のみ行う
3. **行列積の計算**:
   - Cの各列ごとに処理
   - betaに応じてCを初期化
   - 行列AとBの対応する要素を掛け合わせて累積

### 最適化のポイント

基本実装ながらも、以下の点で効率化を図っています：

1. **ゼロチェック**: B[l + j * ldb]が0の場合、内側のループをスキップ
2. **特殊ケースの分離**: alpha=0やbeta=1などの特殊ケースを別途処理
3. **行列アクセスの最適化**: 列優先メモリレイアウトに合わせたアクセスパターン

## 使用例

```c++
#include <iostream>
#include <vector>

int main() {
    const int m = 3, n = 2, k = 4;
    
    // 行列Aの初期化（3×4）
    std::vector<double> A = {
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0
    };
    
    // 行列Bの初期化（4×2）
    std::vector<double> B = {
        1.0, 2.0,
        3.0, 4.0,
        5.0, 6.0,
        7.0, 8.0
    };
    
    // 行列Cの初期化（3×2）
    std::vector<double> C(m * n, 0.0);
    
    // DGEMM実行
    double alpha = 1.0, beta = 0.0;
    dgemm_simple_nn(m, n, k, alpha, A.data(), m, B.data(), k, beta, C.data(), m);
    
    // 結果の表示
    std::cout << "Result matrix C:" << std::endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << C[i + j * m] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

## パフォーマンスについて

この実装は教育目的としては適していますが、実際の大規模な行列計算では最適化された実装（BLAS実装など）を使用すべきです。以下の点で最適化の余地があります：

1. **キャッシュ効率**: ブロック化技術によりキャッシュ利用を改善
2. **SIMD命令**: ベクトル化によるパラレル計算
3. **スレッド並列化**: マルチコアCPUの活用
4. **メモリアクセスパターン**: データ局所性の向上

## 結論

この実装は最も基本的なDGEMM（NN版）です。シンプルながらも特殊ケースの処理や効率化の基本的な考え方を取り入れています。より高度なパフォーマンスが必要な場合は、OpenBLASやIntel MKLなどの最適化されたライブラリの使用を検討してください。
