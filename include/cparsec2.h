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

// clang-format off
#define PARSE_TEST_I(msg, p, input)             \
  _Generic((p)                                  \
           , PARSER(Char)   : PARSETEST(Char)   \
           , PARSER(String) : PARSETEST(String) \
           , PARSER(Token)  : PARSETEST(Token)  \
           , PARSER(Int)    : PARSETEST(Int)    \
           )(msg, p, input)
// clang-format on

// ---- building-block for making parser ----

#define PARSER(T) T##Parser
#define PARSER_FN(T) T##ParserFn
#define PARSER_ST(T) st##T##Parser
#define PARSER_GEN(T) gen##T##Parser
#define PARSE(T) parse_##T
#define PARSETEST(T) parseTest_##T
#define SHOW(T) show_##T

#define DECLARE_PARSER(T, R)                                          \
  typedef R (*PARSER_FN(T))(void* arg, Source src, Ctx* ex);          \
  typedef struct PARSER_ST(T) * PARSER(T);                            \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg);                 \
  R PARSE(T)(PARSER(T) p, Source src, Ctx * ctx);                     \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input); \
  void SHOW(T)(R val)

#define DEFINE_PARSER(T, R)                                             \
  struct PARSER_ST(T) {                                                 \
    PARSER_FN(T) run;                                                   \
    void* arg;                                                          \
  };                                                                    \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg) {                  \
    PARSER(T) p = mem_malloc(sizeof(struct PARSER_ST(T)));              \
    p->run = f;                                                         \
    p->arg = arg;                                                       \
    return p;                                                           \
  }                                                                     \
  R PARSE(T)(PARSER(T) p, Source src, Ctx* ctx) {                       \
    assert(ctx);                                                        \
    return p->run(p->arg, src, ctx);                                    \
  }                                                                     \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input) {  \
    printf("%s", msg);                                                  \
    Source src = Source_new(input);                                     \
    Ctx ctx;                                                            \
    TRY(&ctx) {                                                         \
      SHOW(T)(PARSE(T)(p, src, &ctx));                                  \
      return true;                                                      \
    }                                                                   \
    else {                                                              \
      printf("error:%s\n", ctx.msg);                                    \
      mem_free((void*)ctx.msg);                                         \
      return false;                                                     \
    }                                                                   \
  }                                                                     \
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

extern CharParser anyChar;
extern CharParser digit;
extern CharParser lower;
extern CharParser upper;
extern CharParser alpha;
extern CharParser alnum;
extern CharParser letter;
extern StringParser spaces;

// ---- parser generators, and parser combinators ----

CharParser char1(char c);
CharParser satisfy(Predicate pred);

StringParser many(CharParser p);
StringParser many1(CharParser p);

// Parser<T[]> seq(Parser<T> p, ...);
#define seq(...) SEQ_I(__VA_ARGS__, NULL)
#define SEQ_I(p, ...)                                                    \
  _Generic((p), CharParser : seq_char)((CharParser[]){p, __VA_ARGS__})
StringParser seq_char(CharParser ps[]);

// Parser<T[]> cons(Parser<T> p, Parser<T[]> ps);
#define cons(p, ps) _Generic((p), CharParser : cons_char)(p, ps)
StringParser cons_char(CharParser p, StringParser ps);

// Parser<String> string1(const char* s);
StringParser string1(const char* s);

// Parser<Token> token(enum TokenType type, T p);
// clang-format off
#define token(type, p)                          \
  _Generic((p)                                  \
           , char         : token_c             \
           , const char*  : token_s             \
           , CharParser   : token_Char          \
           , StringParser : token_String        \
           )(type, p)
// clang-format on

TokenParser token_c(int type, char c);
TokenParser token_s(int type, const char* s);
TokenParser token_Char(int type, CharParser p);
TokenParser token_String(int type, StringParser p);

#ifdef __cplusplus
}
#endif
