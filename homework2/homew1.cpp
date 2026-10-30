#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& s, Polynomial p);

private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    void newTerm(const float theCoef, const int theExp);
    void sortTerms();

public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& other)
        : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];

        for (int i = 0; i < terms; ++i)
            termArray[i] = other.termArray[i];
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other)
            return *this;

        Term* newArray = new Term[other.capacity];

        for (int i = 0; i < other.terms; ++i)
            newArray[i] = other.termArray[i];

        delete[] termArray;

        termArray = newArray;
        capacity = other.capacity;
        terms = other.terms;

        return *this;
    }

    ~Polynomial() {
        delete[] termArray;
    }

    Polynomial Add(const Polynomial& b);
    Polynomial Mult(Polynomial b);
    float Eval(float f);

    friend istream& operator>>(istream& s, Polynomial& p);
    friend ostream& operator<<(ostream& s, Polynomial p);
};

void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0)
        return;

    if (terms == capacity) {
        capacity *= 2;

        Term* temp = new Term[capacity];

        copy(termArray, termArray + terms, temp);

        delete[] termArray;
        termArray = temp;
    }

    termArray[terms].coef = theCoef;
    termArray[terms++].exp = theExp;
}

void Polynomial::sortTerms() {
    for (int i = 0; i < terms - 1; ++i) {
        int maxPos = i;

        for (int j = i + 1; j < terms; ++j) {
            if (termArray[j].exp > termArray[maxPos].exp)
                maxPos = j;
        }

        if (maxPos != i)
            swap(termArray[i], termArray[maxPos]);
    }

    int count = 0;

    for (int i = 0; i < terms; ++i) {
        if (count > 0 &&
            termArray[count - 1].exp == termArray[i].exp) {
            termArray[count - 1].coef += termArray[i].coef;

            if (termArray[count - 1].coef == 0)
                --count;
        } else {
            termArray[count] = termArray[i];
            ++count;
        }
    }

    terms = count;
}

Polynomial Polynomial::Add(const Polynomial& b) {
    Polynomial c;

    int aPos = 0;
    int bPos = 0;

    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;

            if (t)
                c.newTerm(t, termArray[aPos].exp);

            ++aPos;
            ++bPos;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.newTerm(b.termArray[bPos].coef,
                      b.termArray[bPos].exp);
            ++bPos;
        } else {
            c.newTerm(termArray[aPos].coef,
                      termArray[aPos].exp);
            ++aPos;
        }
    }

    for (; aPos < terms; ++aPos)
        c.newTerm(termArray[aPos].coef,
                  termArray[aPos].exp);

    for (; bPos < b.terms; ++bPos)
        c.newTerm(b.termArray[bPos].coef,
                  b.termArray[bPos].exp);

    return c;
}

Polynomial Polynomial::Mult(Polynomial b) {
    Polynomial c;

    for (int i = 0; i < terms; ++i) {
        Polynomial row;

        for (int j = 0; j < b.terms; ++j) {
            row.newTerm(
                termArray[i].coef * b.termArray[j].coef,
                termArray[i].exp + b.termArray[j].exp
            );
        }

        c = c.Add(row);
    }

    return c;
}

float Polynomial::Eval(float f) {
    float result = 0;

    for (int i = 0; i < terms; ++i)
        result += termArray[i].coef *
                  pow(f, termArray[i].exp);

    return result;
}

istream& operator>>(istream& s, Polynomial& p) {
    int n;

    if (!(s >> n) || n < 0) {
        s.setstate(ios::failbit);
        return s;
    }

    Polynomial temp;

    for (int i = 0; i < n; ++i) {
        float c;
        int e;

        if (!(s >> c >> e) || c == 0 || e < 0) {
            s.setstate(ios::failbit);
            return s;
        }

        temp.newTerm(c, e);
    }

    temp.sortTerms();
    p = temp;

    return s;
}

ostream& operator<<(ostream& s, Polynomial p) {
    if (p.terms == 0) {
        s << "0";
        return s;
    }

    for (int i = 0; i < p.terms; ++i) {
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;

        if (i > 0 && c > 0)
            s << "+";

        if (e == 0) {
            s << c;
        } else {
            if (c == -1)
                s << "-";
            else if (c != 1)
                s << c;

            s << "X";

            if (e != 1)
                s << "^" << e;
        }
    }

    return s;
}

int main() {
    Polynomial p1, p2;
    int choice = -1;

    cout << "請輸入多項式 A: ";
    if (!(cin >> p1)) {
        cerr << "多項式 A 輸入錯誤。\n";
        return 1;
    }

    cout << "請輸入多項式 B: ";
    if (!(cin >> p2)) {
        cerr << "多項式 B 輸入錯誤。\n";
        return 1;
    }

    do {
        cout << "\n === 選單 ===\n";
        cout << "1. A + B\n";
        cout << "2. A * B\n";
        cout << "3. 計算 A(x)\n";
        cout << "4. 計算 B(x)\n";
        cout << "0. 結束程式\n";
        cout << "選擇: ";

        if (!(cin >> choice)) {
            cerr << "選單輸入錯誤。\n";
            return 1;
        }

        if (choice == 1) {
            cout << "A(x) = " << p1 << endl;
            cout << "B(x) = " << p2 << endl;
            cout << "A + B = " << p1.Add(p2) << endl;
        } else if (choice == 2) {
            cout << "A(x) = " << p1 << endl;
            cout << "B(x) = " << p2 << endl;
            cout << "A * B = " << p1.Mult(p2) << endl;
        } else if (choice == 3) {
            float x;
            cout << "輸入 x: ";

            if (!(cin >> x)) {
                cerr << "x 輸入錯誤。\n";
                return 1;
            }

            cout << "A(" << x << ") = " << p1.Eval(x) << endl;
        } else if (choice == 4) {
            float x;
            cout << "輸入 x: ";

            if (!(cin >> x)) {
                cerr << "x 輸入錯誤。\n";
                return 1;
            }

            cout << "B(" << x << ") = " << p2.Eval(x) << endl;
        } else if (choice != 0) {
            cout << "沒有這個選項。\n";
        }
    } while (choice != 0);

    return 0;
}
