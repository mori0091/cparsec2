/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#include "cons.h"
#include "many.h"

#define MANY1(T) CAT(many1_, T)

#define DECLARE_MANY1(T) PARSER(List(T)) MANY1(T)(PARSER(T) p)

#define DEFINE_MANY1(T)                                                  \
  PARSER(List(T)) MANY1(T)(PARSER(T) p) {                                \
    return CONS(T)(p, MANY(T)(p));                                       \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_MANY1, TYPESET(0));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define many1(p) cxx_many1(parser_cast(p))
#define DEFINE_CXX_MANY1(T)                                              \
  inline auto cxx_many1(PARSER(T) p) {                                   \
    return MANY1(T)(p);                                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_MANY1, TYPESET(0));

#else

#define many1(p)                                                         \
  (GENERIC_P(PARSER_CAST(p), MANY1, TYPESET(0))(PARSER_CAST(p)))

#endif
