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

struct stSource {
  const char* input; /* whole input */
  const char* p;     /* pointer to next char */
};

// Construct new Source object
Source Source_new(const char* input);
// peek head char
char peek(Source src, Ctx* ctx);
// drop head char
void consume(Source src);

// ---- parser invocation ----

// T parse(Parser<T> p, Souce src, Ctx *ctx)
#define parse(p, src, ctx) ((p)->run((p)->arg, src, ctx))

// ---- parser invocation (for debug purpose) ----

// bool parseTest(Parser<T> p, const char *input);
#define parseTest(p, input) PARSE_TEST_I("", p, input)

#define PARSE_TEST(p, input)                                             \
  PARSE_TEST_I(mem_printf("%s \"%s\" => ", #p, input), p, input)

#define PARSE_TEST_I(msg, p, input) (p->test(msg, p, input))

// ---- building-block for making parser ----

#define RETURN_TYPE(T) CAT(T, _return_type)

#define PARSER(T) CAT(T, Parser)
#define PARSER_ST(T) CAT(PARSER(T), St)
#define PARSER_FN(T) CAT(PARSER(T), Fn)
#define PARSER_TESTFN(T) CAT(PARSER(T), TestFn)
#define PARSER_GEN(T) CAT(PARSER(T), _gen)
#define PARSE(T) CAT(PARSER(T), _parse)
#define PARSETEST(T) CAT(PARSER(T), _parseTest)
#define PARSER_ID_FN(T) CAT(PARSER(T), _identity)
#define SHOW(T) CAT(T, _show)

#define TYPEDEF_PARSER(T, R)                                             \
  typedef struct PARSER_ST(T) * PARSER(T);                               \
  typedef R RETURN_TYPE(PARSER(T));                                      \
  typedef R (*PARSER_FN(T))(void* arg, Source src, Ctx* ex);             \
  typedef bool (*PARSER_TESTFN(T))(const char* msg, PARSER(T) p,         \
                                   const char* input);                   \
  struct PARSER_ST(T) {                                                  \
    PARSER_FN(T) run;                                                    \
    PARSER_TESTFN(T) test;                                               \
    void* arg;                                                           \
  }

#define DECLARE_PARSER(T)                                                \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg);                    \
  RETURN_TYPE(PARSER(T)) PARSE(T)(PARSER(T) p, Source src, Ctx * ctx);   \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input);    \
  PARSER(T) PARSER_ID_FN(T)(PARSER(T) p);                                \
  void SHOW(T)(RETURN_TYPE(PARSER(T)) val)

#define DEFINE_PARSER(T, RET)                                            \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg) {                   \
    PARSER(T) p = mem_malloc(sizeof(struct PARSER_ST(T)));               \
    p->run = f;                                                          \
    p->test = PARSETEST(T);                                              \
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

// clang-format off
#define PARSER_CAST(x)                                          \
  _Generic((x)                                                  \
           , char                 : char1                       \
           , char*                : string1                     \
           , const char*          : string1                     \
           , PARSER(Char)         : PARSER_ID_FN(Char)          \
           , PARSER(String)       : PARSER_ID_FN(String)        \
           , PARSER(Int)          : PARSER_ID_FN(Int)           \
           , PARSER(List(String)) : PARSER_ID_FN(List(String))  \
           , PARSER(List(Int))    : PARSER_ID_FN(List(Int))     \
           )(x)
// clang-format on

// ---- CharParser ----
TYPEDEF_PARSER(Char, char);
DECLARE_PARSER(Char);

// ---- StringParser ----
TYPEDEF_PARSER(String, const char*);
DECLARE_PARSER(String);

// ---- IntParser ----
TYPEDEF_PARSER(Int, int);
DECLARE_PARSER(Int);

// ---- StringListParser ----
TYPEDEF_PARSER(List(String), List(String));
DECLARE_PARSER(List(String));

// ---- IntListParser ----
TYPEDEF_PARSER(List(Int), List(Int));
DECLARE_PARSER(List(Int));

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

PARSER(Char) oneOf(const char* cs);
PARSER(Char) noneOf(const char* cs);
PARSER(Char) char1(char c);
PARSER(Char) satisfy(Predicate pred);

// Parser<String> string1(const char* s);
PARSER(String) string1(const char* s);

// ---- parser combinators ----

// clang-format off
#define GENERIC_P0(expr, F)                         \
  _Generic((expr)                                   \
           , PARSER(Char)         : F(Char)         \
           , PARSER(String)       : F(String)       \
           , PARSER(Int)          : F(Int)          \
           )
#define GENERIC_P1(expr, F)                         \
  _Generic((expr)                                   \
           , PARSER(Char)         : F(Char)         \
           , PARSER(String)       : F(String)       \
           , PARSER(Int)          : F(Int)          \
           , PARSER(List(String)) : F(List(String)) \
           , PARSER(List(Int))    : F(List(Int))    \
           )
// clang-format on

#define DECLARE_P0(COMBINATOR)                                           \
  CAT(DECLARE_, COMBINATOR)(Char);                                       \
  CAT(DECLARE_, COMBINATOR)(String);                                     \
  CAT(DECLARE_, COMBINATOR)(Int)
#define DECLARE_P1(COMBINATOR)                                           \
  DECLARE_P0(COMBINATOR);                                                \
  CAT(DECLARE_, COMBINATOR)(List(String));                               \
  CAT(DECLARE_, COMBINATOR)(List(Int))

#define DEFINE_P0(COMBINATOR)                                            \
  CAT(DEFINE_, COMBINATOR)(Char);                                        \
  CAT(DEFINE_, COMBINATOR)(String);                                      \
  CAT(DEFINE_, COMBINATOR)(Int)
#define DEFINE_P1(COMBINATOR)                                            \
  DEFINE_P0(COMBINATOR);                                                 \
  CAT(DEFINE_, COMBINATOR)(List(String));                                \
  CAT(DEFINE_, COMBINATOR)(List(Int))

// Parser<T> expects(const char* desc, Parser<T> p);
PARSER(Char) expects(const char* desc, PARSER(Char) p); // TODO test

// Parser<Int> skip(Parser<T> p);
#define SKIP(T) CAT(skip_, T)
#define skip(p) (GENERIC_P1(PARSER_CAST(p), SKIP)(PARSER_CAST(p)))
#define DECLARE_SKIP(T) PARSER(Int) SKIP(T)(PARSER(T) p)

DECLARE_P1(SKIP);

// Parser<T2> skip1st(Parser<T1> p1, Parser<T2> p2);
#define SKIP1ST(T) CAT(skip1st_, T)
#define skip1st(p1, p2)                                                  \
  (GENERIC_P1(PARSER_CAST(p2), SKIP1ST)(skip(p1), (PARSER_CAST(p2))))
#define DECLARE_SKIP1ST(T)                                               \
  PARSER(T) SKIP1ST(T)(PARSER(Int) p1, PARSER(T) p2)

DECLARE_P1(SKIP1ST);

#define MANY(T) CAT(many_, T)
#define many(p) (GENERIC_P0(PARSER_CAST(p), MANY)(PARSER_CAST(p)))
#define DECLARE_MANY(T) PARSER(List(T)) MANY(T)(PARSER(T) p)

DECLARE_P0(MANY);

#define MANY1(T) CAT(many1_, T)
#define many1(p) (GENERIC_P0(PARSER_CAST(p), MANY1)(PARSER_CAST(p)))
#define DECLARE_MANY1(T) PARSER(List(T)) MANY1(T)(PARSER(T) p)

DECLARE_P0(MANY1);

// Parser<T[]> seq(Parser<T> p, ...);
#define SEQ(T) CAT(seq_, T)
#define seq(...) SEQ_0(__VA_ARGS__, NULL)
#define SEQ_0(p, ...) (GENERIC_P0(p, SEQ)((void*[]){p, __VA_ARGS__}))
#define DECLARE_SEQ(T) PARSER(List(T)) SEQ(T)(void* ps[])

DECLARE_P0(SEQ);

// Parser<T[]> cons(Parser<T> p, Parser<T[]> ps);
#define CONS(T) CAT(cons_, T)
#define cons(p, ps)                                                      \
  (GENERIC_P0(PARSER_CAST(p), CONS)(PARSER_CAST(p), PARSER_CAST(ps)))
#define DECLARE_CONS(T)                                                  \
  PARSER(List(T)) CONS(T)(PARSER(T) p, PARSER(List(T)) ps)

DECLARE_P0(CONS);

// Parser<T> either(Parser<T> p1, Parser<T> p2);
#define EITHER(T) CAT(either_, T)
#define either(p1, p2)                                                   \
  (GENERIC_P1(PARSER_CAST(p1), EITHER)(PARSER_CAST(p1), PARSER_CAST(p2)))
#define DECLARE_EITHER(T) PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2)

DECLARE_P1(EITHER);

// Parser<T> tryp(Parser<T> p);
#define TRYP(T) CAT(tryp_, T)
#define tryp(p) (GENERIC_P1(PARSER_CAST(p), TRYP)(PARSER_CAST(p)))
#define DECLARE_TRYP(T) PARSER(T) TRYP(T)(PARSER(T) p)

DECLARE_P1(TRYP);

// Parser<T> token(Parser<T> p);
#define TOKEN(T) CAT(token_, T)
#define token(p) (GENERIC_P1(PARSER_CAST(p), TOKEN)(PARSER_CAST(p)))
#define DECLARE_TOKEN(T) PARSER(T) TOKEN(T)(PARSER(T) p)

DECLARE_P1(TOKEN);

#ifdef __cplusplus
}
#endif
