# 41443129

姓名：孫益康  
課程：資料結構（一）  
作業：Homework 1 — Ackermann function 與 Powerset

題目來源：老師提供的 `Homework 1.pptx`，第 1 頁（頁碼 1-1），Problem 1、Problem 2。  
繳交格式依據：[NFU 資料結構作業規範](https://github.com/NFU-OpenDataStructure/NFU-DS-Instruction)。

## 解題說明

本節對應「解題說明」15%。

### Problem 1：問題描述

對非負整數 $m,n$，Ackermann 函數定義為：

$$
A(m,n)=
\begin{cases}
n+1, & m=0,\\
A(m-1,1), & m>0\text{ 且 }n=0,\\
A(m-1,A(m,n-1)), & m>0\text{ 且 }n>0.
\end{cases}
$$

題目要求分別實作遞迴函式與非遞迴演算法，計算相同的函數。判斷時必須先檢查 $m=0$，所以 $A(0,0)=1$。

### Problem 1：解題策略

遞迴版直接按照公式分成三種情況。當兩個參數都大於零時，先求內層 $A(m,n-1)$，再將結果當成外層 $A(m-1,\cdot)$ 的第二個參數。例如：

$$
A(1,2)=A(0,A(1,1))=A(0,3)=4.
$$

非遞迴版用自行實作的陣列堆疊記錄「還沒執行的第一個參數 $m$」，變數 `n` 保存下一次計算的第二個參數，或已經完成的內層結果。

| 取出的狀態 | 處理方式 |
|---|---|
| `m == 0` | 把 `n` 加一，得到目前子問題的結果。 |
| `m > 0 && n == 0` | 將 `n` 設為 1，推入 `m - 1`。 |
| `m > 0 && n > 0` | 先推入外層 `m - 1`，再推入內層 `m`，最後令 `n` 減一。 |

堆疊採後進先出（LIFO），因此第三種情況會先處理內層。若推入順序顛倒，就不能保證內層結果先算出來。

以 $A(1,2)$ 為例，堆疊由左至右表示底部至頂端：

| 步驟 | 堆疊 | `n` | 說明 |
|---:|---|---:|---|
| 0 | `[1]` | 2 | 初始問題。 |
| 1 | `[0, 1]` | 1 | 留下外層 0，先計算內層。 |
| 2 | `[0, 0, 1]` | 0 | 再展開一次內層。 |
| 3 | `[0, 0, 0]` | 1 | 依照 `n == 0` 規則轉換。 |
| 4 | `[0, 0]` | 2 | 處理一個 `m == 0`。 |
| 5 | `[0]` | 3 | 再處理一個 `m == 0`。 |
| 6 | `[]` | 4 | 堆疊清空，答案為 4。 |

### Problem 2：問題描述

給定含有 $n$ 個相異元素的集合 $S$，以遞迴方式列出所有子集合，亦即冪集合 $\mathcal{P}(S)$。投影片範例為 $S=\{a,b,c\}$，應包含空集合、三個單元素集合、三個雙元素集合，以及原集合，共 $2^3=8$ 個子集合。

### Problem 2：解題策略

每個元素都有「不選」與「選入」兩種可能。函式使用 `index` 表示目前處理的位置，`selected` 記錄已選元素的索引，`selectedCount` 表示目前子集合的大小。

1. 若 `index == n`，表示所有元素都已決定，輸出目前子集合。
2. 先遞迴處理不選目前元素的分支。
3. 把目前索引存到 `selected[selectedCount]`，再遞迴處理選入的分支。

輸出順序由遞迴走訪決定，與投影片的列舉順序可以不同；集合內容必須相同。空集合輸入 $S=\varnothing$ 時，結果是只含空集合的集合 $\{\varnothing\}$，共有一個子集合。

### 輸入、輸出與實作範圍

執行程式後選 `1` 計算 Ackermann，或選 `2` 列出 Powerset。每次執行處理一題；空白與換行皆可分隔輸入。

- Ackermann 的 `m` 接受 0 至 2147483647，`n` 接受 0 至 9223372036854775807；這是輸入型別範圍，並不代表所有組合都能完成計算。
- 結果使用 `long long`，加一之前檢查是否超出 9223372036854775807。
- 遞迴版最多同時使用 1024 層呼叫；兩版各自最多執行 2000000 次狀態處理；手動堆疊最多存放 100000 個待處理參數。
- 超出上述限制會輸出明確錯誤，並以結束碼 1 結束，不把中止時的暫存值當成答案。兩個版本獨立執行，因此遞迴版達上限時，非遞迴版仍可能算出結果。
- Powerset 接受 0 至 20 個相異、以空白分隔的字串元素，例如 `a b c`、`red blue` 或 `10 20`。重複元素直接拒絕，不偷偷刪除或改變輸入。元素以字串比較，因此 `1` 和 `01` 是不同元素。
- Powerset 的 20 個元素上限是本程式為控制輸出量採取的實作限制，並非投影片另有規定。子集合以大括號與逗號顯示；示例使用不含逗號、大括號或空白的名稱以避免顯示歧義。

## 程式實作

本節對應「程式實作」30%。完整檔案為 [`src/main.cpp`](src/main.cpp)，以下內容與檔案一致。使用 C++17，且只引用老師允許清單中的 `<iostream>`、`<sstream>`、`<string>`。

```cpp
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
```

`readInteger` 會確認整個輸入 token 都是可表示的整數，避免將 `1.5` 或 `2abc` 誤當成合法整數。`IntStack` 在空間不足時倍增容量，並於解構時釋放記憶體。Powerset 的暫存陣列也會在正常完成及例外發生時釋放。

## 效能分析

本節對應「效能分析」10%。下列分析以未觸發資源限制、完整求得結果的演算法為對象；數學上的成長率與實作中的保護上限分開討論，並以固定寬度整數運算為 $O(1)$。

### Problem 1：時間複雜度

令 $C(m,n)$ 表示直接依定義計算時，總共處理的 Ackermann 呼叫數：

$$
\begin{aligned}
C(0,n)&=1,\\
C(m,0)&=1+C(m-1,1) && (m>0),\\
C(m,n)&=1+C(m,n-1)+C(m-1,A(m,n-1)) && (m,n>0).
\end{aligned}
$$

遞迴版每次呼叫做固定量的控制工作，時間為 $\Theta(C(m,n))$，因此也可寫成 $O(C(m,n))$。

非遞迴版每次迴圈對應相同遞迴展開中的一個呼叫，堆疊 push 採容量倍增，攤銷成本為 $O(1)$，pop 成本為 $O(1)$，因此總時間同樣為 $\Theta(C(m,n))$。移除函式遞迴呼叫不會消除 Ackermann 本身大量的子問題。

固定較小的 $m$ 可以更具體分析：

| 固定 $m$ | 函數值 | 此實作的時間複雜度 |
|---:|---|---|
| 0 | $A(0,n)=n+1$ | $\Theta(1)$ |
| 1 | $A(1,n)=n+2$ | $\Theta(n+1)$ |
| 2 | $A(2,n)=2n+3$ | $\Theta((n+1)^2)$ |
| 3 | $A(3,n)=2^{n+3}-3$ | $\Theta(4^n)$ |

例如 $m=2$ 每一層都要額外計算一個參數約為 $2n$ 的 $m=1$ 子問題，累加線性成本後成為平方成長。$m=3$ 會反覆呼叫輸入約為 $2^n$ 的 $m=2$ 子問題，得到 $\Theta(4^n)$。這些閉式公式只用於分析及獨立測試，程式本身仍使用題目要求的遞迴與堆疊演算法。

當 $m$ 也作為輸入增加時，不能用單一固定次數的多項式或一般 $O(2^n)$ 概括所有情況；以上述 $C(m,n)$ 的遞迴式表達一般成本較精確。

### Problem 1：空間複雜度

令 $D(m,n)$ 為最多同時存在的遞迴呼叫層數（不假設編譯器進行尾呼叫最佳化）：

$$
\begin{aligned}
D(0,n)&=1,\\
D(m,0)&=1+D(m-1,1) && (m>0),\\
D(m,n)&=1+\max\{D(m,n-1),D(m-1,A(m,n-1))\} && (m,n>0).
\end{aligned}
$$

遞迴版每層只保留固定數量的參數與區域變數，所以輔助空間為 $O(D(m,n))$。令 $S(m,n)$ 為非遞迴版堆疊同時存放的最大參數數量，陣列倍增後的容量與 $S$ 同階，擴充時的新舊陣列也只增加常數倍空間，因此輔助空間為 $O(S(m,n))$，且 $S(m,n)\leq D(m,n)$。

固定 $m=0$ 時空間為 $O(1)$；固定 $m=1,2$ 時兩版皆可用 $O(n+1)$ 表示；固定 $m=3$ 時可用 $O(2^n)$ 表示。非遞迴版仍需要堆疊空間，不能因為沒有遞迴呼叫就寫成 $O(1)$。

### Problem 2：時間與空間複雜度

共有 $2^n$ 個選擇結果，完整二元遞迴樹包含 $2^{n+1}-1$ 個節點。不考慮輸出內容，走訪成本為 $\Theta(2^n)$；但本程式會實際印出每一個子集合，因此必須計入輸出成本。

當 $n\geq1$ 時，每個元素出現在一半的子集合中，全部輸出的元素總次數為 $n2^{n-1}$。若把每個元素視為固定長度，總時間為 $\Theta(n2^n)$，亦即 $O(n2^n)$。更精確地，若最長字串長度為 $L$，重複檢查成本為 $O(n^2L)$，遞迴及輸出成本為 $O((1+nL)2^n)$。空集合的計算與輸出成本為 $O(1)$。

程式只儲存輸入陣列、目前選取的索引，以及深度最多 $n+1$ 的呼叫堆疊；不將所有子集合留在記憶體中。固定長度元素下，總空間為 $O(n+1)$；考慮字串內容則為 $O(nL+n+1)$，其中演算法額外使用的索引陣列與遞迴堆疊為 $O(n+1)$。此分析不包含終端機或接收端自行保留的歷史輸出。

## 測試與驗證

本節對應「測試與驗證」20%。以下為 2026-09-13 在 Windows、MSYS2 UCRT64 GCC 15.2.0 實際編譯執行的紀錄。

### 編譯與執行方式

在 repo 根目錄執行，下列 `$` 代表使用者在終端機輸入的指令，不是程式輸出：

```shell
$ g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror homework1/src/main.cpp -o homework1.exe
```

實際編譯結束碼為 0，沒有警告或錯誤。本次驗證時將執行檔放在 repo 外的測試資料夾；以上指令使用相同原始碼及編譯參數，僅將輸出位置改為 repo 根目錄方便重現。

在 Windows PowerShell 可直接使用下列方式提供輸入：

```powershell
$ "1 3 4" | .\homework1.exe
```

在 Linux 或 macOS，編譯輸出檔名可改為 `homework1-bin`，再使用 `printf '1\n3 4\n' | ./homework1-bin`。此為跨平台重現指令，本次本機 C++ 編譯測試環境為 Windows。

### Ackermann 測試結果

| 測試 | 輸入 $(m,n)$ | 預期答案 | 遞迴實際答案 | 非遞迴實際答案 |
|---|---|---:|---:|---:|
| 兩參數皆零 | (0,0) | 1 | 1 | 1 |
| `m == 0` | (0,5) | 6 | 6 | 6 |
| `n == 0` | (1,0) | 2 | 2 | 2 |
| 小型巢狀遞迴 | (1,2) | 4 | 4 | 4 |
| `m == 2` | (2,3) | 9 | 9 | 9 |
| 較多次呼叫 | (3,4) | 125 | 125 | 125 |
| 較深呼叫 | (3,7) | 1021 | 1021 | 1021 |
| 更高的 `m` | (4,0) | 13 | 13 | 13 |
| 線性結果、多次展開 | (2,100) | 203 | 203 | 203 |
| 最大可表示結果 | (0,9223372036854775806) | 9223372036854775807 | 9223372036854775807 | 9223372036854775807 |

輸入 `1 3 4` 的完整標準輸出：

```text
Homework 1
1. Ackermann function
2. Powerset
Choose 1 or 2:
Enter m and n (non-negative integers):
Recursive result: 125
Non-recursive result: 125
Results match: yes
```

### Powerset 測試結果

| 輸入集合 | 預期子集合數 | 實際結果 |
|---|---:|---|
| 空集合 | 1 | 只輸出 `{}`，總數 1。 |
| 一個元素 `item0` | 2 | 空集合及原集合，總數 2。 |
| 兩個元素 `item0 item1` | 4 | 4 個相異子集合，內容與獨立列舉結果一致。 |
| 投影片的 `a b c` | 8 | 8 個相異子集合，含空集合與完整集合。 |
| 10 個相異元素 | 1024 | 1024 個相異子集合，逐一比對內容無遺漏。 |

輸入 `2 3 a b c` 的完整標準輸出：

```text
Homework 1
1. Ackermann function
2. Powerset
Choose 1 or 2:
Enter the number of elements (0..20):
Enter 3 distinct elements (separated by whitespace):
Powerset:
{}
{c}
{b}
{b, c}
{a}
{a, c}
{a, b}
{a, b, c}
Total subsets: 8
```

輸入 `2 0` 的完整標準輸出：

```text
Homework 1
1. Ackermann function
2. Powerset
Choose 1 or 2:
Enter the number of elements (0..20):
Enter 0 distinct elements (separated by whitespace):
Powerset:
{}
Total subsets: 1
```

### 錯誤輸入與資源限制

| 案例 | 實際行為 | 結束碼 |
|---|---|---:|
| 空輸入、選項 3 或 1.5 | 顯示 `Error: choose 1 or 2.` | 1 |
| 負數、缺少參數、`1.5`、`2abc` 或超出整數型別範圍 | 顯示合法 `m`、`n` 範圍，不開始計算。 | 1 |
| $A(0,9223372036854775807)$ | 兩版皆顯示 `integer overflow`。 | 1 |
| $A(1,2000)$ | 遞迴版顯示深度限制；非遞迴版正確得到 2002。 | 1 |
| $A(4,1)$ | 遞迴版先達深度限制；非遞迴版達計算步數限制。 | 1 |
| $A(1,100000)$ | 遞迴版先達深度限制；非遞迴版達手動堆疊限制。 | 1 |
| 集合大小 -1、21 或 2.5 | 顯示大小必須是 0 至 20 的整數。 | 1 |
| `n=3`，輸入 `a b a` | 顯示 `set elements must be distinct`。 | 1 |
| `n=3`，只提供 `a b` 後結束輸入 | 顯示 `missing set element`。 | 1 |

以下是 `1 4 1` 的實際輸出，保護機制中止計算時沒有宣稱已取得答案：

```text
Homework 1
1. Ackermann function
2. Powerset
Choose 1 or 2:
Enter m and n (non-negative integers):
Recursive error: recursion depth limit exceeded
Non-recursive error: step limit exceeded
```

### 自動化驗證與正確性

測試程式放在 [`tests/test_homework.py`](tests/test_homework.py)，僅用 Python 3 標準函式庫；它是驗證輔助工具，繳交的演算法實作為 C++。在 repo 根目錄編譯後，可執行：

```shell
$ python homework1/tests/test_homework.py ./homework1.exe
PASS: 66 cases (35 Ackermann, 12 Powerset, 19 error/limit cases).
```

實際執行 66 組測試，全部通過，測試程序結束碼為 0。測試涵蓋 $0\leq m\leq3,0\leq n\leq7$ 的 32 組 Ackermann 輸入，再加入 $(4,0)$、$(2,100)$ 與最大可表示結果；測試期望值使用閉式公式或已知答案，避免只檢查兩版相等卻同時算錯。

Powerset 使用 Python 的組合列舉作為獨立期望值，對 $n=0$ 至 $10$ 逐一檢查內容、數量及唯一性，再驗證投影片範例。19 組錯誤及上限測試另檢查非零結束碼與錯誤訊息。

有限測試不能取代一般性論證：遞迴 Ackermann 的三個分支直接對應定義；非遞迴版的堆疊保存待完成的外層計算，三種狀態轉換分別保留相同運算意義，當堆疊清空時 `n` 即為最外層結果。Powerset 的每條根到葉路徑對應唯一的選取決策序列，因此每個子集合恰好輸出一次，總數為 $2^n$。

老師的 GitHub Actions 執行 `.github/scripts/validator_linux`，用途是報告格式檢查；上述 C++ 編譯與 66 組功能測試另行實際執行。線上檢查結果可由 [Actions 頁面](https://github.com/ccbee-15/NFU-DataStructure-Homework-2026/actions) 查閱。

## 申論及開發報告

本節對應「申論及開發報告」25%。

### 資料結構與演算法選擇

Ackermann 的定義本來就含有函數自我呼叫，遞迴版能清楚對照數學上的基底條件與巢狀計算。非遞迴版需要保存尚未完成的外層，因此選擇後進先出的堆疊；本題只需保存第一個參數 `m`，第二個參數或內層結果由共同變數 `n` 傳遞，不必保存完整函式框架。

依據上學期標頭限制，沒有使用 `<stack>`、`<vector>` 或 `<set>`。手動陣列堆疊由 16 個位置開始、必要時倍增，兼顧可讀性及攤銷效率。限制前的容量與實際最大需求同階，不必為每次小輸入預先配置最大容量。

Powerset 使用遞迴選取法，是因為「每個元素選或不選」能直接對應題目。`selectedCount` 以傳值方式表示有效區間，回到上一層後不需要清空整個陣列，後續選入時覆寫對應位置即可。逐個輸出子集合，可以避免把全部 $2^n$ 個結果存進記憶體。

### 開發與驗證過程

先核對投影片第 1 頁的兩個 Problem，再核對官方 repo 的五大章節、程式碼附件與標頭規定。完成三個主要函式後，以編譯器的警告檢查確認程式可編譯，再利用獨立期望值測試，最後把完整來源碼、實測結果與分析整合進本報告。

本次程式、測試及報告整理由 OpenAI Codex 協助完成。開發紀錄與測試輸出依實際執行結果記載；本報告不將工具代為執行的工作描述為學生已獨立完成或已理解。

### 設計上需要注意的地方

1. Ackermann 在 `m=0,n=0` 時，要優先使用第一個分支，避免把 `m` 減成負數。
2. 非遞迴版必須先推外層、後推內層；`n` 在每次內層完成後才能作為外層輸入。
3. Ackermann 的答案大小、呼叫總次數與最大堆疊深度是不同量；例如 $(4,1)$ 的答案雖可用 `long long` 表示，直接依定義計算仍可能先達資源限制。
4. Powerset 應包含空集合，且輸入元素需要相異。空集合不是「沒有輸出」，而是輸出一個空子集合。
5. 效能分析要計入輸出。Powerset 列出 $2^n$ 個子集合，若每個子集合還要印出所含元素，就不能只寫 $O(2^n)$ 而忽略字串輸出。

### 限制與可能改進

本作業以呈現遞迴與堆疊轉換為主，沒有使用 Ackermann 閉式公式或記憶化去取代指定演算法。若要支援更大的輸入，可另外研究任意精度整數及避免重複計算的方式，但仍要評估運算次數與記憶體需求；單純改為非遞迴或加大整數型別，無法保證所有輸入都能完成。

Powerset 的限制主要來自結果數量，20 個元素就有 1048576 個子集合。若需要較大集合，應先確定是否真的需要列出全部結果，或改成依需求逐筆消費輸出的方式。現有實作已逐筆產生結果，但終端機大量顯示仍可能很慢。
