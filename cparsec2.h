/* -*- coding:utf-8-unix -*- */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <setjmp.h>

#define UNUSED(v) ((void)v)

typedef struct {
  char *data;
  int capacity;
  int len;
} Buffer;

Buffer buf_new(void);
void buf_ensure(Buffer *b);
void buf_push(Buffer *b, char v);
void buf_append(Buffer *b, const char* s);
char* buf_finish(Buffer *b);

typedef struct {
  void **data;
  int capacity;
  int len;
} PtrBuffer;

PtrBuffer ptrbuf_new(void);
void ptrbuf_ensure(PtrBuffer *b);
void ptrbuf_push(PtrBuffer *b, void *v);
void ptrbuf_append(PtrBuffer *b, void **s);
void **ptrbuf_finish(PtrBuffer *b);

/** Construct an error message */
const char *error(const char *fmt, ...);

typedef struct stSource *Source;
struct stSource {
  const char *input; /* whole input */
  const char *p;     /* pointer to next char */
};

typedef bool (*Predicate)(char c);
bool is_anyChar(char c);
bool is_digit(char c);
bool is_lower(char c);
bool is_upper(char c);
bool is_alpha(char c);
bool is_alnum(char c);
bool is_letter(char c);

typedef struct {
  const char *error; // An error message if an error occurred, otherwise NULL.
  char result;       // the result value if and only if operation succeeded.
} CharResult;

typedef struct {
  const char *error;  // An error message if an error occurred, otherwise NULL.
  const char *result; // the result value if and only if operation succeeded.
} StringResult;

typedef CharResult (*CharParserFn)(void *arg, Source src);
typedef StringResult (*StringParserFn)(void *arg, Source src);

typedef struct stCharParser {
  CharParserFn run;
  void *arg;
} *CharParser;

typedef struct stStringParser {
  StringParserFn run;
  void *arg;
} *StringParser;

#define genParser(f, arg)                       \
  _Generic((f)                                  \
           , CharParserFn   : genCharParser     \
           , StringParserFn : genStringParser   \
           )(f, arg)

CharParser genCharParser(CharParserFn f, void *arg);
StringParser genStringParser(StringParserFn f, void *arg);

// context object for exception handling
typedef struct {
  const char* msg;
  jmp_buf e;
} Ctx;

// TRY(Ctx *ctx) {statement...} else {exception-handler...}
#define TRY(ctx) if (!setjmp((ctx)->e))

// throw exception
noreturn void raise(Ctx* ctx, const char *msg);

// peek head char
char peek(Source src, Ctx *ctx);
// drop head char
void consume(Source src);

// initialize cparsec2
void cparsec2_init(void);
// clean up cparsec2 (free all allocated memory)
void cparsec2_end(void);

void *mem_malloc(size_t s);
void *mem_realloc(void *p, size_t s);
void mem_free(void *p);

#define PARSE_TEST(p, input)                                                   \
  printf("%8s \"%s\" => ", #p, input);                                         \
  parseTest(p, input)

// T parse(Parser<T> p, Source src);
#define parse(p, src) ((p)->run((p)->arg, (src)))

// void parseTest(Parser<T> p, const char *input);
#define parseTest(p, input)                                             \
  _Generic((p)                                                          \
           , CharParser   : parseTest_char                              \
           , StringParser : parseTest_string                            \
           )(p, input)

void parseTest_char(CharParser p, const char *input);
void parseTest_string(StringParser p, const char *input);

// T parseEx(Parser<T> p, Souce src, Ctx *ctx)
#define parseEx(p, src, ctx)                    \
  _Generic((p)                                  \
           , CharParser   : parseEx_char        \
           , StringParser : parseEx_string      \
           )(p, src, ctx)

char parseEx_char(CharParser p, Source src, Ctx *ctx);
const char *parseEx_string(StringParser p, Source src, Ctx *ctx);

extern CharParser anyChar;
extern CharParser digit;
extern CharParser lower;
extern CharParser upper;
extern CharParser alpha;
extern CharParser alnum;
extern CharParser letter;

CharParser char1(char c);
CharParser satisfy(Predicate pred);

StringParser many(CharParser p);
StringParser many1(CharParser p);

// Parser<T[]> seq(Parser<T> p, ...);
#define seq(...) SEQ_I(__VA_ARGS__, NULL)
#define SEQ_I(p, ...) _Generic((p), CharParser : seq_char)((CharParser []){p, __VA_ARGS__})
StringParser seq_char(CharParser ps[]);

// Parser<T[]> cons(Parser<T> p, Parser<T[]> ps);
#define cons(p, ps) _Generic((p), CharParser : cons_char)(p, ps)
StringParser cons_char(CharParser p, StringParser ps);
