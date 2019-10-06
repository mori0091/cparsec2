/* -*- coding: utf-8-unix -*- */

_Static_assert(1, "PARSER(Int) and SHOW(Int)(int x) are already defined.");

#if 0

#include "IntParser.h"

/* Defines (implement) functions/methods for PARSER(Int) */
DEFINE_PARSER(Int);
/* implementation of void SHOW(Int)(int x) */
SHOW(Int)(int x) {
  printf("%d", x);
}

#endif
