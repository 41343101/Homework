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
