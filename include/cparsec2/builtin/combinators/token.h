/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#include "../parsers.h"

#include "skip2nd.h"

#define TOKEN(T) CAT(token_, T)

#define DECLARE_TOKEN(T) PARSER(T) TOKEN(T)(PARSER(T) p)

#define DEFINE_TOKEN(T)                                                  \
  PARSER(T) TOKEN(T)(PARSER(T) p) {                                      \
    return SKIP2ND(T)(p, spaces);                                        \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_TOKEN, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define token(p) cxx_token(parser_cast(p))
#define DEFINE_CXX_TOKEN(T)                                              \
  inline auto cxx_token(PARSER(T) p) {                                   \
    return TOKEN(T)(p);                                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_TOKEN, TYPESET(1));

#else

#define token(p)                                                         \
  (GENERIC_P(PARSER_CAST(p), TOKEN, TYPESET(1))(PARSER_CAST(p)))

#endif
