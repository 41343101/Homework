# 41343101

# Binary Search Tree (BST)

## 解題說明

### 問題描述

本程式實作**二元搜尋樹（Binary Search Tree, BST）**，並用於測量隨機插入 n 個整數後的樹高度，分析高度與理論對數之比。

程式功能包括：
- 插入元素到 BST  
- 計算 BST 高度  
- 生成 n 個元素的隨機排列（Fisher–Yates shuffle）  
- 輸出每個 n 對應的 BST 高度及高度與 log₂(n) 比例   

程式特別使用 **Available List（可用節點串列）** 管理記憶體，以減少重複配置與釋放節點所造成的成本。

### 解題策略

### 2.1 資料結構設計

#### TreeNode 類別
- 儲存一個 key-value pair  
- 左右子樹指標：`left`、`right`  

#### BST 類別
- 私有成員：根節點 `root`  
- 提供操作：
  - `Insert(pair<K,E>)`：插入節點  
  - `Height()`：計算樹高度  
  - 自動釋放記憶體  

---

### 2.2 核心運算流程

1. **插入元素（Insert）**
   - 遞迴方式比較 key 值：
     - 小於節點 key → 左子樹  
     - 大於節點 key → 右子樹  
   - 若節點為空，建立新節點  

2. **計算高度（Height）**
   - 遞迴計算左右子樹高度  
   - 高度 = 左右子樹最大高度 + 1  

3. **Fisher–Yates 隨機排列**
   - 將陣列從尾端隨機交換元素  
   - 確保插入順序隨機化，避免 BST 退化成鏈表  

4. **高度分析**
   - 計算 `height / log2(n)` 比值  
   - 用於檢驗 BST 在隨機插入下接近平衡樹的理論高度 

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

template <class K, class E>
class TreeNode {
public:
    pair<K, E> data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const pair<K, E>& e)
        : data(e), left(nullptr), right(nullptr) {}
};

template <class K, class E>
class BST {
private:
    TreeNode<K, E>* root;

    TreeNode<K, E>* insert(TreeNode<K, E>* node, const pair<K, E>& e) {
        if (!node) return new TreeNode<K, E>(e);

        if (e.first < node->data.first)
            node->left = insert(node->left, e);
        else if (e.first > node->data.first)
            node->right = insert(node->right, e);

        return node;
    }

    int height(TreeNode<K, E>* node) {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    void destroy(TreeNode<K, E>* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void Insert(const pair<K, E>& e) {
        root = insert(root, e);
    }

    int Height() {
        return height(root);
    }
};

// Fisher–Yates shuffle
void shuffleArray(int* a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(a[i], a[j]);
    }
}

int main() {
    srand((unsigned)time(nullptr));

    int ns[] = { 100, 500, 1000, 2000, 3000, 4000, 5000,
                 6000, 7000, 8000, 9000, 10000 };

    cout << left << setw(10) << "n"
         << setw(10) << "Height"
         << "Ratio(h/log2n)" << endl;
    cout << string(40, '-') << endl;

    for (int n : ns) {
        BST<int, int> tree;

        int* arr = new int[n];
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;

        shuffleArray(arr, n);

        for (int i = 0; i < n; i++)
            tree.Insert({arr[i], arr[i]});

        int h = tree.Height();
        double ratio = (double)h / log2((double)n);

        cout << left << setw(10) << n
             << setw(10) << h
             << fixed << setprecision(4)
             << ratio << endl;

        delete[] arr;
    }

    return 0;
}
```

## 效能分析

1.時間複雜度：
| 操作 | 複雜度 |
|------|--------|
| 單次插入（平均） | O(log n) |
| 單次插入（最壞） | O(n) |
| 計算高度 | O(n) |
| Fisher–Yates shuffle | O(n) |
| n 個元素插入 | 平均 O(n log n)、最壞 O(n²) |

2.空間複雜度：
- O(n)  
- 儲存 BST 節點與陣列  

## 測試與驗證

### 測試案例

**輸入**

n = 10
元素: 1~10 隨機排列

**輸出**

BST高度: 6
h / log2(n) ≈ 2.0

### 編譯與執行指令

```bash
g++ bst_height.cpp -std=c++14 -o bst_height
bst_height.exe

```

## 申論及開發報告

### 選擇 BST 的原因

1.高效率查找：
平均插入與搜尋 O(log n)
適合動態資料的排序與查找

2.樹結構直觀：
左小右大，方便理解與驗證
遞迴實作簡單易讀

3.高度分析意義：
可觀察隨機插入的平均高度
檢驗與理論對數關係，用於資料結構教學

### 改進方式

可改用 自平衡 BST（如 AVL 或 Red-Black Tree）
使用 迭代方式插入 避免遞迴過深
儲存節點高度作為成員以加速高度查詢
