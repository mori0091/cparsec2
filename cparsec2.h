/* -*- coding:utf-8-unix -*- */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

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
noreturn void raise(Ctx* ctx, const char* msg);

/** Construct an error message */
const char* error(const char* fmt, ...);

// ---- source of input character sequence ----

typedef struct stSource* Source;
struct stSource {
  const char* input; /* whole input */
  const char* p;     /* pointer to next char */
};

// peek head char
char peek(Source src, Ctx* ctx);
// drop head char
void consume(Source src);

// ---- building-block for making parser ----

typedef char (*CharParserFn)(void* arg, Source src, Ctx* ex);
typedef const char* (*StringParserFn)(void* arg, Source src, Ctx* ex);

typedef struct stCharParser {
  CharParserFn run;
  void* arg;
} * CharParser;

typedef struct stStringParser {
  StringParserFn run;
  void* arg;
} * StringParser;

// clang-format off
#define genParser(f, arg)                                                \
  _Generic((f)                                                           \
           , CharParserFn   : genCharParser                              \
           , StringParserFn : genStringParser                            \
           )(f, arg)
// clang-format on

CharParser genCharParser(CharParserFn f, void* arg);
StringParser genStringParser(StringParserFn f, void* arg);

// ---- parser invocation ----

// T parse(Parser<T> p, Souce src, Ctx *ctx)
// clang-format off
#define parse(p, src, ctx)                                               \
  _Generic((p)                                                           \
           , CharParser   : parse_char                                   \
           , StringParser : parse_string                                 \
           )(p, src, ctx)
// clang-format on

char parse_char(CharParser p, Source src, Ctx* ctx);
const char* parse_string(StringParser p, Source src, Ctx* ctx);

// void parseTest(Parser<T> p, const char *input);
// clang-format off
#define parseTest(p, input)                                              \
  _Generic((p)                                                           \
           , CharParser   : parseTest_char                               \
           , StringParser : parseTest_string                             \
           )(p, input)
// clang-format on

void parseTest_char(CharParser p, const char* input);
void parseTest_string(StringParser p, const char* input);

#define PARSE_TEST(p, input)                                             \
  do {                                                                   \
    printf("%8s \"%s\" => ", #p, input);                                 \
    parseTest(p, input);                                                 \
  } while (0)

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

// ---- built-in parsers, parser generators, and parser combinators ----

extern CharParser anyChar;
extern CharParser digit;
extern CharParser lower;
extern CharParser upper;
extern CharParser alpha;
extern CharParser alnum;
extern CharParser letter;
extern StringParser spaces;

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
