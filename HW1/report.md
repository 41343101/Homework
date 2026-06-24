# 41343101

# MinHeap 最小堆積程式

## 解題說明

### 問題描述

本程式實作**最小優先佇列（Min Priority Queue）**，並以**最小堆積（Min Heap）**作為底層資料結構，提供以下功能：

- 插入元素（Push）  
- 刪除最小元素（Pop）  
- 取得最小元素（Top）  
- 判斷是否為空（IsEmpty）  
- 以陣列順序輸出堆積內容  

使用者輸入 n 個整數，程式將其插入 MinHeap 中，並輸出堆積的內部陣列表示。

### 解題策略

### 2.1 資料結構設計

#### MinPQ（抽象類別）
- 定義優先佇列的基本操作介面：
  - `IsEmpty()`
  - `Top()`
  - `Push()`
  - `Pop()`

#### MinHeap（實作類別）
- 使用**動態陣列**儲存堆積
- 採用**完全二元樹（Complete Binary Tree）**結構
- 陣列從 index 1 開始：
  - 父節點：i / 2  
  - 左子節點：2i  
  - 右子節點：2i + 1  

---

### 2.2 核心運算流程

1. **插入元素（Push）**
   - 將新元素放在陣列尾端  
   - 使用「上浮（heapify up）」維持最小堆性質  
   - 若容量不足則進行陣列擴充（Resize）

2. **刪除最小元素（Pop）**
   - 移除根節點（最小值）  
   - 將最後一個元素移至根節點  
   - 使用「下沉（heapify down）」重新調整  

3. **取得最小值（Top）**
   - 回傳陣列 index 1 的元素  

4. **動態擴充（Resize）**
   - 當容量不足時，將陣列大小擴大為原本的 2 倍 

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>     // 輸入輸出
#include <stdexcept>    // runtime_error
using namespace std;

// =========================
// 最小優先佇列(Min Priority Queue)
// 抽象類別(介面)
// =========================
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}

    // 判斷是否為空
    virtual bool IsEmpty() const = 0;

    // 取得最小元素
    virtual const T& Top() const = 0;

    // 插入元素
    virtual void Push(const T&) = 0;

    // 刪除最小元素
    virtual void Pop() = 0;
};
// =========================
// MinHeap
// 使用 Binary Heap 實作 MinPQ
// =========================
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;        // Heap陣列
    int capacity;   // 最大容量
    int size;       // 目前元素個數
    // =========================
    // 容量不足時擴充為兩倍
    // =========================
    void Resize() {

        capacity *= 2;

        // index從1開始，因此多開一格
        T* newHeap = new T[capacity + 1];

        // 複製原本資料
        for (int i = 1; i <= size; i++)
            newHeap[i] = heap[i];

        delete[] heap;
        heap = newHeap;
    }
public:
    // =========================
    // 建構子
    // =========================
    MinHeap(int cap = 10) {

        capacity = cap;

        // index從1開始
        heap = new T[capacity + 1];

        size = 0;
    }
    // =========================
    // 解構子
    // =========================
    ~MinHeap() {
        delete[] heap;
    }
    // =========================
    // 判斷Heap是否為空
    // =========================
    bool IsEmpty() const {
        return size == 0;
    }
    // =========================
    // 回傳最小值(根節點)
    // =========================
    const T& Top() const {

        if (IsEmpty())
            throw runtime_error("Heap is empty");
        return heap[1];
    }

    // =========================
    // 插入元素
    // Heapify Up (上浮)
    // =========================
    void Push(const T& x) {

        // 空間不足就擴充
        if (size + 1 == capacity)
            Resize();

        // 新元素先放最後面
        int i = ++size;

        // 與父節點比較
        while (i != 1 && x < heap[i / 2]) {

            // 父節點往下移
            heap[i] = heap[i / 2];

            // 繼續往上比較
            i /= 2;
        } // 放入正確位置
        heap[i] = x;
    }

    // =========================
    // 刪除最小元素
    // Heapify Down (下沉)
    // =========================
    void Pop() {

        if (IsEmpty())
            throw runtime_error("Heap is empty");

        // 最後一個元素
        T last = heap[size--];
        int parent = 1;
        int child = 2;

        // 找適當位置放 last
        while (child <= size) {

            // 找較小的孩子
            if (child < size &&
                heap[child] > heap[child + 1])
                child++;

            // 已符合 MinHeap
            if (last <= heap[child])
                break;

            // 孩子往上移
            heap[parent] = heap[child];
            parent = child;
            child *= 2;
        }
        // 放入最後元素
        heap[parent] = last;
    }

    // =========================
    // 依照Heap陣列索引輸出
    // =========================
    void PrintByIndex() const {
        for (int i = 1; i <= size; i++)
            cout << heap[i] << " ";
        cout << endl;
    }
};

// =========================
// 主程式
// =========================
int main() {

    // 建立MinHeap
    MinHeap<int> h;
    int n, x;

    // 輸入元素數量
    cout << "輸入元素個數: ";
    cin >> n;

    // 插入元素
    cout << "輸入元素: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        h.Push(x);
    }

    // 輸出Heap內容
    cout << "Heap (index順序): ";
    h.PrintByIndex();
    return 0;
}
```

## 效能分析

1.時間複雜度：
| 操作 | 複雜度 |
|------|--------|
| 插入（Push） | O(log n) |
| 刪除最小值（Pop） | O(log n) |
| 取得最小值（Top） | O(1) |
| 判斷是否為空 | O(1) |
| 建立堆積（n 次插入） | O(n log n) |

2.空間複雜度：
- O(n)  
- 使用動態陣列儲存所有元素  

## 測試與驗證

### 測試案例

**輸入**

5

5 3 8 1 2


**輸出**

Heap (index順序): 1 2 8 5 3

### 編譯與執行指令

```bash
g++ minheap.cpp -std=c++14 -o minheap
minheap.exe

```

## 申論及開發報告

### 選擇 MinHeap 的原因

1.高效率優先佇列：
插入與刪除操作皆為 O(log n)，效率優於一般排序方式

2.結構簡單：
使用陣列即可實現完全二元樹，不需額外指標結構

3.應用廣泛：
可應用於 Dijkstra、Huffman Coding、排程系統等

### 改進方式

改用 平衡樹（AVL / Red-Black Tree） 優化搜尋效率
使用 快速冪（Fast Power） 加速 Evaluate
加入 多執行緒 平行處理乘法運算
