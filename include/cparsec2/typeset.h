/* -*- coding:utf-8-unix -*- */
#pragma once

#include "macros.h"

#define ELEMENT_TYPESET TYPESET(0), Ptr
#define TYPESET(n) Char, CAT(TYPESET_, n)()
#define TYPESET_0()                                                      \
  SQUASH(String, Int, None, Node, CPARSEC2_USER_TYPESET_I())
#define TYPESET_1() TYPESET_0(), APPLY(List, TYPESET_0())

#ifdef CPARSEC2_USER_TYPESET

#define CPARSEC2_USER_TYPESET_I() CPARSEC2_USER_TYPESET

#define CPARSEC2_DEFINE_USER_TYPESET()                                   \
  CPARSEC2_C_API_BEGIN                                                   \
  FOREACH(DEFINE_LIST, CPARSEC2_USER_TYPESET_0());                       \
  FOREACH(DEFINE_BUFF, CPARSEC2_USER_TYPESET_0());                       \
  FOREACH(DEFINE_SHOW_LIST, CPARSEC2_USER_TYPESET_0());                  \
  FOREACH(DEFINE_PARSER, CPARSEC2_USER_TYPESET_1());                     \
  FOREACH(DEFINE_RUNPARSER, CPARSEC2_USER_TYPESET_1());                  \
  FOREACH(CPARSEC2_DEFINE_COMBINATORS_0, CPARSEC2_USER_TYPESET_0());     \
  FOREACH(CPARSEC2_DEFINE_COMBINATORS_1, CPARSEC2_USER_TYPESET_1());     \
  CPARSEC2_C_API_END                                                     \
  END_OF_STATEMENTS

#define CPARSEC2_USER_TYPESET_0() SQUASH(CPARSEC2_USER_TYPESET)
#define CPARSEC2_USER_TYPESET_1()                                        \
  CPARSEC2_USER_TYPESET_0(), APPLY(List, CPARSEC2_USER_TYPESET_0())

#else

#define CPARSEC2_USER_TYPESET_I()

#define CPARSEC2_DEFINE_USER_TYPESET()                                   \
  static_assert(0, "CPARSEC2_USER_TYPESET is not defined.")

#endif

// Parser-combinators which
// - takes a PARSER(T) and constructs a PARSER(List(T))
#define CPARSEC2_DEFINE_COMBINATORS_0(T)                                 \
  DEFINE_MANY(T);                                                        \
  DEFINE_MANY1(T);                                                       \
  DEFINE_CONS(T);                                                        \
  DEFINE_SEQ(T)

// Parser-combinators which
// - takes a PARSER(T) and constructs a PARSER(T), or
// - takes a PARSER(List(T)) and constructs a PARSER(List(T))
#define CPARSEC2_DEFINE_COMBINATORS_1(T)                                 \
  DEFINE_EXPECTS(T);                                                     \
  DEFINE_SKIP(T);                                                        \
  DEFINE_SKIP1ST(T);                                                     \
  DEFINE_SKIP2ND(T);                                                     \
  DEFINE_EITHER(T);                                                      \
  DEFINE_TRYP(T);                                                        \
  DEFINE_TOKEN(T)
