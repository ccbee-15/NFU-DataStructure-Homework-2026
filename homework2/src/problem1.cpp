#include <cmath>
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;

private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 2;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];

        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other)
            return *this;

        Term* newArray = new Term[other.capacity];
        for (int i = 0; i < other.terms; i++)
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

    void AddTerm(float coef, int exp) {
        if (coef == 0 || exp < 0)
            return;

        int pos = 0;
        while (pos < terms && termArray[pos].exp > exp)
            pos++;

        if (pos < terms && termArray[pos].exp == exp) {
            termArray[pos].coef += coef;

            if (termArray[pos].coef == 0) {
                for (int i = pos; i < terms - 1; i++)
                    termArray[i] = termArray[i + 1];
                terms--;
            }
            return;
        }

        if (terms == capacity) {
            int newCapacity = capacity * 2;
            Term* newArray = new Term[newCapacity];

            for (int i = 0; i < terms; i++)
                newArray[i] = termArray[i];

            delete[] termArray;
            termArray = newArray;
            capacity = newCapacity;
        }

        for (int i = terms; i > pos; i--)
            termArray[i] = termArray[i - 1];

        termArray[pos].coef = coef;
        termArray[pos].exp = exp;
        terms++;
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result(*this);

        for (int i = 0; i < poly.terms; i++)
            result.AddTerm(poly.termArray[i].coef, poly.termArray[i].exp);

        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float coef = termArray[i].coef * poly.termArray[j].coef;
                int exp = termArray[i].exp + poly.termArray[j].exp;
                result.AddTerm(coef, exp);
            }
        }

        return result;
    }

    float Eval(float x) {
        float answer = 0;

        for (int i = 0; i < terms; i++)
            answer += termArray[i].coef * pow(x, termArray[i].exp);

        return answer;
    }
};

int main() {
    Polynomial a, b;

    a.AddTerm(3, 2);
    a.AddTerm(5, 0);

    b.AddTerm(2, 1);
    b.AddTerm(-5, 0);

    Polynomial sum = a.Add(b);
    Polynomial product = a.Mult(b);

    float x = 2;
    cout << "A(2) = " << a.Eval(x) << '\n';
    cout << "B(2) = " << b.Eval(x) << '\n';
    cout << "(A + B)(2) = " << sum.Eval(x) << '\n';
    cout << "(A * B)(2) = " << product.Eval(x) << '\n';

    return 0;
}