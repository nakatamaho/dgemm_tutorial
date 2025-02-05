# 05 DGEMMの結果のプリントとチェック

本章では、実装した DGEMM の計算結果をコンソールにプリントし、正しさを確認する方法について解説します。計算結果が期待通りになっているかを検証するため、以下の2点を行います。

1. **結果のプリント**  
   計算後の行列 \( C \) を見やすい形式で出力し、実際にどのような値が得られているかを確認します。

2. **結果のチェック**  
   事前に用意した正解となる行列（リファレンス）と計算結果の行列を比較し、各要素の差が許容範囲内に収まっているかを検証します。

以下は、C++ を用いたサンプルコードです。ここでは、行列の表示および結果チェック用の関数を定義しています。

```cpp
#include <iostream>
#include <vector>
#include <cmath>

// 行列を表現するための型定義（2次元ベクトル）
using Matrix = std::vector<std::vector<double>>;

// 行列をプリントする関数
void printMatrix(const Matrix &mat) {
    for (const auto &row : mat) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

// 計算結果とリファレンスとの誤差をチェックする関数
// tolerance: 許容誤差（デフォルトは 1e-9）
bool checkResult(const Matrix &C, const Matrix &C_ref, double tolerance = 1e-9) {
    int m = C.size();
    int n = C[0].size();
    bool correct = true;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double diff = std::fabs(C[i][j] - C_ref[i][j]);
            if (diff > tolerance) {
                std::cerr << "Mismatch at (" << i << ", " << j << "): "
                          << "Computed = " << C[i][j] << ", Expected = " << C_ref[i][j]
                          << ", Diff = " << diff << "\n";
                correct = false;
            }
        }
    }
    return correct;
}

int main() {
    // サンプルの行列サイズ
    int m = 3, n = 3, k = 3;

    // サンプルとして行列 A, B, C を初期化
    Matrix A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    Matrix B = {
        {9.0, 8.0, 7.0},
        {6.0, 5.0, 4.0},
        {3.0, 2.0, 1.0}
    };

    // 結果を格納する行列 C をゼロで初期化
    Matrix C = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    // リファレンスとなる正しい結果の行列（事前に計算済みのもの）
    // ここでは便宜上、手計算もしくは信頼性のあるライブラリの結果を想定
    Matrix C_ref = {
        { 30.0,  24.0,  18.0},
        { 84.0,  69.0,  54.0},
        {138.0, 114.0,  90.0}
    };

    // スカラー値
    double alpha = 1.0;
    double beta  = 0.0;

    // ここで、実際に DGEMM の関数を呼び出して C を計算する
    // 例えば、dgemm() 関数を呼び出すと仮定
    // dgemm(m, n, k, alpha, &A[0][0], m, &B[0][0], k, beta, &C[0][0], m);

    // サンプルとして、上記のリファレンスと同じ計算結果を代入しておく
    C = C_ref;  // ※ 実際には計算結果がここに格納される

    // 結果のプリント
    std::cout << "Computed Matrix C:\n";
    printMatrix(C);

    // 結果のチェック
    if (checkResult(C, C_ref)) {
        std::cout << "Result is correct within the tolerance.\n";
    } else {
        std::cerr << "Result does not match the reference!\n";
    }

    return 0;
}
```

### 説明

- **printMatrix 関数**  
  行列の各要素をスペース区切りで出力し、行ごとに改行して表示します。これにより、計算結果の行列 \(C\) を簡単に確認できます。

- **checkResult 関数**  
  計算結果の行列 \(C\) とリファレンス行列 \(C_{ref}\) の各要素を比較します。差分が指定した許容誤差 (`tolerance`) を超える場合、エラーメッセージを出力し、全体として正しくないと判断します。

- **main 関数**  
  サンプルとして行列 \(A\)、\(B\)、\(C\) およびリファレンス行列 \(C_{ref}\) を初期化し、実際の DGEMM 関数（ここでは仮想的に `dgemm()` と記述）を呼び出して計算結果を得た後、結果のプリントとチェックを行っています。

このように、結果を出力して比較することで、実装した DGEMM が正しく動作しているかどうかを効率的に検証できます。  
