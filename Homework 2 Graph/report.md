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
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define INF 1e9

class BasicGraph {
private:
    int n;
    vector<vector<int>> adj;
    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";
        for (int u : adj[v]) {
            if (!visited[u]) {
                DFSUtil(u, visited);
            }
        }
    }
public:
    BasicGraph(int nodes) : n(nodes) {
        adj.resize(n);
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void DFS(int startVertex) {
        vector<bool> visited(n, false);
        cout << "DFS: ";
        DFSUtil(startVertex, visited);
        cout << endl;
    }
    void BFS(int startVertex) {
        vector<bool> visited(n, false);
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);
        cout << "BFS: ";
        while (!q.empty()) {
            int v = q.front();
            cout << v << " ";
            q.pop();
            for (int u : adj[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }
        cout << endl;
    }
    void ConnectedComponents() {
        vector<bool> visited(n, false);
        cout << "Connected Components:" << endl;
        int count = 1;
        for (int v = 0; v < n; v++) {
            if (!visited[v]) {
                cout << "  Component " << count++ << ": ";
                DFSUtil(v, visited);
                cout << endl;
            }
        }
    }
};

struct Edge {
    int u, v, weight;
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};

class DisjointSet {
    vector<int> parent, rank;
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            if (rank[rootI] < rank[rootJ]) parent[rootI] = rootJ;
            else if (rank[rootI] > rank[rootJ]) parent[rootJ] = rootI;
            else {
                parent[rootJ] = rootI;
                rank[rootI]++;
            }
        }
    }
};

class MSTGraph {
    int V;
    vector<Edge> edges;
public:
    MSTGraph(int V) : V(V) {}
    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }
    void KruskalMST() {
        int mst_weight = 0;
        sort(edges.begin(), edges.end());
        DisjointSet ds(V);
        cout << "Kruskal's MST Edges:\n";
        for (Edge e : edges) {
            if (ds.find(e.u) != ds.find(e.v)) {
                ds.unite(e.u, e.v);
                mst_weight += e.weight;
                cout << "  " << e.u << " - " << e.v << " \t(Weight: " << e.weight << ")\n";
            }
        }
        cout << "Total MST Weight: " << mst_weight << endl;
    }
};

class ShortestPathGraph {
    int V;
    vector<vector<pair<int, int>>> adj;
public:
    ShortestPathGraph(int V) : V(V) {
        adj.resize(V);
    }
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
    }
    void Dijkstra(int src) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(V, INF);
        pq.push({0, src});
        dist[src] = 0;
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            for (auto x : adj[u]) {
                int v = x.first;
                int weight = x.second;
                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        cout << "Vertex\tDistance from Source (" << src << ")\n";
        for (int i = 0; i < V; ++i)
            cout << i << "\t" << (dist[i] == INF ? -1 : dist[i]) << "\n";
    }
};

class AOVNetwork {
    int V;
    vector<vector<int>> adj;
    vector<int> in_degree;
public:
    AOVNetwork(int V) : V(V) {
        adj.resize(V);
        in_degree.resize(V, 0);
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        in_degree[v]++;
    }
    void TopologicalSort() {
        queue<int> q;
        for (int i = 0; i < V; i++) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }
        int count = 0;
        vector<int> top_order;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            top_order.push_back(u);
            for (int v : adj[u]) {
                if (--in_degree[v] == 0) {
                    q.push(v);
                }
            }
            count++;
        }
        if (count != V) {
            cout << "There exists a cycle in the network (Not a DAG)\n";
            return;
        }
        cout << "Topological Sort (Activity Order): ";
        for (int i : top_order) {
            cout << i << " ";
        }
        cout << endl;
    }
};

int main() {
    cout << "========== 1 & 2. 基本圖形操作 ==========\n";
    BasicGraph bg(5);
    bg.addEdge(0, 1);
    bg.addEdge(0, 2);
    bg.addEdge(1, 2);
    bg.addEdge(3, 4); 
    bg.DFS(0);
    bg.BFS(0);
    bg.ConnectedComponents();
    cout << "\n";

    cout << "========== 3. 最小花費生成樹 (Kruskal) ==========\n";
    MSTGraph mg(4);
    mg.addEdge(0, 1, 10);
    mg.addEdge(0, 2, 6);
    mg.addEdge(0, 3, 5);
    mg.addEdge(1, 3, 15);
    mg.addEdge(2, 3, 4);
    mg.KruskalMST();
    cout << "\n";

    cout << "========== 4. 最短路徑 (Dijkstra) ==========\n";
    ShortestPathGraph spg(5);
    spg.addEdge(0, 1, 10);
    spg.addEdge(0, 4, 5);
    spg.addEdge(1, 2, 1);
    spg.addEdge(1, 4, 2);
    spg.addEdge(4, 1, 3);
    spg.addEdge(4, 2, 9);
    spg.addEdge(4, 3, 2);
    spg.addEdge(2, 3, 4);
    spg.addEdge(3, 2, 6);
    spg.addEdge(3, 0, 7);
    spg.Dijkstra(0);
    cout << "\n";

    cout << "========== 5. 活動網路 (Topological Sort) ==========\n";
    AOVNetwork aov(6);
    aov.addEdge(5, 2);
    aov.addEdge(5, 0);
    aov.addEdge(4, 0);
    aov.addEdge(4, 1);
    aov.addEdge(2, 3);
    aov.addEdge(3, 1);
    aov.TopologicalSort();
    cout << "\n";

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
