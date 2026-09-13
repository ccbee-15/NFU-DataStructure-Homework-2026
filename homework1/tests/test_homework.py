"""Run against an already compiled Homework 1 executable (Python 3 standard library)."""
import itertools
import json
from pathlib import Path
import re
import subprocess
import sys

exe = Path(sys.argv[1]).resolve()
results = []


def run(label, data, expected_code=0):
    result = subprocess.run([str(exe)], input=data, text=True,
                            capture_output=True, timeout=10)
    assert result.returncode == expected_code, (label, result.returncode,
                                                 result.stdout, result.stderr)
    results.append({"case": label, "input": data, "exit_code": result.returncode,
                    "stdout": result.stdout, "stderr": result.stderr})
    return result.stdout + result.stderr


# Independent closed forms, not a second copy of the implemented recurrence.
for m in range(4):
    for n in range(8):
        expected = [n + 1, n + 2, 2 * n + 3, 2 ** (n + 3) - 3][m]
        out = run(f"Ackermann({m},{n})", f"1\n{m} {n}\n")
        assert f"Recursive result: {expected}\n" in out
        assert f"Non-recursive result: {expected}\n" in out
        assert "Results match: yes\n" in out

for m, n, expected in [(4, 0, 13), (2, 100, 203),
                       (0, 9223372036854775806, 9223372036854775807)]:
    out = run(f"Ackermann({m},{n})", f"1\n{m} {n}\n")
    assert f"Recursive result: {expected}\n" in out
    assert f"Non-recursive result: {expected}\n" in out

for n in range(11):
    elements = [f"item{i}" for i in range(n)]
    out = run(f"Powerset n={n}", f"2\n{n}\n" + " ".join(elements) + "\n")
    actual = [tuple(line[1:-1].split(", ")) if line != "{}" else ()
              for line in out.splitlines() if line.startswith("{")]
    expected = {combination for k in range(n + 1)
                for combination in itertools.combinations(elements, k)}
    assert len(actual) == 2 ** n
    assert len(set(actual)) == len(actual)
    assert set(actual) == expected
    assert f"Total subsets: {2 ** n}\n" in out

out = run("Powerset slide example", "2\n3\na b c\n")
assert "Powerset:\n{}\n{c}\n{b}\n{b, c}\n{a}\n{a, c}\n{a, b}\n{a, b, c}\nTotal subsets: 8\n" in out

invalid = [
    ("empty input", "", "choose 1 or 2"),
    ("invalid choice", "3\n", "choose 1 or 2"),
    ("fractional choice", "1.5\n", "choose 1 or 2"),
    ("negative m", "1\n-1 0\n", "m must be"),
    ("negative n", "1\n1 -1\n", "m must be"),
    ("missing Ackermann n", "1\n2\n", "m must be"),
    ("fractional m", "1\n1.5 0\n", "m must be"),
    ("n with suffix", "1\n1 2abc\n", "m must be"),
    ("m out of range", "1\n2147483648 0\n", "m must be"),
    ("n out of range", "1\n0 9223372036854775808\n", "m must be"),
    ("result overflow", "1\n0 9223372036854775807\n", "integer overflow"),
    ("recursive depth guard", "1\n1 2000\n", "recursion depth limit exceeded"),
    ("work budget guard", "1\n4 1\n", "step limit exceeded"),
    ("explicit stack guard", "1\n1 100000\n", "explicit stack limit exceeded"),
    ("negative set size", "2\n-1\n", "integer in [0, 20]"),
    ("oversized set", "2\n21\n", "integer in [0, 20]"),
    ("fractional set size", "2\n2.5\n", "integer in [0, 20]"),
    ("duplicate elements", "2\n3\na b a\n", "set elements must be distinct"),
    ("missing element", "2\n3\na b\n", "missing set element"),
]
for label, data, expected in invalid:
    out = run(label, data, expected_code=1)
    assert expected in out, (label, out)
    if label == "result overflow":
        assert out.count("integer overflow") == 2
    if label == "recursive depth guard":
        assert "Non-recursive result: 2002\n" in out

print(f"PASS: {len(results)} cases (35 Ackermann, 12 Powerset, 19 error/limit cases).")
if len(sys.argv) > 2:
    Path(sys.argv[2]).write_text(json.dumps(results, ensure_ascii=False, indent=2),
                                encoding="utf-8")
