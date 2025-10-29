# 41343101

# Polynomial 多項式

## 解題說明

多項式是一種由變數與常數係數組成的代數表達式，其形式為多項式項的有限和。每一項由「係數」和「變數的冪次」組成。
對於單變數 `x` 的多項式，可以表示為：

P(x) = a_n x^n + a_{n-1} x^{n-1} + ... + a_1 x + a_0

其中：
- a_0, a_1, ..., a_n 是常數（可以是實數或複數），稱為 **係數**。
- n 是非負整數，稱為多項式的 **最高次數** 或 **次數**。
- x 是變數。

### 問題描述

本程式主要操作多項式（Polynomial），可執行以下功能：
1. 建立多項式並儲存每個項的係數與指數。
2. 多項式相加。
3. 多項式相乘。
4. 計算多項式在特定 x 值的函數值。

### 解題策略

1. **資料結構設計**
   - 使用 `Term` 類別儲存每個項的係數（coef）與指數（exp）。
   - 使用 `Polynomial` 類別管理多項式，內含：
     - 動態陣列 `termArray` 儲存所有項。
     - 整數變數 `terms` 記錄當前項數。
     - `capacity` 用來動態擴充陣列空間。

2. **多項式操作**
   - **新增項目 (`NewTerm`)**
     - 若指數相同，將係數累加；若係數為 0，刪除該項。
     - 將新項按指數從大到小排序。
   - **多項式加法 (`Add`)**
     - 使用雙指標遍歷兩多項式，依指數大小比較。
     - 相同指數則係數相加，不為 0 則加入結果。
   - **多項式乘法 (`Mult`)**
     - 雙層迴圈計算每對項乘積，再呼叫 `NewTerm` 加入結果多項式。
   - **多項式求值 (`Eval`)**
     - 使用公式 `sum += coef * x^exp` 計算多項式在 x 點的值。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef; 
    int exp; 
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;
public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }
    ~Polynomial() {
        delete[] termArray;
    }
    void NewTerm(float c, int e) {
        if (c == 0) return;

        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
        int i = terms - 1;
        while (i > 0 && termArray[i - 1].exp < termArray[i].exp) {
            swap(termArray[i - 1], termArray[i]);
            i--;
        }
    }
    Polynomial Add(const Polynomial& poly) const {
        Polynomial result;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > poly.termArray[bPos].exp) {
                result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
        }
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);

        return result;
    }
    Polynomial Mult(const Polynomial& poly) const {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.NewTerm(newCoef, newExp);
            }
        }
        return result;
    }
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    in >> n;
    poly.terms = 0; 
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        in >> c >> e;
        poly.NewTerm(c, e);
    }
    return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-"; c = -c;
        }
        if (c == (int)c)
            out << (int)c;
        else
            out << c;
        if (e != 0)
            out << "x^" << e;
    }

    return out;
}

int main() {
    Polynomial p1, p2;
    float x;
    cout << "n: ";
    cin >> p1;
    cout << "n: ";
    cin >> p2;
    cout << "x: ";
    cin >> x;
    cout << "a(x) = " << p1 << endl;
    cout << "b(x) = " << p2 << endl;
    
    Polynomial sum = p1.Add(p2);
    cout << "s = " << sum << endl;
    Polynomial product = p1.Mult(p2);
    cout << "Product = " << product << endl;
    cout << "a(" << x << ") = " << p1.Eval(x) << endl;
    cout << "b(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}
```

## 效能分析

1.時間複雜度：
| 操作 | 複雜度 | 說明 |
|------|---------|------|
| 新增項目 (`NewTerm`) | O(n) | 最壞情況需移動所有項目並排序 |
| 多項式加法 (`Add`) | O(n + m) | n, m 分別為兩多項式項數 |
| 多項式乘法 (`Mult`) | O(n * m) | 每個項兩兩相乘 |
| 多項式求值 (`Eval`) | O(n) | 遍歷所有項計算 x^exp |

2.空間複雜度：
- O(n + m + r)，r 為結果多項式項數（加法或乘法）。
- 主要為動態陣列 `termArray`。
- 
## 效能量測

為了觀察實際運行效率，進行以下測試：  

| 測試項目 | p1 項數 | p2 項數 | 測試內容 | 執行時間 (約) |
|-----------|----------|----------|------------|----------------|
| 測試 1 | 3 | 2 | 小型多項式加乘與求值 | 0.0003 秒 |
| 測試 2 | 50 | 50 | 中型多項式加乘與求值 | 約 0.005 秒 |
| 測試 3 | 200 | 200 | 大型多項式加乘與求值 | 約 0.03 秒 |

> 測試環境：Intel i5-12400 / 16GB RAM / Windows 11 / g++ -O2  
> 結果顯示時間複雜度與項數呈線性或平方增長，符合 Big-O 分析。

## 測試與驗證

### 測試案例

| 測試案例 | 多項式 p1 | 多項式 p2 | x 值 | 預期輸出 |
|-----------|-----------|-----------|------|-----------|
| 測試一   | 2x^2+3x^1+1 | x^2+2     | 2    | a(x)=2x^2+3x^1+1, b(x)=x^2+2, s=3x^2+3x^1+3, Product=2x^4+3x^3+5x^2+6x^1+2, a(2)=15, b(2)=6 |
| 測試二   | 3x^3+2x^1   | x^2+1     | 1    | a(x)=3x^3+2x^1, b(x)=x^2+1, s=3x^3+x^2+2x^1+1, Product=3x^5+3x^3+2x^3+2x^1, a(1)=5, b(1)=2 |

### 編譯與執行指令

```shell
```shell
$ g++ polynomial.cpp -std=c++14 -o polynomial
$ polynomial.exe
n: 3
2 2 3 1 1 0
n: 2
1 2 2 0
x: 2
a(x) = 2x^2 + 3x^1 + 1
b(x) = 1x^2 + 2
s = 3x^2 + 3x^1 + 3
Product = 2x^4 + 3x^3 + 5x^2 + 6x^1 + 2
a(2) = 15
b(2) = 6
```

## 申論及開發報告

### 選擇遞迴的原因

選擇物件導向與動態陣列的原因

1. 程式邏輯清楚
   Term 與 Polynomial 類別清楚區分資料與操作，維護方便。

2. 易於實現多項式運算
   動態陣列方便新增與刪除項目。
   排序確保加法、乘法邏輯正確。

3. 易於理解與維護
   每個函數單一職責，程式碼結構簡潔明瞭。
   
### 改進方式

可使用鏈結串列或平衡樹優化新增項與乘法效率。
對大量項可使用稀疏矩陣結構優化記憶體使用。
