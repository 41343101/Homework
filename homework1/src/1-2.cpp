#include <iostream>
using namespace std;

int ack(int m, int n) {
    const int MAX = 10000;
    int M[MAX];
    int top = 0;
    M[top++] = m;

    while (top > 0) {
        m = M[--top];
        if (m == 0)  n += 1;
        else if (n == 0) {
            M[top++] = m - 1;
            n = 1;
        }
        else {
            M[top++] = m - 1;
            M[top++] = m;
            n -= 1;
        }
    }
    return n;
}
int main() {
    int m, n;
    cin >> m >> n;
    cout << ack(m, n) << endl;
    return 0;
}
