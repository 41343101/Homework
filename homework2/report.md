# 41343101

# Polynomial 多項式

## 解題說明

多項式由多個項（Term）以加法或減法組成，每個項由 係數（coefficient) 與 變數的指數（exponent) 組成。

一般形式：
P(x) = a_n x^n + a_{n-1} x^{n-1} + \dots + a_1 x + a_0

- \(a_i\)：係數（整數、浮點數等）  
- \(x\)：變數  
- \(n\)：多項式的最高次數（degree）

### 問題描述

本題要求計算 Ackermann 函數 A(m, n) 的值。Ackermann 函數是一個經典的遞迴函數，定義如下：
若 m = 0，則回傳 n + 1。
若 n = 0，則回傳 A(m - 1, 1)。
否則回傳 A(m - 1, A(m, n - 1))。

### 解題策略

利用遞迴方式直接依據定義實作函數 ack(m, n)。
主程式讀入兩個整數 m、n，呼叫 ack(m, n) 計算並輸出結果。
由於 Ackermann 函數的遞迴深度非常大，只適合處理小的輸入值，以避免堆疊溢位。

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
Ackermann 函數是一個極度遞迴的函數，其時間複雜度隨 m、n 增長極快。
對小 m（例如 m=0、1、2）可以近似用 O(n) 或 O(2^n) 表示。
對一般 m、n，時間複雜度大約 O(A(m, n))，因為函數本身的遞迴深度與呼叫次數會呈現 超指數增長。
總結：時間複雜度非常高，對大輸入幾乎不可行。

2.空間複雜度：
由於使用純遞迴，每次函數呼叫都會佔用堆疊空間，堆疊深度等於遞迴層數。
空間複雜度大約與時間複雜度同階：O(A(m, n))。
對小輸入可以忽略，但輸入稍大就可能導致 stack overflow。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 m | 輸入參數 n | 預期輸出 | 實際輸出 |
|-----------|------------|------------|-----------|-----------|
| 測試一   | 0          | 0          | 1         | 1         |
| 測試二   | 1          | 3          | 5         | 5         |
| 測試三   | 2          | 3          | 9         | 9         |
| 測試四   | 3          | 4          | 125       | 125       |
| 測試五   | 4          | 1          | 65533     | 異常拋出 (stack overflow) |

> **備註**：Ackermann 函數對 m ≥ 4 時，容易造成堆疊溢位（stack overflow），因此實際執行可能拋出異常。

### 編譯與執行指令

```shell
$ g++ ack.cpp -std=c++14 -o ack (Visual Studio 2022 為C+14)
$ ack.exe
A(m,n)
輸入 m 值:
(輸入)2
輸入 n 值:
(輸入)3
(結果): 9
```

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算 Ackermann 函數的主要原因如下：

1. 程式邏輯簡單直觀
   遞迴能清楚表達「將問題拆解為更小的子問題」的概念，直到 m 或 n 達到基準條件返回結果。

2. 易於理解與實現  
   遞迴程式碼接近數學公式表示方式，直觀且適合展示遞迴基本概念。

3. 遞迴語意清楚  
   每次呼叫代表一個子問題的解，最終結果逐層返回，完成整體問題求解，無需額外變數維護中間狀態。  
### 改進方式

為了避免遞迴造成 Stack Overflow ，可考慮：
- 使用 迭代（Iteration) 或 尾遞迴（Tail Recursion） 優化。  
- 對較大輸入值採用 記憶化（Memoization) 或 動態規劃（Dynamic Programming） 儲存中間結果，減少重複計算。
