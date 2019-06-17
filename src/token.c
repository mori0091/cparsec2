/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define DEFINE_TOKEN(T)                                                  \
  PARSER(T) TOKEN(T)(PARSER(T) p) {                                      \
    return skip1st(spaces, p);                                           \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_P1(TOKEN);
