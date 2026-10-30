# 41443129

作業二

班級：資工二甲  
學號：41443129  
姓名：孫益康

## 解題說明

這次作業按照老師「多項式物件」投影片的框架完成 Polynomial 類別，內容包含多項式加法、乘法和代入 x 計算，第二題的輸入輸出多載也一起放在同一支程式裡。

輸入兩個多項式後，程式會顯示選單。選項 1 是加法，2 是乘法，3 和 4 分別代入 A、B，選項 0 結束程式。

每一項用 Term 儲存，裡面有係數 coef 和指數 exp。Polynomial 用 termArray 存這些項目，capacity 是陣列容量，terms 是現在有幾項。項目要按照指數由大到小放，例如 `3x^2 + 2x - 5` 會存成 `(3, 2)`、`(2, 1)`、`(-5, 0)`。

newTerm 會把新的一項放到陣列後面。陣列滿的時候，先把 capacity 乘以 2，再建立新的陣列，把原本的內容複製過去。輸入完成後，sortTerms 會按照次方由大到小排列，也會合併相同次方。Add 用 aPos 和 bPos 比較兩個多項式的指數，相同就把係數相加，不同就先放指數比較大的項目。

Mult 用兩層迴圈做每一項的相乘，係數相乘、指數相加，再用 Add 合併同次方。Eval 就是把 x 代入每一項後加起來。

輸入格式是先輸入項數，後面再輸入每一項的係數和指數。例如：

```text
3 2 2 -1 1 5 0
```

這代表 `2X^2-X+5`。輸入順序可以不同，程式讀完後會自動按照指數由大到小排列。

因為輸入的是非零項，所以項數不能是負數，每一項的係數不能是 0，指數也不能是負數。如果多項式、選單或 x 的輸入格式錯誤，程式會顯示輸入錯誤後結束，避免繼續使用不正確的資料。

## 程式實作

### homew1.cpp

```cpp
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
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            ++bPos;
        } else {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            ++aPos;
        }
    }

    for (; aPos < terms; ++aPos)
        c.newTerm(termArray[aPos].coef, termArray[aPos].exp);

    for (; bPos < b.terms; ++bPos)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

    return c;
}

Polynomial Polynomial::Mult(Polynomial b) {
    Polynomial c;

    for (int i = 0; i < terms; ++i) {
        Polynomial row;
        for (int j = 0; j < b.terms; ++j)
            row.newTerm(termArray[i].coef * b.termArray[j].coef,
                        termArray[i].exp + b.termArray[j].exp);
        c = c.Add(row);
    }

    return c;
}

float Polynomial::Eval(float f) {
    float result = 0;
    for (int i = 0; i < terms; ++i)
        result += termArray[i].coef * pow(f, termArray[i].exp);
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
```

## 效能分析

假設 A 有 n 項，B 有 m 項。Add 只要把兩個陣列各走一次，所以時間是 O(n + m)，結果最多有 n + m 項。

Mult 要做 n × m 次單項相乘，另外還要把每一列的結果用 Add 合併。這份程式最差大約是 O(n²m)，乘積最多有 nm 項。Eval 把 A 的 n 項走一次，所以是 O(n)。

newTerm 平常直接加在陣列後面，是 O(1)。只有陣列滿的時候要複製原本的內容，那一次會是 O(k)。多項式用了多少非零項，空間就大約是 O(k)。

sortTerms 使用簡單的選擇排序，若有 k 個非零項，排序時間是 O(k²)。

## 效能量測

使用 `-O2` 編譯後，測試 10、20、40 項，單位是微秒：

| 項數 | Add | Mult | Eval |
|---:|---:|---:|---:|
| 10 | 0.144 | 3.376 | 0.003 |
| 20 | 0.206 | 13.012 | 0.003 |
| 40 | 0.324 | 63.458 | 0.006 |

項數增加時，乘法的時間增加最多，因為每一項都要跟另一個多項式的各項相乘。

## 測試與驗證

在 Windows 的 PowerShell 進入作業資料夾後，用下面的指令編譯及執行：

```powershell
chcp 65001
g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8 homew1.cpp -o homew1.exe
.\homew1.exe
```

程式先讀入 A 和 B，接著顯示選單。選項 1 做加法、2 做乘法、3 計算 A(x)、4 計算 B(x)，輸入 0 才會結束程式。

### 測資一：一般加法和乘法

```text
2 5 0 3 2
2 -5 0 2 1
1
2
3
2
4
2
0
```

主要結果：

```text
A + B = 3X^2+2X
A * B = 6X^3-15X^2+10X-25
A(2) = 17
B(2) = -1
```

### 測資二：同次方相消

```text
2 2 3 4 1
2 -2 3 1 0
1
2
3
2
4
2
0
```

主要結果：

```text
A + B = 4X+1
A * B = -4X^6-8X^4+2X^3+4X
A(2) = 24
B(2) = -15
```

### 測資三：超過初始容量

```text
4 5 4 -7 3 1 2 -4 0
1 2 1
1
2
3
1
4
1
0
```

主要結果：

```text
A + B = 5X^4-7X^3+X^2+2X-4
A * B = 10X^5-14X^4+2X^3-8X
A(1) = -5
B(1) = 2
```

### 測資四：零多項式

```text
0
0
1
2
3
3
4
3
0
```

輸出結果：

```text
A + B = 0
A * B = 0
A(3) = 0
B(3) = 0
```

### 測資五：小數係數

```text
2 1.5 2 -0.5 0
2 2 1 1 0
1
2
3
2
4
2
0
```

輸出結果：

```text
A + B = 1.5X^2+2X+0.5
A * B = 3X^3+1.5X^2-X-0.5
A(2) = 5.5
B(2) = 5
```

### 測資六：負的項數

```text
-2
```

輸出結果：

```text
多項式 A 輸入錯誤。
```

項數不可能是負數，所以程式不再把它當成零多項式，而是直接顯示錯誤並結束。

### 測資七：係數為 0

```text
1 0 2
```

輸出結果：

```text
多項式 A 輸入錯誤。
```

題目的輸入是非零項，既然這一項的係數是 0，就不應該算在項數裡面。

### 測資八：負次方

```text
1 2 -1
```

輸出結果：

```text
多項式 A 輸入錯誤。
```

這份作業處理一般多項式，指數必須是大於或等於 0 的整數。這項檢查也能避免把 x 代入 0 時算出不正常的結果。

### 測資九：選單輸入錯誤

先輸入兩個零多項式，再輸入不存在的數字選項：

```text
0
0
9
0
```

輸入 9 時會顯示：

```text
沒有這個選項。
```

接著會回到選單，仍然可以輸入 0 正常結束。如果把 9 改成英文字母 q，輸出結果是：

```text
選單輸入錯誤。
```

### 測資十：x 輸入錯誤

```text
1 1 1
0
3
q
```

輸出結果：

```text
x 輸入錯誤。
```

這是確認選擇代入功能後，如果 x 不是數字，程式不會繼續拿錯誤的內容計算。

## 申論及開發報告

這題一開始先完成 Term、Polynomial 和 newTerm，確認動態陣列可以正常增加容量，再做 Add。Add 的 aPos 和 bPos 要分別記住兩個多項式走到哪裡，指數相同時才把係數加起來。剛開始測試時，我輸入的次方本來就是由大到小，所以答案看起來沒有問題。後來把輸入順序打亂，才知道不能假設使用者一定會照順序輸入，因此又補上 sortTerms，讓每次輸入完成後都先排序並合併同次方。

乘法先做出一列單項乘積，再用 Add 加到結果。這樣可以直接利用已經完成的加法處理同次方。輸入和輸出用 `>>`、`<<` 完成，main 讀入 A、B 後用選單決定要執行哪一個功能。

動態陣列如果只複製指標，兩個物件會指到同一個地方，所以程式還要處理複製建構、指定和解構。這部分一開始比較容易出錯，完成後 Add 和 Mult 回傳 Polynomial 物件才不會有記憶體問題。

第一次把正常的加法、乘法和代入跑完時，我原本以為程式已經完成了。後來拿給學長看，學長提醒我不能只測會成功的資料，還要故意輸入不合理的內容。他叫我試負的項數、0 係數、負次方和英文字母，我實際輸入後才發現原本的程式確實有漏洞。

負的項數原本會讓 for 迴圈一次都不執行，最後反而被當成零多項式。負次方原本也會直接存進陣列，如果是 `2X^-1` 又把 x 代入 0，結果可能會出現無限大的數值。另一個問題是 `cin` 讀到英文字母時會進入失敗狀態，如果程式沒有檢查，後面的變數就不能正常讀取，選單也可能一直重複。

我後來在輸入多載裡面檢查項數、係數和指數，讀取失敗時就把 stream 設成失敗狀態。讀多項式時先用暫時的 Polynomial 儲存，全部資料正確才指定給原本的物件，這樣輸入到一半發生錯誤時，不會留下一半的內容。main 裡面的選單和 x 也都檢查 `cin`，如果輸入格式錯誤就顯示訊息並結束。數字選項只是超出 0 到 4 時，則顯示沒有這個選項，再讓使用者重新選擇。

改完這些地方後，我把前面五組正常測資重新跑過一次，確認加法、乘法、排序、動態陣列和代入的答案沒有因為修改輸入檢查而改變，再另外跑負項數、0 係數、負次方、錯誤選單和錯誤 x。這次讓我發現程式能算出答案不代表完全正確，輸入不照預期時會發生什麼事也要自己測。

最後把程式移到 Windows 時還遇到兩個環境問題。第一次 VS Code 選到 gcc，連結時找不到 `cout` 和 `cerr`，改用 g++ 才能正常編譯。之後中文輸出又出現亂碼，所以把終端機代碼頁設成 65001，並在編譯時指定 UTF-8。這些不是多項式計算本身的錯誤，但也讓我知道同一份程式換到不同電腦時，編譯器和文字編碼都要確認。
