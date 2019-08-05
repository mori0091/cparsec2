#!/bin/sh

CMD=./bin/excc
CC=${CC:-gcc}

compile_and_run() {
    "$@" > tmp.s 2> err.log || {
        cat err.log
        return 127
    }
    ${CC} ${CFLAGS} -o tmp tmp.s ${SRCS} ${LDFLAGS} ${LDLIBS} || {
        return 127
    }
    ./tmp
    echo $?
}

pass=0
fail=0

assert() {
    expect=$1
    op=$2
    shift 2
    actual=$(compile_and_run "$@")
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

assert  0 = ${CMD} '0;'
assert 42 = ${CMD} '42;'
assert 13 = ${CMD} '2+11;'
assert 21 = ${CMD} '5+20-4;'
assert 21 = ${CMD} '5 + 20 - 4;'
assert 41 = ${CMD} ' 12 + 34 - 5 ;'
assert  7 = ${CMD} '1+2*3;'
assert  9 = ${CMD} '(1+2)*3;'
assert 17 = ${CMD} '10 / 2 + 4 * 3;'
assert  5 = ${CMD} '-1+2*3;'
assert  9 = ${CMD} '-(1+2)*-3;'
assert  9 = ${CMD} '(-1 + -2) * -3;'
assert "error:expects <eof> but was '$'" = ${CMD} '1+2;$'
assert 13 = ${CMD} '14; 2+11;'
assert 1 = ${CMD} '100 == 100;'
assert 0 = ${CMD} '101 == 100;'
assert 0 = ${CMD} '100 != 100;'
assert 1 = ${CMD} '101 != 100;'
assert 0 = ${CMD} '101 < 100;'
assert 0 = ${CMD} '100 < 100;'
assert 1 = ${CMD} ' 99 < 100;'
assert 0 = ${CMD} '101 <= 100;'
assert 1 = ${CMD} '100 <= 100;'
assert 1 = ${CMD} ' 99 <= 100;'
assert 1 = ${CMD} '101 > 100;'
assert 0 = ${CMD} '100 > 100;'
assert 0 = ${CMD} ' 99 > 100;'
assert 1 = ${CMD} '101 >= 100;'
assert 1 = ${CMD} '100 >= 100;'
assert 0 = ${CMD} ' 99 >= 100;'
assert 10 = ${CMD} 'a = 10;'
assert 20 = ${CMD} 'a = b = 10; a+b;'
assert 30 = ${CMD} 'a = b = 10; b = b + 10; a + b;'
assert "error:lvalue of assignment is not a variable" \
       = ${CMD} '1 + 9 = 10;'
assert "error:lvalue of assignment is not a variable" \
       = ${CMD} '1 + a = 9 == 10;'
assert 1 = ${CMD} '1 + (a = 9) == 10;'
assert 30 = ${CMD} 'abc1 = _bcd32 = 10; 2 * abc1 + _bcd32;'
assert 15 = ${CMD} 'return 15;'
assert 15 = ${CMD} 'a = 10; return a+5;'
assert 15 = ${CMD} 'a = 10; return 15; a;'
assert 15 = ${CMD} 'return+15;'
assert "error:expected identifier or '(' but was 'return'" \
       = ${CMD} 'return;'
assert 15 = ${CMD} 'returnx=15;'
assert "error:expected identifier or '(' but was 'return'" \
       = ${CMD} 'return=15;'
assert 45 = ${CMD} '
  sum = 0;
  for (i=0; i<10; i=i+1)
    sum = sum + i;
  sum;
'
assert 220 = ${CMD} '
  sum = 0;
  for (i=0; i<4; i=i+1)
    for (j=1; j<=10; j=j+1)
      sum = sum + j;
  return sum;
'
assert 64 = ${CMD} '
  x = 1;
  while (x < 64)
    x = x * 2;
  x;
'
assert 10 = ${CMD} '
  x = 0;
  if (x < 10)
    x = 10;
  x;
'
assert 20 = ${CMD} '
  x = 0;
  if (x > 10)
    x = 10;
  else
    x = 20;
  x;
'
assert 10 = ${CMD} '
  x = 0;
  if (x < 10)
    x = 10;
  else
    x = 20;
  x;
'
assert 10 = ${CMD} '
  x = 0;
  while (1)
    if (x < 10)
      x = x + 1;
    else
      return x;
'
assert 10 = ${CMD} '
  x = 0;
  for (;;)
    if (x < 10)
      x = x + 1;
    else
      return x;
'
assert 10 = ${CMD} '
  x = 0;
  for (;;)
    if (10 <= x)
      return x;
    else
      x = x + 1;
'
assert 30 = ${CMD} '
  {
    10;
    20;
    30;
  }
'
assert 55 = ${CMD} '
  sum = 0;
  for (i = 0; ; i = i + 1 ) {
    sum = sum + i;
    if (10 <= i) {
      return sum;
    }
  }
'

NEWLINE='
'
SRCS=test_func.c assert "OK${NEWLINE}0" = ${CMD} '
  printOK();
'
SRCS=test_func.c assert "30${NEWLINE}0" = ${CMD} '
  printSum(10, 20);
'
SRCS=test_func.c assert "12345678${NEWLINE}0" = ${CMD} '
  print8(1,2,3,4,5,6,7,8);
'

echo
echo "$((pass + fail)) tests, $pass passed, $fail failed"

test $fail -eq 0
