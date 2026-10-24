#include <cmath>
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

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

    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
};

istream& operator>>(istream& in, Polynomial& poly) {
    int count;
    if (!(in >> count))
        return in;

    if (count < 0) {
        in.setstate(ios::failbit);
        return in;
    }

    poly.terms = 0;

    for (int i = 0; i < count; i++) {
        float coef;
        int exp;

        if (!(in >> coef >> exp))
            return in;

        if (exp < 0) {
            in.setstate(ios::failbit);
            return in;
        }

        poly.AddTerm(coef, exp);
    }

    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0)
        return out << '0';

    for (int i = 0; i < poly.terms; i++) {
        float coef = poly.termArray[i].coef;
        int exp = poly.termArray[i].exp;

        if (i == 0) {
            if (coef < 0)
                out << '-';
        } else {
            if (coef < 0)
                out << " - ";
            else
                out << " + ";
        }

        float number = coef;
        if (number < 0)
            number = -number;
        if (number != 1 || exp == 0)
            out << number;

        if (exp > 0) {
            out << 'x';
            if (exp > 1)
                out << '^' << exp;
        }
    }

    return out;
}

int main() {
    Polynomial p;

    cout << "Enter number of terms, then coefficient and exponent for each term: ";
    if (!(cin >> p)) {
        cerr << "Invalid polynomial.\n";
        return 1;
    }

    cout << "P(x) = " << p << '\n';
    return 0;
}
