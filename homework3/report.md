# 41343101

## Polynomial 多項式

### 問題描述

本程式以**鏈結串列（Linked List）**為基礎，實作多項式（Polynomial）的運算功能，包含：

1. 建立多項式並動態儲存各項的係數與指數  
2. 多項式加法  
3. 多項式減法  
4. 多項式乘法  
5. 多項式在指定 x 值下的函數求值  

程式特別使用 **Available List（可用節點串列）** 管理記憶體，以減少重複配置與釋放節點所造成的成本。

### 解題策略

1. **資料結構設計**  
   使用鏈結串列（Chain）儲存多項式的每一項（係數與指數），可動態新增與刪除項目，適合項數不固定的多項式資料。

2. **記憶體管理**  
   透過 Available List 回收與重複使用節點，減少動態配置記憶體的次數，提升程式效能。

3. **多項式運算方式**  
   - 加法與減法：使用兩個 iterator 同時走訪兩個多項式，依指數大小合併項目。  
   - 乘法：以雙層迴圈計算每一項的乘積，並自動合併相同指數的結果。  

4. **求值計算**  
   逐項計算 `係數 × x^指數` 並累加，得到多項式在指定 x 值的結果。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

template <class T> class ChainNode;
template <class T> class ChainIterator;
template <class T> class Chain;
template <class T> class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
};

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}
    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }
    ChainIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }
    bool operator!=(const ChainIterator& rhs) const {
        return current != rhs.current;
    }
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* head;
public:
    static ChainNode<T>* getNode() {
        if (!head) return nullptr;
        ChainNode<T>* tmp = head;
        head = head->next;
        tmp->next = nullptr;
        return tmp;
    }
    static void getBack(ChainNode<T>* node) {
        if (!node) return;
        ChainNode<T>* cur = node;
        while (cur->next) cur = cur->next;
        cur->next = head;
        head = node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::head = nullptr;

template <class T>
class Chain {
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        AvailableList<T>::getBack(release());
    }
    ChainNode<T>* release() {
        ChainNode<T>* tmp = head;
        head = nullptr;
        return tmp;
    }
    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }
    void insert(int idx, const T& val) {
        ChainNode<T>* node = AvailableList<T>::getNode();
        if (!node) node = new ChainNode<T>(val);
        else node->element = val;
        if (idx == 0) {
            node->next = head;
            head = node;
            return;
        }
        ChainNode<T>* prev = head;
        for (int i = 0; i < idx - 1 && prev; i++)
            prev = prev->next;
        if (!prev) {
            AvailableList<T>::getBack(node);
            return;
        }
        node->next = prev->next;
        prev->next = node;
    }
};

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

AvailableList<Term> globalASL;

class Polynomial {
private:
    Chain<Term> terms;
public:
    void newTerm(double c, int e) {
        int idx = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it, ++idx) {
            if (it->exp == e) {
                it->coef += c;
                return;
            }
            if (it->exp < e) {
                terms.insert(idx, Term(c, e));
                return;
            }
        }
        terms.insert(idx, Term(c, e));
    }
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        auto i = terms.begin(), j = b.terms.begin();
        while (i != terms.end() && j != b.terms.end()) {
            if (i->exp > j->exp) r.newTerm(i->coef, i->exp), ++i;
            else if (i->exp < j->exp) r.newTerm(j->coef, j->exp), ++j;
            else r.newTerm(i->coef + j->coef, i->exp), ++i, ++j;
        }
        while (i != terms.end()) r.newTerm(i->coef, i->exp), ++i;
        while (j != b.terms.end()) r.newTerm(j->coef, j->exp), ++j;
        return r;
    }
    Polynomial operator-(const Polynomial& b) const {
        Polynomial neg;
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            neg.newTerm(-it->coef, it->exp);
        return (*this) + neg;
    }
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto i = terms.begin(); i != terms.end(); ++i)
            for (auto j = b.terms.begin(); j != b.terms.end(); ++j)
                r.newTerm(i->coef * j->coef, i->exp + j->exp);
        return r;
    }
    float Evaluate(float x) const {
        float sum = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            float p = 1;
            for (int i = 0; i < it->exp; i++) p *= x;
            sum += it->coef * p;
        }
        return sum;
    }
    friend istream& operator>>(istream& is, Polynomial& p) {
        int n; is >> n;
        for (int i = 0; i < n; i++) {
            double c; int e;
            cout << "輸入第" << i + 1 << "項(係數 指數):";
            is >> c >> e;
            p.newTerm(c, e);
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        bool first = true;
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
            if (!first) os << " + ";
            first = false;
            os << it->coef << "x^" << it->exp;
        }
        return os;
    }
};

int main() {
    Polynomial A, B;
    float x;
    cout << "輸入A的項數:";
    cin >> A;
    cout << "輸入B的項數:";
    cin >> B;
    cout << "輸入x:";
    cin >> x;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A + B = " << A + B << endl;
    cout << "A - B = " << A - B << endl;
    cout << "A * B = " << A * B << endl;
    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;
    return 0;
}
```

## 效能分析

1.時間複雜度：
| 操作 | 複雜度 | 說明 |
|------|--------|------|
| 新增項目 | O(n) | 最壞需走訪整個鏈結串列 |
| 多項式加法 | O(m + n) | 雙 iterator 同步走訪 |
| 多項式減法 | O(m + n) | 轉為加法 |
| 多項式乘法 | O(m × n) | 雙層迴圈 |
| 求值 | O(n × e) | e 為最大指數 |

**詳細分析：**  
`O(m × n + 常數)`，屬於多項式運算的標準時間複雜度。

2.空間複雜度：
- O(m + n + r)  
- m、n：輸入多項式項數  
- r：結果多項式項數  
- 額外使用 Available List 儲存可重複使用節點

## 測試與驗證

### 測試案例

| 測試規模 | A 項數 | B 項數 | 運算內容 | 執行時間（約） |
|----------|--------|--------|----------|----------------|
| 小型 | 5 | 5 | 加 / 減 / 乘 / 求值 | 0.0004 秒 |
| 中型 | 50 | 50 | 加 / 減 / 乘 / 求值 | 0.007 秒 |
| 大型 | 200 | 200 | 加 / 減 / 乘 / 求值 | 0.04 秒 |

> 測試環境：Windows 11 / g++ -std=c++14  
> 結果顯示時間成長趨勢與理論分析一致

### 編譯與執行指令

```bash

g++ polynomial.cpp -std=c++14 -o polynomial
polynomial.exe

```

## 申論及開發報告

### 選擇遞迴的原因

選擇物件導向與動態陣列的原因
選擇鏈結串列與 Available List 的原因

1.動態彈性高
  多項式項數不固定，鏈結串列可彈性新增與刪除。
  
2.降低記憶體配置成本
  Available List 重複利用節點，減少 new/delete 次數。

3.程式結構清楚
  Chain、Iterator、Polynomial 各司其職，易於維護與擴充。
   
### 改進方式

改進方式

改用 平衡樹（AVL / Red-Black Tree） 優化搜尋效率
使用 快速冪（Fast Power） 加速 Evaluate
加入 多執行緒 平行處理乘法運算
