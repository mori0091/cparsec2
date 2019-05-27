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
const char* error(const char* fmt, ...);

// ---- source of input character sequence ----

typedef struct stSource* Source;

// Construct new Source object
Source Source_new(const char* input);
// peek head char
char peek(Source src, Ctx* ctx);
// drop head char
void consume(Source src);

// ---- building-block for making parser ----

// clang-format off
#define genParser(f, arg)                       \
  _Generic((f)                                  \
           , CharParserFn   : genCharParser     \
           , StringParserFn : genStringParser   \
           , TokenParserFn  : genTokenParser    \
           )(f, arg)
// clang-format on

// ---- parser invocation ----

// T parse(Parser<T> p, Souce src, Ctx *ctx)
// clang-format off
#define parse(p, src, ctx)                      \
  _Generic((p)                                  \
           , CharParser   : parse_Char          \
           , StringParser : parse_String        \
           , TokenParser  : parse_Token         \
           )(p, src, ctx)
// clang-format on

// ---- parser invocation (for debug purpose) ----

// void parseTest(Parser<T> p, const char *input);
// clang-format off
#define parseTest(p, input)                     \
  _Generic((p)                                  \
           , CharParser   : parseTest_Char      \
           , StringParser : parseTest_String    \
           , TokenParser  : parseTest_Token     \
           )(p, input)
// clang-format on

// clang-format off
#define PARSE_TEST(p, input)                    \
  do {                                          \
    printf("%8s \"%s\" => ", #p, input);        \
    parseTest(p, input);                        \
  } while (0)
// clang-format on

// ---- CharParser ----
typedef char (*CharParserFn)(void* arg, Source src, Ctx* ex);
typedef struct stCharParser* CharParser;
CharParser genCharParser(CharParserFn f, void* arg);
char parse_Char(CharParser p, Source src, Ctx* ctx);
void parseTest_Char(CharParser p, const char* input);

struct stCharParser {
  CharParserFn run;
  void* arg;
};

// ---- StringParser ----
typedef const char* (*StringParserFn)(void* arg, Source src, Ctx* ex);
typedef struct stStringParser* StringParser;
StringParser genStringParser(StringParserFn f, void* arg);
const char* parse_String(StringParser p, Source src, Ctx* ctx);
void parseTest_String(StringParser p, const char* input);

struct stStringParser {
  StringParserFn run;
  void* arg;
};

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
typedef Token (*TokenParserFn)(void* arg, Source src, Ctx* ex);
typedef struct stTokenParser* TokenParser;
TokenParser genTokenParser(TokenParserFn f, void* arg);
Token parse_Token(TokenParser p, Source src, Ctx* ctx);
void parseTest_Token(TokenParser p, const char* input);

struct stTokenParser {
  TokenParserFn run;
  void* arg;
};

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
