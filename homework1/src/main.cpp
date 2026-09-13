#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const long long MAX_VALUE = 9223372036854775807LL;
const long long MAX_STEPS = 2000000;
const int MAX_DEPTH = 1024;
const int MAX_PENDING = 100000;
const int MAX_ELEMENTS = 20;

// Read one complete integer token; reject inputs such as 1.5 or 3abc.
bool readInteger(long long& value) {
    string token;
    if (!(cin >> token)) return false;
    istringstream parser(token);
    parser >> value;
    return !parser.fail() && parser.eof();
}

long long increment(long long n) {
    if (n == MAX_VALUE) throw "integer overflow";
    return n + 1;
}

// Problem 1: direct translation of the three cases in the definition.
long long ackermannRecursive(int m, long long n, long long& steps,
                            int depth = 1) {
    if (depth > MAX_DEPTH) throw "recursion depth limit exceeded";
    if (++steps > MAX_STEPS) throw "step limit exceeded";
    if (m == 0) return increment(n);
    if (n == 0) return ackermannRecursive(m - 1, 1, steps, depth + 1);
    long long inner = ackermannRecursive(m, n - 1, steps, depth + 1);
    return ackermannRecursive(m - 1, inner, steps, depth + 1);
}

// A growing array stack, implemented without <stack> or <vector>.
class IntStack {
private:
    int* data;
    int count;
    int capacity;

public:
    IntStack() : data(new int[16]), count(0), capacity(16) {}
    ~IntStack() { delete[] data; }
    IntStack(const IntStack&) = delete;
    IntStack& operator=(const IntStack&) = delete;

    bool empty() const { return count == 0; }

    void push(int value) {
        if (count == MAX_PENDING) throw "explicit stack limit exceeded";
        if (count == capacity) {
            int nextCapacity = capacity * 2;
            if (nextCapacity > MAX_PENDING) nextCapacity = MAX_PENDING;
            int* next = new int[nextCapacity];
            for (int i = 0; i < count; ++i) next[i] = data[i];
            delete[] data;
            data = next;
            capacity = nextCapacity;
        }
        data[count++] = value;
    }

    // The caller only pops after checking !empty().
    int pop() { return data[--count]; }
};

// Problem 1: each stack entry is a pending first argument m.
// n holds the input for the next call, or the last completed result.
long long ackermannNonRecursive(int m, long long n, long long& steps) {
    IntStack pending;
    pending.push(m);
    while (!pending.empty()) {
        if (++steps > MAX_STEPS) throw "step limit exceeded";
        m = pending.pop();
        if (m == 0) {
            n = increment(n);
        } else if (n == 0) {
            n = 1;
            pending.push(m - 1);
        } else {
            // LIFO: evaluate A(m,n-1) before the outer A(m-1,...).
            pending.push(m - 1);
            pending.push(m);
            --n;
        }
    }
    return n;
}

int runAckermann() {
    long long inputM, n;
    cout << "Enter m and n (non-negative integers):\n";
    if (!readInteger(inputM) || !readInteger(n) || inputM < 0 ||
        inputM > 2147483647LL || n < 0) {
        cerr << "Error: m must be in [0, 2147483647] and n in "
             << "[0, 9223372036854775807].\n";
        return 1;
    }
    int m = static_cast<int>(inputM);
    long long recursive = 0, iterative = 0;
    long long recursiveSteps = 0, iterativeSteps = 0;
    bool recursiveOK = false, iterativeOK = false;
    try {
        recursive = ackermannRecursive(m, n, recursiveSteps);
        recursiveOK = true;
        cout << "Recursive result: " << recursive << '\n';
    } catch (const char* error) {
        cout << "Recursive error: " << error << '\n';
    }
    try {
        iterative = ackermannNonRecursive(m, n, iterativeSteps);
        iterativeOK = true;
        cout << "Non-recursive result: " << iterative << '\n';
    } catch (const char* error) {
        cout << "Non-recursive error: " << error << '\n';
    }
    if (recursiveOK && iterativeOK) {
        cout << "Results match: " << (recursive == iterative ? "yes" : "no")
             << '\n';
        return recursive == iterative ? 0 : 1;
    }
    return 1;
}

// Problem 2: two recursive branches decide whether to include each item.
void powersetRecursive(const string elements[], int n, int index,
                       int selected[], int selectedCount, long long& total) {
    if (index == n) {
        cout << '{';
        for (int i = 0; i < selectedCount; ++i) {
            if (i != 0) cout << ", ";
            cout << elements[selected[i]];
        }
        cout << "}\n";
        ++total;
        return;
    }
    powersetRecursive(elements, n, index + 1, selected, selectedCount, total);
    selected[selectedCount] = index;
    powersetRecursive(elements, n, index + 1, selected, selectedCount + 1,
                      total);
}

int runPowerset() {
    long long inputN;
    cout << "Enter the number of elements (0..20):\n";
    if (!readInteger(inputN) || inputN < 0 || inputN > MAX_ELEMENTS) {
        cerr << "Error: the number of elements must be an integer in [0, 20].\n";
        return 1;
    }
    int n = static_cast<int>(inputN);
    string* elements = new string[n];
    int* selected = nullptr;
    try {
        selected = new int[n];
        cout << "Enter " << n << " distinct elements (separated by whitespace):\n";
        for (int i = 0; i < n; ++i) {
            if (!(cin >> elements[i])) throw "missing set element";
            for (int j = 0; j < i; ++j) {
                if (elements[i] == elements[j]) throw "set elements must be distinct";
            }
        }
        cout << "Powerset:\n";
        long long total = 0;
        powersetRecursive(elements, n, 0, selected, 0, total);
        cout << "Total subsets: " << total << '\n';
    } catch (...) {
        delete[] selected;
        delete[] elements;
        throw;
    }
    delete[] selected;
    delete[] elements;
    return 0;
}

int main() {
    cout << "Homework 1\n1. Ackermann function\n2. Powerset\nChoose 1 or 2:\n";
    long long choice;
    if (!readInteger(choice) || (choice != 1 && choice != 2)) {
        cerr << "Error: choose 1 or 2.\n";
        return 1;
    }
    try {
        return choice == 1 ? runAckermann() : runPowerset();
    } catch (const char* error) {
        cerr << "Error: " << error << '\n';
    } catch (...) {
        cerr << "Error: unable to complete the computation (allocation or runtime failure).\n";
    }
    return 1;
}
