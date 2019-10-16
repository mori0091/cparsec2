/* -*- coding: utf-8-unix -*- */

// (1) include 'my_cparsec2.h'
#include "my_cparsec2.h"

// (2) Call CPARSEC2_DEFINE_USER_TYPESET() macro, which defines implementation
//     of the following classes/APIs for each U in CPARSEC2_USER_TYPESET:
//     - Buff(U),
//     - List(U),
//     - PARSER(U),
//     - PARSER(List(U)),
//     - All APIs for these classes such as `parse(p, src, ex)`, and
//     - All parser-combinators such as `many(p)`, `either(p1, p2)`, etc.
CPARSEC2_DEFINE_USER_TYPESET();

// (3) Define function `void SHOW(U)(U x)` for each U in CPARSEC2_USER_TYPESET.
void SHOW(Person)(Person x) {
  printf("<Person:.name=\"%s\", .age=%d>", x.name, x.age);
}
