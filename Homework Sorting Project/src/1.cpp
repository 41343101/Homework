#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <iomanip>
#include <string>

using namespace std;
using Clk = chrono::high_resolution_clock;
using Us = chrono::duration<double, micro>;

enum AlgType { INS, QCK, MRG, HEP, CMP };

struct Result {
    int n;
    string alg;
    string kind;
    double us;
    int trials;
};

mt19937 rng(20260530);

string GetAlgName(AlgType t) {
    if (t == INS) return "Insertion";
    if (t == QCK) return "Quick";
    if (t == MRG) return "Merge";
    if (t == HEP) return "Heap";
    if (t == CMP) return "Composite";
    return "Unknown";
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

vector<int> MakeMergeWorstRec(const vector<int>& s) {
    int n = s.size();
    if (n <= 1) return s;
    vector<int> l, r;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) l.push_back(s[i]);
        else r.push_back(s[i]);
    }
    l = MakeMergeWorstRec(l);
    r = MakeMergeWorstRec(r);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}

vector<int> GetWorstData(AlgType t, int n) {
    if (t == INS) return MakeReverse(n);
    if (t == MRG || t == CMP) {
        vector<int> base(n);
        iota(base.begin(), base.end(), 1);
        return MakeMergeWorstRec(base);
    }
    return MakeRandom(n);
}

void InsertionSortCore(int* a, int l, int r) {
    for (int i = l + 1; i <= r; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= l && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void InsertionSort(int* a, int n) {
    if (n <= 1) return;
    InsertionSortCore(a, 0, n - 1);
}

int Median3(int* a, int l, int r) {
    int m = l + (r - l) / 2;
    if (a[m] < a[l]) swap(a[m], a[l]);
    if (a[r] < a[l]) swap(a[r], a[l]);
    if (a[r] < a[m]) swap(a[r], a[m]);
    swap(a[m], a[r - 1]);
    return a[r - 1];
}

void QuickSortRec(int* a, int l, int r) {
    const int CUTOFF = 16;
    if (l + CUTOFF <= r) {
        int pivot = Median3(a, l, r);
        int i = l;
        int j = r - 1;
        while (true) {
            while (a[++i] < pivot);
            while (pivot < a[--j]);
            if (i < j) swap(a[i], a[j]);
            else break;
        }
        swap(a[i], a[r - 1]);
        QuickSortRec(a, l, i - 1);
        QuickSortRec(a, i + 1, r);
    } else {
        InsertionSortCore(a, l, r);
    }
}

void QuickSort(int* a, int n) {
    if (n > 1) QuickSortRec(a, 0, n - 1);
}

void MergeCore(int* a, int* tmp, int l, int m, int r) {
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
            MergeCore(a, tmp, l, m, r);
        }
    }
}

void HeapDown(int* a, int s, int e) {
    int root = s;
    while (root * 2 + 1 <= e) {
        int child = root * 2 + 1;
        if (child + 1 <= e && a[child] < a[child + 1]) child++;
        if (a[root] >= a[child]) break;
        swap(a[root], a[child]);
        root = child;
    }
}

void HeapSort(int* a, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) HeapDown(a, i, n - 1);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        HeapDown(a, 0, i - 1);
    }
}

void CompositeSort(int* a, int n) {
    const int CMP_CUTOFF = 32;
    for (int i = 0; i < n; i += CMP_CUTOFF) {
        InsertionSortCore(a, i, min(i + CMP_CUTOFF - 1, n - 1));
    }
    vector<int> tmpv(n);
    int* tmp = tmpv.data();
    for (int w = CMP_CUTOFF; w < n; w *= 2) {
        for (int l = 0; l < n; l += 2 * w) {
            int m = min(l + w, n);
            int r = min(l + 2 * w, n);
            MergeCore(a, tmp, l, m, r);
        }
    }
}

void RunSort(AlgType t, int* a, int n) {
    if (t == INS) InsertionSort(a, n);
    else if (t == QCK) QuickSort(a, n);
    else if (t == MRG) MergeSort(a, n);
    else if (t == HEP) HeapSort(a, n);
    else if (t == CMP) CompositeSort(a, n);
}

double TimeSort(AlgType t, const vector<int>& src, int n) {
    int trials = 1;
    double totalUs = 0.0;
    vector<int> workspace(n);

    while (trials <= 1048576) {
        auto st = Clk::now();
        for (int i = 0; i < trials; i++) {
            for (int x = 0; x < n; x++) workspace[x] = src[x];
            RunSort(t, workspace.data(), n);
        }
        auto ed = Clk::now();
        totalUs = Us(ed - st).count();
        if (totalUs >= 100000.0) break;
        trials *= 2;
    }
    return totalUs / trials;
}

bool Check(AlgType t) {
    vector<vector<int>> testCases = {
        {}, {42}, {5, 1}, {3, 9, 2, 3, 1, 7}, MakeReverse(45), MakeRandom(100)
    };
    for (auto& tc : testCases) {
        vector<int> gold = tc;
        sort(gold.begin(), gold.end());
        vector<int> test = tc;
        RunSort(t, test.data(), test.size());
        if (test != gold) return false;
    }
    return true;
}

int main() {
    vector<AlgType> algs = { INS, QCK, MRG, HEP, CMP };
    vector<int> ns = {20, 50, 100, 500, 1000, 2000, 4000};

    for (AlgType t : algs) {
        if (!Check(t)) {
            cout << "Correctness failed\n";
            return 0;
        }
    }

    cout << "n,Insertion,Quick,Merge,Heap,Composite(us)\n";
    cout << fixed << setprecision(3);

    for (int n : ns) {
        vector<int> base = MakeRandom(n);
        double ti = TimeSort(INS, base, n);
        double tq = TimeSort(QCK, base, n);
        double tm = TimeSort(MRG, base, n);
        double th = TimeSort(HEP, base, n);
        double tc = TimeSort(CMP, base, n);

        cout << n << "," << ti << "," << tq << "," << tm << "," << th << "," << tc << "\n";
    }
    return 0;
}
