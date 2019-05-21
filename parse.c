/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

const char *error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(NULL, 0, fmt, ap);
  if (len < 0) {
    fprintf(stderr, "vsnprintf(NULL, 0, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  char *buf = malloc(len + 1);
  if (vsnprintf(buf, len + 1, fmt, ap) < 0) {
    fprintf(stderr, "vsnprintf(buf, len, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  return buf;
}

char peek(Source src, Ctx *ctx) {
  char c = *src->p;
  if (!c) {
    raise(ctx, error("too short"));
  }
  return c;
}

void consume(Source src) {
  assert(*src->p);
  src->p++;
}

CharParser anyChar;
CharParser digit;
CharParser lower;
CharParser upper;
CharParser alpha;
CharParser alnum;
CharParser letter;

void cparsec2_init(void) {
  anyChar = satisfy(is_anyChar);
  digit = satisfy(is_digit);
  lower = satisfy(is_lower);
  upper = satisfy(is_upper);
  alpha = satisfy(is_alpha);
  alnum = satisfy(is_alnum);
  letter = satisfy(is_letter);
}

CharParser genCharParser(CharParserFn f, void *arg) {
  CharParser p = malloc(sizeof(struct stCharParser));
  p->run = f;
  p->arg = arg;
  return p;
}

StringParser genStringParser(StringParserFn f, void *arg) {
  StringParser p = malloc(sizeof(struct stStringParser));
  p->run = f;
  p->arg = arg;
  return p;
}

void parseTest_char(CharParser p, const char *input) {
  struct stSource src = {.input = input, .p = input};
  CharResult x = parse(p, &src);
  if (x.error) {
    printf("error:%s\n", x.error);
  } else {
    printf("'%c'\n", x.result);
  }
}

void parseTest_string(StringParser p, const char *input) {
  struct stSource src = {.input = input, .p = input};
  StringResult x = parse(p, &src);
  if (x.error) {
    printf("error:%s\n", x.error);
  } else {
    printf("\"%s\"\n", x.result);
  }
}

noreturn void raise(Ctx* ctx, const char *msg) {
  ctx->msg = msg;
  longjmp(ctx->e, -1);
}

char parseEx_char(CharParser p, Source src, Ctx *ctx) {
  assert(ctx);
  CharResult x = parse(p, src);
  if (x.error) {
    raise(ctx, x.error);
  }
  return x.result;
}

const char *parseEx_string(StringParser p, Source src, Ctx *ctx) {
  assert(ctx);
  StringResult x = parse(p, src);
  if (x.error) {
    raise(ctx, x.error);
  }
  return x.result;
}
