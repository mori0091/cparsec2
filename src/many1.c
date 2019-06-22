/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define DEFINE_MANY1(T)                                                  \
  PARSER(List(T)) MANY1(T)(PARSER(T) p) {                                \
    return cons(p, many(p));                                             \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_MANY1, TYPESET(0));
