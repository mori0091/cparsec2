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

// ---- variadic buffer ----

typedef struct {
  char* data;
  int capacity;
  int len;
} Buffer;

Buffer buf_new(void);
void buf_ensure(Buffer* b);
void buf_push(Buffer* b, char v);
void buf_append(Buffer* b, const char* s);
char* buf_finish(Buffer* b);

typedef struct {
  void** data;
  int capacity;
  int len;
} PtrBuffer;

PtrBuffer ptrbuf_new(void);
void ptrbuf_ensure(PtrBuffer* b);
void ptrbuf_push(PtrBuffer* b, void* v);
void ptrbuf_append(PtrBuffer* b, void** s);
void** ptrbuf_finish(PtrBuffer* b);

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

#define PARSER(T) T##Parser
#define PARSER_FN(T) T##ParserFn
#define PARSER_ST(T) st##T##Parser
#define PARSER_GEN(T) gen##T##Parser
#define PARSE(T) parse_##T
#define PARSETEST(T) parseTest_##T
#define SHOW(T) show_##T

#define DECLARE_PARSER(T, R)                                             \
  typedef R (*PARSER_FN(T))(void* arg, Source src, Ctx* ex);             \
  typedef struct PARSER_ST(T) * PARSER(T);                               \
  struct PARSER_ST(T) {                                                  \
    PARSER_FN(T) run;                                                    \
    bool (*test)(const char* msg, PARSER(T) p, const char* input);       \
    void* arg;                                                           \
  };                                                                     \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg);                    \
  R PARSE(T)(PARSER(T) p, Source src, Ctx * ctx);                        \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input);    \
  void SHOW(T)(R val)

#define DEFINE_PARSER(T, R)                                              \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg) {                   \
    PARSER(T) p = mem_malloc(sizeof(struct PARSER_ST(T)));               \
    p->run = f;                                                          \
    p->test = PARSETEST(T);                                              \
    p->arg = arg;                                                        \
    return p;                                                            \
  }                                                                      \
  R PARSE(T)(PARSER(T) p, Source src, Ctx * ctx) {                       \
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
  void SHOW(T)(R x)

// ---- CharParser ----
DECLARE_PARSER(Char, char);

// ---- StringParser ----
DECLARE_PARSER(String, const char*);

// ---- Token ----
typedef struct stToken* Token;

enum TokenType {
  TK_NUMBER,
};

struct stToken {
  enum TokenType type; // kind of the token
  const char* str;     // string recognized as the token
};

// ---- TokenParser ----
DECLARE_PARSER(Token, Token);

// ---- IntParser ----
DECLARE_PARSER(Int, int);

// ---- predicates ----

typedef bool (*Predicate)(char c);
bool is_anyChar(char c);
bool is_digit(char c);
bool is_lower(char c);
bool is_upper(char c);
bool is_alpha(char c);
bool is_alnum(char c);
bool is_letter(char c);
bool is_space(char c);

// ---- built-in parsers ----

extern PARSER(Char) anyChar;
extern PARSER(Char) digit;
extern PARSER(Char) lower;
extern PARSER(Char) upper;
extern PARSER(Char) alpha;
extern PARSER(Char) alnum;
extern PARSER(Char) letter;
extern PARSER(String) spaces;

// ---- parser generators, and parser combinators ----

PARSER(Char) char1(char c);
PARSER(Char) satisfy(Predicate pred);

PARSER(String) many(PARSER(Char) p);
PARSER(String) many1(PARSER(Char) p);

// Parser<T[]> seq(Parser<T> p, ...);
#define seq(...) SEQ_I(__VA_ARGS__, NULL)
#define SEQ_I(p, ...)                                                    \
  _Generic((p), PARSER(Char) : seq_char)((PARSER(Char)[]){p, __VA_ARGS__})

PARSER(String) seq_char(PARSER(Char) ps[]);

// Parser<T[]> cons(Parser<T> p, Parser<T[]> ps);
#define cons(p, ps) _Generic((p), PARSER(Char) : cons_char)(p, ps)
PARSER(String) cons_char(PARSER(Char) p, PARSER(String) ps);

// Parser<String> string1(const char* s);
PARSER(String) string1(const char* s);

// Parser<T> either(Parser<T> p1, Parser<T> p2);
#define EITHER(T) either_##T
// clang-format off
#define either(p1, p2)                          \
  _Generic((p1)                                 \
           , char           : EITHER(c)         \
           , const char*    : EITHER(s)         \
           , PARSER(Char)   : EITHER(Char)      \
           , PARSER(String) : EITHER(String)    \
           , PARSER(Int)    : EITHER(Int)       \
           , PARSER(Token)  : EITHER(Token)     \
           )(p1, p2)
// clang-format on

PARSER(Char) EITHER(c)(char c1, char c2);
PARSER(String) EITHER(s)(const char* s1, const char* s2);
PARSER(Char) EITHER(Char)(PARSER(Char) p1, PARSER(Char) p2);
PARSER(String) EITHER(String)(PARSER(String) p1, PARSER(String) p2);
PARSER(Int) EITHER(Int)(PARSER(Int) p1, PARSER(Int) p2);
PARSER(Token) EITHER(Token)(PARSER(Token) p1, PARSER(Token) p2);

// Parser<Token> token(enum TokenType type, T p);
#define TOKEN(T) token_##T
// clang-format off
#define token(type, p)                          \
  _Generic((p)                                  \
           , char           : TOKEN(c)          \
           , const char*    : TOKEN(s)          \
           , PARSER(Char)   : TOKEN(Char)       \
           , PARSER(String) : TOKEN(String)     \
           )(type, p)
// clang-format on

PARSER(Token) TOKEN(c)(int type, char c);
PARSER(Token) TOKEN(s)(int type, const char* s);
PARSER(Token) TOKEN(Char)(int type, PARSER(Char) p);
PARSER(Token) TOKEN(String)(int type, PARSER(String) p);

#ifdef __cplusplus
}
#endif
