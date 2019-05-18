/* -*- coding:utf-8-unix -*- */

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(v) ((void)v)

typedef struct {
  const char *error; // An error message if an error occurred, otherwise NULL.
  char result;       // the result value if and only if operation succeeded.
} CharResult;

/** Construct an error message */
const char *error(const char *fmt, ...);

typedef struct stSource *Source;
struct stSource {
  const char *input; /* whole input */
  const char *p;     /* pointer to next char */
};

typedef bool (*Predicate)(char c);
bool is_digit(char c);
bool is_lower(char c);
bool is_upper(char c);
bool is_alpha(char c);
bool is_alnum(char c);
bool is_letter(char c);

typedef struct stCharParser *CharParser;
struct stCharParser {
  CharResult (*run)(CharParser self, Source src);
  union {
    char expected;
    Predicate pred;
  };
};

#define PARSE_TEST(p, input)                                                   \
  printf("%8s \"%s\" => ", #p, input);                                         \
  parseTest(p, input)

// T parse(Parser<T> p, Source src);
#define parse(p, src) ((p)->run((p), (src)))

// void parseTest(Parser<T> p, const char *input);
#define parseTest(P, INPUT)                                                    \
  do {                                                                         \
    struct stSource src = {.input = (INPUT), .p = (INPUT)};                    \
    showResult(parse((P), &src));                                              \
  } while (0)

// void showResult(Result<T> x);
#define showResult(x)                                                          \
  do {                                                                         \
    if ((x).error) {                                                           \
      printf("error:%s\n", (x).error);                                         \
    } else {                                                                   \
      show((x).result);                                                        \
    }                                                                          \
  } while (0)

// void show(T x);
#define show(x) _Generic((x), char : show_char)(x)

void show_char(char c);

extern CharParser const anyChar;
CharParser char1(char c);
CharParser satisfy(Predicate pred);
