# 41343101

# Graph Algorithms 圖形演算法實作報告

## 解題說明

### 問題描述

本程式實作了圖形理論中五個核心的演算法與操作，涵蓋了從基本的遍歷到進階的網路分析功能：

- **基本圖形遍歷**：實作深度優先搜尋（DFS）與廣度優先搜尋（BFS）。
- **連通分支（Connected Components）**：識別無向圖中所有孤立的連通群體。
- **最小生成樹（MST）**：使用 **Kruskal 演算法** 在加權圖中尋找總權重最小的生成樹。
- **單源最短路徑**：實作 **Dijkstra 演算法** 計算起點到各頂點的最短距離。
- **拓撲排序（Topological Sort）**：針對有向無環圖（DAG）決定活動執行的先後順序（AOV 網路）。

### 解題策略

### 2.1 資料結構設計

#### 鄰接表 (Adjacency List)
- 使用 `vector<vector<int>>` 或 `vector<vector<pair<int, int>>>`。
- 相比鄰接矩陣，能有效節省空間並提高遍歷鄰居的效率。

#### 互斥集合 (Disjoint Set Union, DSU)
- 用於 Kruskal 演算法。
- 實作「路徑壓縮」與「按秩合併」優化，使查詢與合併操作趨近於常數時間。

#### 優先佇列 (Priority Queue)
- 用於 Dijkstra 演算法。
- 使用最小堆積（Min-Heap）結構，每次取出目前距離最小的節點。

---

### 2.2 核心運算流程

1. **DFS & BFS**
   - **DFS**：利用遞迴深入探索，直到無路可走再回溯。
   - **BFS**：利用佇列（Queue）逐層探索，適合尋找無權重圖的最短路徑。

2. **Kruskal 演算法**
   - 將所有邊按權重**從小到大排序**。
   - 利用 DSU 檢查邊的兩個端點是否已連通，若未連通則加入 MST，避免形成環。

3. **Dijkstra 演算法**
   - 初始化距離陣列為無限大，起點為 0。
   - 重複選取距離最短的頂點進行**鬆弛操作（Relaxation）**，更新鄰居的最短距離。

4. **拓撲排序 (Kahn's Algorithm)**
   - 統計所有節點的**入度 (In-degree)**。
   - 將入度為 0 的節點放入佇列，取出後更新其指向節點的入度，重複此過程。

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
| 演算法 | 複雜度 | 說明 |
|------|--------|------|
| DFS / BFS | $O(V + E)$ | 每個節點與邊皆走訪一次 |
| Kruskal | $O(E \log E)$ | 主要時間花在邊的排序 |
| Dijkstra | $O(E \log V)$ | 使用優先佇列優化 |
| Topological Sort | $O(V + E)$ | 基於入度的線性處理 |

2.空間複雜度：
- $O(V + E)$  
- 使用鄰接表儲存圖形結構。

## 測試與驗證

### 測試案例

**1. MST 驗證**
- 輸入：邊(0,1,10), (0,2,6), (0,3,5), (2,3,4)
- 輸出：Total MST Weight: 19

**2. Dijkstra 驗證**
- 輸入：0->1(10), 0->4(5), 1->2(1)
- 輸出：0 到各點的最短路徑。

### 編譯與執行指令

```bash
g++ graph_algo.cpp -std=c++14 -o graph_algo
./graph_algo

```

## 申論及開發報告

### 選擇這些演算法的原因

1. **模組化設計**：
   將不同功能的演算法封裝在獨立類別中，易於維護與測試。

2. **實用性廣**：
   圖形演算法是解決排程（Topological Sort）、導航（Dijkstra）及網路佈線（Kruskal）的基礎。

3. **效能優勢**：
   採用鄰接表而非鄰接矩陣，對於稀疏圖（Sparse Graph）能大幅減少記憶體佔用。

### 改進方式

- **負權重支援**：Dijkstra 不支援負權重，未來可加入 Bellman-Ford 演算法。
- **強連通分量**：針對有向圖，可實作 Tarjan 演算法識別強連通分量。
- **迭代優化**：在 DFS 實作中若圖過深，改用顯式堆疊（Stack）可避免溢位。
