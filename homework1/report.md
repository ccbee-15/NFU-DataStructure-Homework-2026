# 41443129

作業一

## 解題說明

### 第一題：Ackermann 函數

這題要用遞迴和非遞迴兩種方式計算 Ackermann 函數，輸入是非負整數 m 和 n。

遞迴版的函式叫 A，照題目分成三種情況：

1. m 是 0，直接回傳 n + 1。
2. m 大於 0、n 是 0，計算 A(m - 1, 1)。
3. m 和 n 都大於 0，先算 A(m, n - 1)，把結果存到 x，再算 A(m - 1, x)。

例如 A(1, 1)，要先算 A(1, 0) 得到 2，再算 A(0, 2)，最後得到 3。

非遞迴版的函式叫 B，用陣列 s 當堆疊，t 表示目前裡面有幾個數。每次從堆疊取出一個 m，再用迴圈處理。

如果 m 是 0，就讓 n 加一。如果 n 是 0，就把 n 改成 1，並把 m - 1 放回堆疊。其他情況則先放 m - 1，再放 m，並讓 n 減一。這樣後放的 m 會先被取出，先算完內層，再繼續算外層。堆疊清空後，n 就是答案。

### 第二題：冪集合

這題要用遞迴印出一個集合的所有子集合，包含空集合。

函式 P 每次處理一個元素，分成不選和選入兩種情況。s 是原本的元素，b 放目前選到的元素，i 是處理到的位置，k 是已選的數量。當 i 等於 n，就代表全部元素都決定好了，可以印出 b 裡的內容。

例如 a、b、c 各有選或不選兩種可能，所以會有 2 × 2 × 2 = 8 個子集合。

這版用 char 儲存元素，因此每個元素是一個字元，最多輸入 20 個。輸入時會檢查有沒有重複，如果有，就印出 Error 並停止。

## 程式實作

### problem1.cpp

```cpp
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
```

### problem2.cpp

```cpp
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
```

## 效能分析

### 第一題

這份程式照定義一步一步計算。m 固定為 0 時，只做一次加法，時間是 O(1)。m 固定為 1 時，時間是 O(n + 1)；m 固定為 2 時，時間是 O((n + 1)^2)；m 固定為 3 時，時間是 O(4^n)。這些結果只適用於各自固定的 m，不能直接拿來表示所有 m、n 的情況。非遞迴版也處理相同的展開步驟，所以兩版的時間成長相同。

遞迴版需要保留還沒結束的函式呼叫。m 固定為 0 時，空間是 O(1)；m 固定為 1 或 2 時，是 O(n + 1)；m 固定為 3 時，是 O(2^n)。非遞迴版在這份程式固定配置 s[10000]，配置空間是 O(1)，但最多只能存 10000 個待處理的 m。

### 第二題

n 個元素會有 2^n 個子集合，每個子集合最多印出 n 個元素，所以時間複雜度是 O(n × 2^n)。重複元素的檢查需要 O(n^2)，整體仍以列出子集合的時間為主。

遞迴最多有 n + 1 層，只保留目前選到的元素，所以輔助空間複雜度是 O(n + 1)。s 和 b 在程式中各固定配置 20 個位置，沒有把所有子集合存起來。

## 測試與驗證

在 Windows PowerShell 的 Homework1 資料夾編譯和執行。下面的 $ 表示輸入的指令。

### 第一題

```shell
$ g++ -std=c++17 problem1.cpp -o problem1.exe
$ .\problem1.exe
Enter m and n: 3 4
A: 125
B: 125
```

| m | n | 預期答案 | A 的結果 | B 的結果 |
|---:|---:|---:|---:|---:|
| 0 | 0 | 1 | 1 | 1 |
| 0 | 3 | 4 | 4 | 4 |
| 1 | 0 | 2 | 2 | 2 |
| 1 | 1 | 3 | 3 | 3 |
| 2 | 3 | 9 | 9 | 9 |
| 3 | 4 | 125 | 125 | 125 |

這幾組測試中，A 和 B 都得到預期答案。

### 第二題

```shell
$ g++ -std=c++17 problem2.cpp -o problem2.exe
$ .\problem2.exe
Number of elements: 3
Enter elements: a b c
Powerset:
{}
{c}
{b}
{b, c}
{a}
{a, c}
{a, b}
{a, b, c}
```

| 數量 | 輸入元素 | 預期結果 | 實際結果 |
|---:|---|---|---|
| 3 | a b c | 8 個子集合 | 印出 8 個，包含空集合和原集合。 |
| 2 | a b | 4 個子集合 | 印出 {}、{b}、{a}、{a, b}。 |
| 0 | 不需輸入 | 1 個空子集合 | 印出 {}。 |
| 3 | a a b | 顯示重複輸入錯誤 | 印出 Error 並停止。 |

數量輸入 0 時，不需要再輸入元素，所以兩個提示接在同一行：

```text
Number of elements: 0
Enter elements: Powerset:
{}
```

重複元素的輸出：

```text
Number of elements: 3
Enter elements: a a b
Error
```

## 申論及開發報告

第一題的遞迴版照公式寫，比較容易對照題目的三種情況。非遞迴版要記住還沒算完的部分，所以使用堆疊。放入順序要注意，先放外層、再放內層，才能先算內層。

第二題用遞迴分成選與不選兩條路，每次決定一個元素。全部決定完就印出一個子集合，不需要另外存下全部結果。

第二題先做讀取元素，再加入遞迴，最後檢查重複元素。空集合也有一個子集合，所以 n 是 0 時，仍然會印出 {}。

第一題目前只測試小型非負整數，沒有加入負數、整數溢位或堆疊容量的檢查。數字太大可能算很久或超出容量。第二題只處理最多 20 個單一字元。兩支程式都需要依提示輸入資料，還沒有完整處理非數字或資料不足的輸入。
