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
#include <iostream>     // 輸入輸出
#include <cmath>        // log2()
#include <cstdlib>      // rand(), srand()
#include <ctime>        // time()
#include <algorithm>    // max(), swap()
#include <iomanip>      // setw(), setprecision()
using namespace std;

// =========================
// TreeNode 類別
// BST 的每一個節點
// =========================
template <class K, class E>
class TreeNode {
public:
    pair<K, E> data;    // 儲存 (key, value)
    TreeNode* left;     // 指向左子樹
    TreeNode* right;    // 指向右子樹

    // 建構子
    TreeNode(const pair<K, E>& e)
        : data(e), left(nullptr), right(nullptr) {}
};

// =========================
// BST(Binary Search Tree)
// 二元搜尋樹
// =========================
template <class K, class E>
class BST {
private:
    TreeNode<K, E>* root;   // 根節點

    // =========================
    // 遞迴插入節點
    // =========================
    TreeNode<K, E>* insert(TreeNode<K, E>* node,
                           const pair<K, E>& e)
    {
        // 找到空位置建立新節點
        if (!node)
            return new TreeNode<K, E>(e);

        // key較小往左走
        if (e.first < node->data.first)
            node->left = insert(node->left, e);

        // key較大往右走
        else if (e.first > node->data.first)
            node->right = insert(node->right, e);

        // 回傳目前節點
        return node;
    }

    // =========================
    // 計算樹高
    // Height = 1 + 左右子樹較大的高度
    // =========================
    int height(TreeNode<K, E>* node)
    {
        // 空樹高度為0
        if (!node)
            return 0;

        return 1 +
            max(height(node->left),
                height(node->right));
    }

    // =========================
    // 後序走訪刪除整棵樹
    // 防止記憶體洩漏
    // =========================
    void destroy(TreeNode<K, E>* node)
    {
        if (!node)
            return;

        destroy(node->left);   // 刪左子樹
        destroy(node->right);  // 刪右子樹

        delete node;           // 刪自己
    }

public:

    // =========================
    // 建構子
    // 初始根節點為空
    // =========================
    BST() : root(nullptr) {}

    // =========================
    // 解構子
    // 程式結束前釋放記憶體
    // =========================
    ~BST()
    {
        destroy(root);
    }

    // =========================
    // 插入資料
    // =========================
    void Insert(const pair<K, E>& e)
    {
        root = insert(root, e);
    }

    // =========================
    // 取得整棵樹高度
    // =========================
    int Height()
    {
        return height(root);
    }
};

// ======================================
// Fisher–Yates Shuffle
// 將陣列隨機打亂
// ======================================
void shuffleArray(int* a, int n)
{
    // 從最後一個元素開始往前交換
    for (int i = n - 1; i > 0; --i)
    {
        // 產生 0~i 的隨機位置
        int j = rand() % (i + 1);

        // 交換
        swap(a[i], a[j]);
    }
}

// =========================
// 主程式
// =========================
int main()
{
    // 設定亂數種子
    srand((unsigned)time(nullptr));

    // 測試的資料量
    int ns[] =
    {
        100,
        500,
        1000,
        2000,
        3000,
        4000,
        5000,
        6000,
        7000,
        8000,
        9000,
        10000
    };

    // 輸出表頭
    cout << left
         << setw(10) << "n"
         << setw(10) << "Height"
         << "Ratio(h/log2n)"
         << endl;

    // 分隔線
    cout << string(40, '-') << endl;

    // ==================================
    // 依序測試不同 n
    // ==================================
    for (int n : ns)
    {
        // 建立空BST
        BST<int, int> tree;

        // 動態配置陣列
        int* arr = new int[n];

        // 建立 1~n
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;

        // 打亂順序
        shuffleArray(arr, n);

        // 插入BST
        for (int i = 0; i < n; i++)
        {
            tree.Insert(
                {
                    arr[i],  // key
                    arr[i]   // value
                }
            );
        }

        // 計算樹高
        int h = tree.Height();

        // 計算高度與 log2(n) 的比值
        double ratio =
            (double)h /
            log2((double)n);

        // 輸出結果
        cout << left
             << setw(10) << n
             << setw(10) << h
             << fixed
             << setprecision(4)
             << ratio
             << endl;

        // 釋放陣列記憶體
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

可改用自平衡BST（如 AVL 或 Red-Black Tree）
使用迭代方式插入避免遞迴過深
儲存節點高度作為成員以加速高度查詢
