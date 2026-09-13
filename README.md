# 資料結構（一）作業

41443129 孫益康

## Homework 1

- [作業報告](homework1/report.md)：兩題解題說明、完整程式碼、效能分析、實測結果及開發報告。
- [C++ 程式](homework1/src/main.cpp)：Ackermann 遞迴與非遞迴版本，以及遞迴 Powerset。
- [測試程式](homework1/tests/test_homework.py)：66 組功能、邊界及錯誤處理測試。
- [老師的格式檢查結果](https://github.com/ccbee-15/NFU-DataStructure-Homework-2026/actions)。

在 repo 根目錄編譯：

```shell
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror homework1/src/main.cpp -o homework1.exe
```

Windows PowerShell 範例：

```powershell
"1 3 4" | .\homework1.exe
"2 3 a b c" | .\homework1.exe
python homework1/tests/test_homework.py ./homework1.exe
```

每次執行先選題目 `1` 或 `2`，再提供參數；也可直接執行後依提示輸入。程式的輸入及資源上限詳見報告。

此 repo fork 自 [老師的官方範本](https://github.com/NFU-OpenDataStructure/Homework-template)。`.github` 與 `homework-template` 保留原樣；本次提交的作業放在根目錄下的 `homework1`。
