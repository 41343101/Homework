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
