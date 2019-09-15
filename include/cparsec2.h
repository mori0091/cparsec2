/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdint.h>

// ---- resource management ----
#include "cparsec2/alloc.h"
// ---- error and exception handling ----
#include "cparsec2/exception.h"
// ---- source of input character sequence ----
#include "cparsec2/source.h"

// ---- common macros ----
#include "cparsec2/macros.h"
// ---- code generators as abstract syntax tree ----
#include "cparsec2/codegen.h"
// ---- type generic list class ----
#include "cparsec2/list.h"

// ---- type generic parser class ----
#include "cparsec2/parser.h"
// ---- type generic parser invocation ----
#include "cparsec2/tgparse.h"

// ---- predicates ----
#include "cparsec2/predicate.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---- built-in parsers, parser generators ----

extern PARSER(Char) anyChar;
extern PARSER(Char) digit;
extern PARSER(Char) hexDigit;
extern PARSER(Char) octDigit;
extern PARSER(Char) lower;
extern PARSER(Char) upper;
extern PARSER(Char) alpha;
extern PARSER(Char) alnum;
extern PARSER(Char) letter;
extern PARSER(Char) space;
extern PARSER(None) spaces;
extern PARSER(Char) newline;
extern PARSER(Char) crlf;
extern PARSER(Char) endOfLine;
extern PARSER(Char) endOfFile;
extern PARSER(Char) tab;
extern PARSER(Int) number;
extern PARSER(String) anyUtf8;

PARSER(Char) range(uint8_t min, uint8_t max);
PARSER(Char) oneOf(const char* cs);
PARSER(Char) noneOf(const char* cs);
PARSER(Char) char1(char c);
PARSER(Char) satisfy(Predicate pred);
PARSER(String) string1(const char* s);
PARSER(String) utf8(const char* s);

// ---- parser combinators ----

// Parser<T> expects(const char* desc, Parser<T> p);
// TODO test
#define EXPECTS(T) CAT(expects_, T)
#define DECLARE_EXPECTS(T)                                               \
  PARSER(T) EXPECTS(T)(const char* desc, PARSER(T) p)
FOREACH(DECLARE_EXPECTS, TYPESET(1));
#define expects(desc, p)                                                 \
  (GENERIC_P(PARSER_CAST(p), EXPECTS, TYPESET(1))((desc), PARSER_CAST(p)))

// Parser<T[]> many(Parser<T> p);
#define MANY(T) CAT(many_, T)
#define DECLARE_MANY(T) PARSER(List(T)) MANY(T)(PARSER(T) p)
FOREACH(DECLARE_MANY, TYPESET(0));
#define many(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), MANY, TYPESET(0))(PARSER_CAST(p)))

// Parser<T[]> many1(Parser<T> p);
#define MANY1(T) CAT(many1_, T)
#define DECLARE_MANY1(T) PARSER(List(T)) MANY1(T)(PARSER(T) p)
FOREACH(DECLARE_MANY1, TYPESET(0));
#define many1(p)                                                         \
  (GENERIC_P(PARSER_CAST(p), MANY1, TYPESET(0))(PARSER_CAST(p)))

// Parser<T[]> seq(Parser<T> p, ...);
#define SEQ(T) CAT(seq_, T)
#define DECLARE_SEQ(T) PARSER(List(T)) SEQ(T)(void* ps[])
FOREACH(DECLARE_SEQ, TYPESET(0));
#define seq(...) SEQ_0(__VA_ARGS__, NULL)
#define SEQ_0(p, ...)                                                    \
  (GENERIC_P(p, SEQ, TYPESET(0))((void*[]){p, __VA_ARGS__}))

// Parser<T[]> cons(Parser<T> p, Parser<T[]> ps);
#define CONS(T) CAT(cons_, T)
#define DECLARE_CONS(T)                                                  \
  PARSER(List(T)) CONS(T)(PARSER(T) p, PARSER(List(T)) ps)
FOREACH(DECLARE_CONS, TYPESET(0));
#define cons(p, ps)                                                      \
  (GENERIC_P(PARSER_CAST(p), CONS, TYPESET(0))(PARSER_CAST(p),           \
                                               PARSER_CAST(ps)))

// Parser<Int> skip(Parser<T> p);
#define SKIP(T) CAT(skip_, T)
#define DECLARE_SKIP(T) PARSER(Int) SKIP(T)(PARSER(T) p)
FOREACH(DECLARE_SKIP, TYPESET(1));
#define skip(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), SKIP, TYPESET(1))(PARSER_CAST(p)))

// Parser<T2> skip1st(Parser<T1> p1, Parser<T2> p2);
#define SKIP1ST(T) CAT(skip1st_, T)
#define DECLARE_SKIP1ST(T)                                               \
  PARSER(T) SKIP1ST(T)(PARSER(Int) p1, PARSER(T) p2)
FOREACH(DECLARE_SKIP1ST, TYPESET(1));
#define skip1st(p1, p2)                                                  \
  (GENERIC_P(PARSER_CAST(p2), SKIP1ST, TYPESET(1))(skip(p1),             \
                                                   (PARSER_CAST(p2))))

// Parser<T2> skip2nd(Parser<T1> p1, Parser<T2> p2);
#define SKIP2ND(T) CAT(skip2nd_, T)
#define DECLARE_SKIP2ND(T)                                               \
  PARSER(T) SKIP2ND(T)(PARSER(T) p1, PARSER(Int) p2)
FOREACH(DECLARE_SKIP2ND, TYPESET(1));
#define skip2nd(p1, p2)                                                  \
  (GENERIC_P(PARSER_CAST(p1), SKIP2ND, TYPESET(1))((PARSER_CAST(p1)),    \
                                                   skip(p2)))

// Parser<T> either(Parser<T> p1, Parser<T> p2);
#define EITHER(T) CAT(either_, T)
#define DECLARE_EITHER(T) PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2)
FOREACH(DECLARE_EITHER, TYPESET(1));
#define either(p1, p2)                                                   \
  (GENERIC_P(PARSER_CAST(p1), EITHER, TYPESET(1))(PARSER_CAST(p1),       \
                                                  PARSER_CAST(p2)))

// Parser<T> tryp(Parser<T> p);
#define TRYP(T) CAT(tryp_, T)
#define DECLARE_TRYP(T) PARSER(T) TRYP(T)(PARSER(T) p)
FOREACH(DECLARE_TRYP, TYPESET(1));
#define tryp(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), TRYP, TYPESET(1))(PARSER_CAST(p)))

// Parser<T> token(Parser<T> p);
#define TOKEN(T) CAT(token_, T)
#define DECLARE_TOKEN(T) PARSER(T) TOKEN(T)(PARSER(T) p)
FOREACH(DECLARE_TOKEN, TYPESET(1));
#define token(p)                                                         \
  (GENERIC_P(PARSER_CAST(p), TOKEN, TYPESET(1))(PARSER_CAST(p)))

#ifdef __cplusplus
}
#endif
