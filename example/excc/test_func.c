/* -*- coding: utf-8-unix */

#include <stdio.h>

int printOK(void) {
  printf("OK\n");
  return 0;
}

int print2x(int x) {
  printf("%d\n", 2 * x);
  return 0;
}

int printSum(int x, int y) {
  printf("%d\n", x + y);
  return 0;
}

int print8(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8 ) {
  int x = 0;
  x = 10 * x + x1;
  x = 10 * x + x2;
  x = 10 * x + x3;
  x = 10 * x + x4;
  x = 10 * x + x5;
  x = 10 * x + x6;
  x = 10 * x + x7;
  x = 10 * x + x8;
  printf("%d\n", x);
  return 0;
}
