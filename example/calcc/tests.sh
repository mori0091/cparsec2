#!/bin/sh

CMD=./bin/calcc
CC=${CC:-gcc}

replace_newline_with_linefeed () {
    sed -E -e 's/(\r|\r\n|\n)/\n/g'
}

compile_and_run() {
    "$@" > tmp.s 2> err.log || {
        cat err.log
        return 127
    }
    ${CC} ${CFLAGS} -o tmp tmp.s ${LDFLAGS} ${LDLIBS}
    ./tmp
    echo $?
}

pass=0
fail=0

assert() {
    expect=$1
    op=$2
    shift 2
    actual=$(compile_and_run "$@" | replace_newline_with_linefeed)
    rm -f tmp tmp.s err.log
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
assert "error:expects <endOfFile> but was ';'
Parse error:3
  unexpected ';'
  expecting <endOfFile>" = ${CMD} '1+2;'

echo
echo "$((pass + fail)) tests, $pass passed, $fail failed"

test $fail -eq 0
