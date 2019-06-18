/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define DEFINE_MANY1(T)                                                  \
  PARSER(List(T)) MANY1(T)(PARSER(T) p) {                                \
    return cons(p, many(p));                                             \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_FORALL_P0(MANY1);
