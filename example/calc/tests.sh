#!/bin/sh

CMD=./bin/calc

pass=0
fail=0

assert() {
    expect=$1
    op=$2
    shift 2
    actual=$("$@")
    test "$expect" "$op" "$actual" || {
        echo "[FAIL] $expect $op $@"
        echo "    expects \"$expect\""
        echo "    but was \"$actual\""
        fail=$((fail + 1))
        return 1
    }
    if [ "$verbose" ] ; then
        echo "[PASS] $expect $op $@"
    fi
    pass=$((pass + 1))
}

assert  0 = ${CMD} '0'
assert 42 = ${CMD} '42'
assert 13 = ${CMD} '2+11'
assert 21 = ${CMD} '5+20-4'
assert 21 = ${CMD} '5 + 20 - 4'
assert 41 = ${CMD} ' 12 + 34 - 5 '
assert  7 = ${CMD} '1+2*3'
assert  9 = ${CMD} '(1+2)*3'
assert 17 = ${CMD} '10 / 2 + 4 * 3'
assert  5 = ${CMD} '-1+2*3'
assert  9 = ${CMD} '-(1+2)*-3'
assert  9 = ${CMD} '(-1 + -2) * -3'
assert "error:expects <eof> but was ';'" = ${CMD} '1+2;'

echo
echo "$((pass + fail)) tests, $pass passed, $fail failed"

test $fail -eq 0
