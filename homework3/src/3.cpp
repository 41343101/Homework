#include <iostream>
using namespace std;

template <class T> class ChainNode;
template <class T> class ChainIterator;
template <class T> class Chain;
template <class T> class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
};

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}
    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }
    ChainIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }
    bool operator!=(const ChainIterator& rhs) const {
        return current != rhs.current;
    }
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* head;
public:
    static ChainNode<T>* getNode() {
        if (!head) return nullptr;
        ChainNode<T>* tmp = head;
        head = head->next;
        tmp->next = nullptr;
        return tmp;
    }
    static void getBack(ChainNode<T>* node) {
        if (!node) return;
        ChainNode<T>* cur = node;
        while (cur->next) cur = cur->next;
        cur->next = head;
        head = node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::head = nullptr;

template <class T>
class Chain {
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        AvailableList<T>::getBack(release());
    }
    ChainNode<T>* release() {
        ChainNode<T>* tmp = head;
        head = nullptr;
        return tmp;
    }
    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }
    void insert(int idx, const T& val) {
        ChainNode<T>* node = AvailableList<T>::getNode();
        if (!node) node = new ChainNode<T>(val);
        else node->element = val;
        if (idx == 0) {
            node->next = head;
            head = node;
            return;
        }
        ChainNode<T>* prev = head;
        for (int i = 0; i < idx - 1 && prev; i++)
            prev = prev->next;
        if (!prev) {
            AvailableList<T>::getBack(node);
            return;
        }
        node->next = prev->next;
        prev->next = node;
    }
};

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

AvailableList<Term> globalASL;

class Polynomial {
private:
    Chain<Term> terms;
public:
    void newTerm(double c, int e) {
        int idx = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it, ++idx) {
            if (it->exp == e) {
                it->coef += c;
                return;
            }
            if (it->exp < e) {
                terms.insert(idx, Term(c, e));
                return;
            }
        }
        terms.insert(idx, Term(c, e));
    }
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        auto i = terms.begin(), j = b.terms.begin();
        while (i != terms.end() && j != b.terms.end()) {
            if (i->exp > j->exp) r.newTerm(i->coef, i->exp), ++i;
            else if (i->exp < j->exp) r.newTerm(j->coef, j->exp), ++j;
            else r.newTerm(i->coef + j->coef, i->exp), ++i, ++j;
        }
        while (i != terms.end()) r.newTerm(i->coef, i->exp), ++i;
        while (j != b.terms.end()) r.newTerm(j->coef, j->exp), ++j;
        return r;
    }
    Polynomial operator-(const Polynomial& b) const {
        Polynomial neg;
        for (auto it = b.terms.begin(); it != b.terms.end(); ++it)
            neg.newTerm(-it->coef, it->exp);
        return (*this) + neg;
    }
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto i = terms.begin(); i != terms.end(); ++i)
            for (auto j = b.terms.begin(); j != b.terms.end(); ++j)
                r.newTerm(i->coef * j->coef, i->exp + j->exp);
        return r;
    }
    float Evaluate(float x) const {
        float sum = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            float p = 1;
            for (int i = 0; i < it->exp; i++) p *= x;
            sum += it->coef * p;
        }
        return sum;
    }
    friend istream& operator>>(istream& is, Polynomial& p) {
        int n; is >> n;
        for (int i = 0; i < n; i++) {
            double c; int e;
            cout << "輸入第" << i + 1 << "項(係數 指數):";
            is >> c >> e;
            p.newTerm(c, e);
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        bool first = true;
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
            if (!first) os << " + ";
            first = false;
            os << it->coef << "x^" << it->exp;
        }
        return os;
    }
};

int main() {
    Polynomial A, B;
    float x;
    cout << "輸入A的項數:";
    cin >> A;
    cout << "輸入B的項數:";
    cin >> B;
    cout << "輸入x:";
    cin >> x;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A + B = " << A + B << endl;
    cout << "A - B = " << A - B << endl;
    cout << "A * B = " << A * B << endl;
    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;
    return 0;
}
