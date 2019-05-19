/* -*- coding:utf-8-unix -*- */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
  void **data;
  int capacity;
  int len;
} PtrBuffer;

PtrBuffer ptrbuf_new(void);
void ptrbuf_ensure(PtrBuffer *b);
void ptrbuf_push(PtrBuffer *b, void *v);

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

typedef struct stCharParser *CharParser;
struct stCharParser {
  CharResult (*run)(CharParser self, Source src);
  union {
    char expected;
    Predicate pred;
  };
};

typedef struct {
  const char *error;  // An error message if an error occurred, otherwise NULL.
  const char *result; // the result value if and only if operation succeeded.
} StringResult;

typedef struct stStringParser *StringParser;
struct stStringParser {
  StringResult (*run)(StringParser self, Source src);
  union {
    CharParser parser;
    CharParser *parsers; // a list of CharParser (NULL terminated)
  };
};

// peek head char
CharResult peek(Source src);
// drop head char
void consume(Source src);

// initialize cparsec2
void cparsec2_init(void);

#define PARSE_TEST(p, input)                                                   \
  printf("%8s \"%s\" => ", #p, input);                                         \
  parseTest(p, input)

// T parse(Parser<T> p, Source src);
#define parse(p, src) ((p)->run((p), (src)))

// void parseTest(Parser<T> p, const char *input);
#define parseTest(p, input)                                             \
  _Generic((p)                                                          \
           , CharParser   : parseTest_char                              \
           , StringParser : parseTest_string                            \
           )(p, input)

void parseTest_char(CharParser p, const char *input);
void parseTest_string(StringParser p, const char *input);

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
