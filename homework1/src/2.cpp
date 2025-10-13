#include <iostream>
using namespace std;

int countBits(int x) {
    int cnt = 0;
    while (x) {
        cnt += (x & 1);
        x >>= 1;
    }
    return cnt;
}
int main() {
    int n;
    char a[100];
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int powerSetSize = 1 << n;
    for (int subsetSize = 0; subsetSize <= n; subsetSize++) {
        for (int i = 0; i < powerSetSize; i++) {
            if (countBits(i) == subsetSize) {
                cout << "(";
                for (int j = 0; j < n; j++) {
                    if (i & (1 << j)) {
                        cout << a[j];
                    }
                }
                cout << ")" << endl;
            }
        }
    }
    return 0;
}
