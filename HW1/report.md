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
#include <iostream>
#include <stdexcept>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int size;

    void Resize() {
        capacity *= 2;
        T* newHeap = new T[capacity + 1]; // index從1開始

        for (int i = 1; i <= size; i++)
            newHeap[i] = heap[i];

        delete[] heap;
        heap = newHeap;
    }

public:
    MinHeap(int cap = 10) {
        capacity = cap;
        heap = new T[capacity + 1]; // index從1開始
        size = 0;
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    const T& Top() const {
        if (IsEmpty())
            throw runtime_error("Heap is empty");
        return heap[1];
    }

    void Push(const T& x) {
        if (size + 1 == capacity)
            Resize();

        int i = ++size;

        // 上浮 (heapify up)
        while (i != 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }

        heap[i] = x;
    }

    void Pop() {
        if (IsEmpty())
            throw runtime_error("Heap is empty");

        T last = heap[size--];

        int parent = 1;
        int child = 2;

        // 下沉 (heapify down)
        while (child <= size) {
            if (child < size && heap[child] > heap[child + 1])
                child++;

            if (last <= heap[child])
                break;

            heap[parent] = heap[child];
            parent = child;
            child *= 2;
        }

        heap[parent] = last;
    }

    void PrintByIndex() const {
        for (int i = 1; i <= size; i++)
            cout << heap[i] << " ";
        cout << endl;
    }
};

int main() {
    MinHeap<int> h;

    int n, x;
    cout << "輸入元素個數: ";
    cin >> n;

    cout << "輸入元素: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        h.Push(x);
    }

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
