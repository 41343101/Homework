
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
