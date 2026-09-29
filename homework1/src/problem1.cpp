#include <iostream>
using namespace std;

int A(int m, int n) {
    if (m == 0)
        return n + 1;
    if (n == 0)
        return A(m - 1, 1);
    int x = A(m, n - 1);
    return A(m - 1, x);
}

int B(int m, int n) {
    int s[10000];
    int t = 0;

    s[t] = m;
    t++;

    while (t > 0) {
        t--;
        m = s[t];

        if (m == 0)
            n++;
        else if (n == 0) {
            n = 1;
            s[t] = m - 1;
            t++;
        } else {
            s[t] = m - 1;
            t++;
            s[t] = m;
            t++;
            n--;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "A: " << A(m, n) << '\n';
    cout << "B: " << B(m, n) << '\n';
    return 0;
}
