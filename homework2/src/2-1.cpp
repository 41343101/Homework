#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

class Polynomial;  // 前置宣告

// ------------------ Term 類別 ------------------
class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef;  // 係數
    int exp;     // 指數
};

// ------------------ Polynomial 類別 ------------------
class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

public:
    // 建構子
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 拷貝建構子
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }

    // 解構子
    ~Polynomial() {
        delete[] termArray;
    }

    // ------------------ 新增或合併項 ------------------
    void NewTerm(float c, int e) {
        if (c == 0) return;

        // 合併相同指數項
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    // 刪除為0的項
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }

        // 陣列擴充
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        // 插入新項
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        // 指數遞減排序（插入排序法）
        int i = terms - 1;
        while (i > 0 && termArray[i - 1].exp < termArray[i].exp) {
            swap(termArray[i - 1], termArray[i]);
            i--;
        }
    }

    // ------------------ 多項式加法 ------------------
    Polynomial Add(const Polynomial& poly) const {
        Polynomial result;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > poly.termArray[bPos].exp) {
                result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
        }
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);

        return result;
    }

    // ------------------ 多項式乘法 ------------------
    Polynomial Mult(const Polynomial& poly) const {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.NewTerm(newCoef, newExp);
            }
        }
        return result;
    }

    // ------------------ 多項式評估 ------------------
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};

// ------------------ operator>> ------------------
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    in >> n;
    poly.terms = 0; // 清空原有內容
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        in >> c >> e;
        poly.NewTerm(c, e);
    }
    return in;
}

// ------------------ operator<< ------------------
ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-"; c = -c;
        }

        // 只顯示必要的小數點
        if (c == (int)c)
            out << (int)c;
        else
            out << c;

        // 所有非零指數都顯示 x^exp
        if (e != 0)
            out << "x^" << e;
    }

    return out;
}


// ------------------ 主程式 ------------------
int main() {
    Polynomial p1, p2;
    float x;
    cout << "n: ";
    cin >> p1;
    cout << "n: ";
    cin >> p2;
    cout << "x: ";
    cin >> x;
    cout << "a(x) = " << p1 << endl;
    cout << "b(x) = " << p2 << endl;
    
    Polynomial sum = p1.Add(p2);
    cout << "s = " << sum << endl;
    Polynomial product = p1.Mult(p2);
    cout << "Product = " << product << endl;
    cout << "a(" << x << ") = " << p1.Eval(x) << endl;
    cout << "b(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}
