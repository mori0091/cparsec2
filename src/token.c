/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define DEFINE_TOKEN(T)                                                  \
  PARSER(T) TOKEN(T)(PARSER(T) p) {                                      \
    return skip1st(spaces, p);                                           \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_TOKEN, TYPESET(1));
