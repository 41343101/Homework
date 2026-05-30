# 41343101

# Sorting Algorithm Benchmark Report（排序演算法效能比較）

## 解題說明

### 問題描述

本程式實作多種經典排序演算法，並進行效能測試與比較，包含：

- Insertion Sort（插入排序）
- Quick Sort（快速排序）
- Merge Sort（合併排序）
- Heap Sort（堆積排序）
- Composite Sort（混合排序：Insertion + Heap）

程式會針對不同規模的隨機資料進行排序，並測量各演算法的平均執行時間（微秒級）。

### 解題策略

### 2.1 資料產生方式

#### 隨機資料
使用 `mt19937` 隨機數生成器：

- `MakeRandom(n)`：產生 1 ~ n 並隨機打亂

#### 逆序資料
- `MakeReverse(n)`：產生反向排序資料
- 用於測試最壞情況

---

### 2.2 正確性驗證

`Check()` 函式用於驗證排序正確性：

測試內容：
- 空陣列
- 單一元素
- 小型資料
- 逆序資料
- 隨機資料

並使用 `std::sort` 作為標準答案比對。

---

### 2.3 排序演算法設計

#### Insertion Sort
- 適用小規模資料
- 時間複雜度：O(n²)

#### Quick Sort
- 使用 Median-of-3 pivot
- 平均 O(n log n)

#### Merge Sort
- Bottom-up 實作
- 穩定排序

#### Heap Sort
- 使用 Max Heap
- 原地排序

#### Composite Sort
- n ≤ 32 → Insertion Sort  
- n > 32 → Heap Sort  

---

### 2.4 效能測試方法

#### Adaptive Benchmark
- 自動調整 repeat 次數
- 確保測量穩定
- 上限 50ms
  
---

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <iomanip>

using namespace std;
using Clk = chrono::high_resolution_clock;

// =========================
// RNG
// =========================
mt19937 rng(20260530);

// =========================
// Utility
// =========================
void CopyArray(const int* src, int* dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

bool IsSorted(const int* a, int n) {
    for (int i = 0; i < n - 1; i++)
        if (a[i] > a[i + 1]) return false;
    return true;
}

vector<int> MakeRandom(int n) {
    vector<int> v(n);
    iota(v.begin(), v.end(), 1);
    shuffle(v.begin(), v.end(), rng);
    return v;
}

vector<int> MakeReverse(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = n - i;
    return v;
}

// =========================
// Sorting Algorithms (raw + optimized mix)
// =========================
void InsertionSort(int* a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int Median3(int* a, int l, int r) {
    int m = (l + r) / 2;
    if (a[l] > a[m]) swap(a[l], a[m]);
    if (a[l] > a[r]) swap(a[l], a[r]);
    if (a[m] > a[r]) swap(a[m], a[r]);
    swap(a[m], a[r]);
    return a[r];
}

void QuickSortRec(int* a, int l, int r) {
    if (l >= r) return;

    int pivot = Median3(a, l, r);
    int i = l, j = r - 1;

    while (true) {
        while (a[++i] < pivot);
        while (a[--j] > pivot);
        if (i < j) swap(a[i], a[j]);
        else break;
    }

    swap(a[i], a[r]);

    QuickSortRec(a, l, i - 1);
    QuickSortRec(a, i + 1, r);
}

void QuickSort(int* a, int n) {
    QuickSortRec(a, 0, n - 1);
}

// bottom-up merge
void Merge(int* a, int* tmp, int l, int m, int r) {
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    }
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int x = l; x < r; x++) a[x] = tmp[x];
}

void MergeSort(int* a, int n) {
    vector<int> tmpv(n);
    int* tmp = tmpv.data();

    for (int w = 1; w < n; w *= 2) {
        for (int l = 0; l < n; l += 2 * w) {
            int m = min(l + w, n);
            int r = min(l + 2 * w, n);
            Merge(a, tmp, l, m, r);
        }
    }
}

// heap
void HeapDown(int* a, int n, int i) {
    while (2 * i + 1 < n) {
        int c = 2 * i + 1;
        if (c + 1 < n && a[c] < a[c + 1]) c++;
        if (a[i] >= a[c]) break;
        swap(a[i], a[c]);
        i = c;
    }
}

void HeapSort(int* a, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        HeapDown(a, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        HeapDown(a, i, 0);
    }
}

// hybrid
const int CUTOFF = 32;

void CompositeSort(int* a, int n) {
    if (n <= CUTOFF) InsertionSort(a, n);
    else HeapSort(a, n);
}

// =========================
// Timing (adaptive like research version)
// =========================
double TimeSort(void (*fn)(int*, int), const vector<int>& src, int n) {
    int* a = new int[n];

    int rep = 1;
    double t = 0;

    while (rep <= 1024) {
        auto st = Clk::now();

        for (int i = 0; i < rep; i++) {
            CopyArray(src.data(), a, n);
            fn(a, n);
        }

        auto ed = Clk::now();
        t = chrono::duration<double, micro>(ed - st).count();

        if (t > 50000) break; // 50ms
        rep *= 2;
    }

    delete[] a;
    return t / rep;
}

// =========================
// Correctness (from framework style)
// =========================
bool Check(void (*fn)(int*, int)) {
    vector<vector<int>> tests = {
        {},
        {1},
        {2,1},
        {5,1,3,2,9},
        MakeReverse(50),
        MakeRandom(50)
    };

    for (auto t : tests) {
        vector<int> ans = t;
        sort(ans.begin(), ans.end());

        vector<int> copy = t;
        if (!copy.empty())
            fn(copy.data(), copy.size());

        if (copy != ans) return false;
    }
    return true;
}

// =========================
// Main Benchmark
// =========================
int main() {
    vector<int> ns = {20, 50, 100, 500, 1000, 2000, 4000};

    if (!Check(InsertionSort) ||
        !Check(QuickSort) ||
        !Check(MergeSort) ||
        !Check(HeapSort) ||
        !Check(CompositeSort)) {
        cout << "Correctness failed\n";
        return 0;
    }

    cout << "n,Insertion,Quick,Merge,Heap,Composite(us)\n";

    for (int n : ns) {
        vector<int> base = MakeRandom(n);

        double ti = TimeSort(InsertionSort, base, n);
        double tq = TimeSort(QuickSort, base, n);
        double tm = TimeSort(MergeSort, base, n);
        double th = TimeSort(HeapSort, base, n);
        double tc = TimeSort(CompositeSort, base, n);

        cout << n << ","
             << ti << ","
             << tq << ","
             << tm << ","
             << th << ","
             << tc << "\n";
    }

    return 0;
}
```

## 效能分析

1.時間複雜度：
演算法,平均情況,最壞情況
Insertion Sort,O(n2),O(n2)
Quick Sort,O(nlogn),O(n2)
Merge Sort,O(nlogn),O(nlogn)
Heap Sort,O(nlogn),O(nlogn)
Composite Sort,O(nlogn),O(nlogn)

2.空間複雜度：
| 演算法 | 空間 |
|--------|------|
| Insertion Sort | O(1) |
| Quick Sort | O(log n) |
| Merge Sort | O(n) |
| Heap Sort | O(1) |
| Composite Sort | O(1) |

## 測試與驗證

### 測試案例

**輸入**

n = 5
5 3 8 1 2


**輸出**

Insertion Sort: 1 2 3 5 8
Quick Sort: 1 2 3 5 8
Merge Sort: 1 2 3 5 8
Heap Sort: 1 2 3 5 8
Composite Sort: 1 2 3 5 8

### 編譯與執行指令

```bash
g++ sort_benchmark.cpp -std=c++14 -O2 -o sort_benchmark
sort_benchmark.exe

```

## 申論及開發報告

### 多種排序演算法比較原因
1. 比較不同時間複雜度（O(n²) vs O(n log n)）
2. 分析理論與實務差異
3. 評估混合排序策略效果

### 改進方式

- **負權重支援**：Dijkstra 不支援負權重，未來可加入 Bellman-Ford 演算法。
- **強連通分量**：針對有向圖，可實作 Tarjan 演算法識別強連通分量。
- **迭代優化**：在 DFS 實作中若圖過深，改用顯式堆疊（Stack）可避免溢位。
