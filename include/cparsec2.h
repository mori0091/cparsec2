/* -*- coding:utf-8-unix -*- */
#pragma once

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "cparsec2/list.h"
#include "cparsec2/macros.h"

#ifdef __cplusplus
#define NORETURN [[noreturn]]
#else
#define NORETURN _Noreturn
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define UNUSED(v) ((void)v)

// ---- resource management ----

// initialize cparsec2
void cparsec2_init(void);
// clean up cparsec2 (free all allocated memory)
void cparsec2_end(void);

void* mem_malloc(size_t s);
void* mem_realloc(void* p, size_t s);
void mem_free(void* p);

/** Construct a formated string */
const char* mem_printf(const char* fmt, ...);

// ---- error and exception handling ----

// context object for exception handling
typedef struct {
  const char* msg;
  jmp_buf e;
} Ctx;

// TRY(Ctx *ctx) {statement...} else {exception-handler...}
#define TRY(ctx) if (!setjmp((ctx)->e))

// throw exception
NORETURN void cthrow(Ctx* ctx, const char* msg);

/** Construct an error message */
#define error(...) mem_printf(__VA_ARGS__)

// ---- source of input character sequence ----

typedef struct stSource* Source;

typedef struct {
  off_t offset;
} SourcePos;

// Construct new Source object from a string or from a FILE pointer
// clang-format off
#define Source_new(x)                                                    \
  _Generic((x)                                                           \
           , char*       : newStringSource                               \
           , const char* : newStringSource                               \
           , FILE*       : newFileSource                                 \
           )(x)
// clang-format on

// Construct new Source object from a string.
Source newStringSource(const char* text);
// Construct new Source object from a FILE pointer.
Source newFileSource(FILE* fp);
// peek head char
char peek(Source src, Ctx* ctx);
// drop head char
void consume(Source src);
// get current source position (backup state of the source)
SourcePos getSourcePos(Source src);
// set current souce position (revert state of the source)
void setSourcePos(Source src, SourcePos pos);
// tests if two SourcePos are same or not
bool isSourcePosEqual(SourcePos p1, SourcePos p2);

// ---- building-block for making parser ----

#define RETURN_TYPE(T) CAT(T, _return_type)

#define PARSER(T) CAT(T, Parser)
#define PARSER_ST(T) CAT(PARSER(T), St)
#define PARSER_FN(T) CAT(PARSER(T), Fn)
#define PARSER_GEN(T) CAT(PARSER(T), _gen)
#define PARSE(T) CAT(PARSER(T), _parse)
#define PARSETEST(T) CAT(PARSER(T), _parseTest)
#define PARSER_ID_FN(T) CAT(PARSER(T), _identity)
#define SHOW(T) CAT(T, _show)

#define TYPEDEF_PARSER(T, R)                                             \
  typedef struct PARSER_ST(T) * PARSER(T);                               \
  typedef R RETURN_TYPE(PARSER(T));                                      \
  typedef R (*PARSER_FN(T))(void* arg, Source src, Ctx* ex)

#define DECLARE_PARSER(T)                                                \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg);                    \
  RETURN_TYPE(PARSER(T)) PARSE(T)(PARSER(T) p, Source src, Ctx * ctx);   \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input);    \
  PARSER(T) PARSER_ID_FN(T)(PARSER(T) p);                                \
  void SHOW(T)(RETURN_TYPE(PARSER(T)) val)

#define DEFINE_PARSER(T, RET)                                            \
  struct PARSER_ST(T) {                                                  \
    PARSER_FN(T) run;                                                    \
    void* arg;                                                           \
  };                                                                     \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg) {                   \
    PARSER(T) p = mem_malloc(sizeof(struct PARSER_ST(T)));               \
    p->run = f;                                                          \
    p->arg = arg;                                                        \
    return p;                                                            \
  }                                                                      \
  RETURN_TYPE(PARSER(T)) PARSE(T)(PARSER(T) p, Source src, Ctx * ctx) {  \
    assert(ctx);                                                         \
    return p->run(p->arg, src, ctx);                                     \
  }                                                                      \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input) {   \
    printf("%s", msg);                                                   \
    Source src = Source_new(input);                                      \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      SHOW(T)(PARSE(T)(p, src, &ctx));                                   \
      return true;                                                       \
    }                                                                    \
    else {                                                               \
      printf("error:%s\n", ctx.msg);                                     \
      mem_free((void*)ctx.msg);                                          \
      return false;                                                      \
    }                                                                    \
  }                                                                      \
  PARSER(T) PARSER_ID_FN(T)(PARSER(T) p) {                               \
    return p;                                                            \
  }                                                                      \
  void SHOW(T)(RETURN_TYPE(PARSER(T)) RET)

// ---- CharParser ----
TYPEDEF_PARSER(Char, char);
// ---- StringParser ----
TYPEDEF_PARSER(String, const char*);
// ---- IntParser ----
TYPEDEF_PARSER(Int, int);
// ---- StringListParser ----
TYPEDEF_PARSER(List(String), List(String));
// ---- IntListParser ----
TYPEDEF_PARSER(List(Int), List(Int));

FOREACH(DECLARE_PARSER, TYPESET(1));

// ---- parser invocation ----

// T parse(Parser<T> p, Souce src, Ctx *ctx)
#define parse(p, src, ctx) GENERIC_P(p, PARSE, TYPESET(1))(p, src, ctx)

// ---- parser invocation (for debug purpose) ----

// bool parseTest(Parser<T> p, const char *input);
#define parseTest(p, input) PARSE_TEST_I("", p, input)

#define PARSE_TEST(p, input)                                             \
  PARSE_TEST_I(mem_printf("%s \"%s\" => ", #p, input), p, input)

// #define PARSE_TEST_I(msg, p, input) (p->test(msg, p, input))
#define PARSE_TEST_I(msg, p, input)                                      \
  GENERIC_P(p, PARSETEST, TYPESET(1))(msg, p, input)

// ---- predicates ----

typedef bool (*Predicate)(char c);
bool is_anyChar(char c);
bool is_digit(char c);
bool is_hexDigit(char c);
bool is_octDigit(char c);
bool is_lower(char c);
bool is_upper(char c);
bool is_alpha(char c);
bool is_alnum(char c);
bool is_letter(char c);
bool is_space(char c);

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
extern PARSER(String) spaces;
extern PARSER(Char) newline;
extern PARSER(Char) crlf;
extern PARSER(Char) endOfLine;
extern PARSER(Char) tab;
extern PARSER(Int) number;

PARSER(Char) range(uint8_t min, uint8_t max);
PARSER(Char) oneOf(const char* cs);
PARSER(Char) noneOf(const char* cs);
PARSER(Char) char1(char c);
PARSER(Char) satisfy(Predicate pred);
PARSER(String) string1(const char* s);

// ---- parser combinators ----

// Parser<T> expects(const char* desc, Parser<T> p);
PARSER(Char) expects(const char* desc, PARSER(Char) p); // TODO test

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
