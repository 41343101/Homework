#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef; 
    int exp; 
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;
public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }
    ~Polynomial() {
        delete[] termArray;
    }
    void NewTerm(float c, int e) {
        if (c == 0) return;

        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
        int i = terms - 1;
        while (i > 0 && termArray[i - 1].exp < termArray[i].exp) {
            swap(termArray[i - 1], termArray[i]);
            i--;
        }
    }
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
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    in >> n;
    poly.terms = 0; 
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        in >> c >> e;
        poly.NewTerm(c, e);
    }
    return in;
}
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
        if (c == (int)c)
            out << (int)c;
        else
            out << c;
        if (e != 0)
            out << "x^" << e;
    }

    return out;
}

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
