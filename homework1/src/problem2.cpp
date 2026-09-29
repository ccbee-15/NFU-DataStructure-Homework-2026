#include <iostream>
using namespace std;

void P(char s[], char b[], int n, int i, int k) {
    if (i == n) {
        cout << '{';
        for (int j = 0; j < k; j++) {
            if (j > 0)
                cout << ", ";
            cout << b[j];
        }
        cout << "}\n";
        return;
    }

    P(s, b, n, i + 1, k);

    b[k] = s[i];
    P(s, b, n, i + 1, k + 1);
}

int main() {
    int n;
    char s[20];

    cout << "Number of elements: ";
    cin >> n;

    if (n < 0 || n > 20) {
        cout << "Please enter 0 to 20.\n";
        return 1;
    }

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> s[i];

        for (int j = 0; j < i; j++) {
            if (s[i] == s[j]) {
                cout << "Error\n";
                return 1;
            }
        }
    }

    char b[20];
    cout << "Powerset:\n";
    P(s, b, n, 0, 0);

    return 0;
}
