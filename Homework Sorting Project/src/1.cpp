#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <iomanip>

using namespace std;
using Clk = chrono::high_resolution_clock;
mt19937 rng(20260530);

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

const int CUTOFF = 32;
void CompositeSort(int* a, int n) {
    if (n <= CUTOFF) InsertionSort(a, n);
    else HeapSort(a, n);
}

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
