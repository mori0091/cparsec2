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
  char *buf = mem_malloc(len + 1);
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

/* list of live objects */
static PtrBuffer cparsec2_objects = {0};

static void cparsec2_ensure(void) {
  PtrBuffer *b = &cparsec2_objects;
  if (b->len == b->capacity) {
    int capa = b->capacity * 2;
    void *data = realloc(b->data, sizeof(void *) * capa);
    if (!data) {
      fprintf(stderr, "fatal:realloc:%s\n", strerror(errno));
      exit(1);
    }
    b->capacity = capa;
    b->data = data;
  }
}

static void *cparsec2_push(void *p) {
  cparsec2_ensure();
  if (p) {
    PtrBuffer *b = &cparsec2_objects;
    b->data[b->len++] = p;
  }
  return p;
}

void cparsec2_init(void) {
  PtrBuffer *b = &cparsec2_objects;
  b->data = malloc(sizeof(void *) * 8);
  b->capacity = 8;
  b->len = 0;
  if (!b->data) {
    fprintf(stderr, "fatal:malloc:%s\n", strerror(errno));
    exit(1);
  }
  anyChar = satisfy(is_anyChar);
  digit = satisfy(is_digit);
  lower = satisfy(is_lower);
  upper = satisfy(is_upper);
  alpha = satisfy(is_alpha);
  alnum = satisfy(is_alnum);
  letter = satisfy(is_letter);
}

void cparsec2_end(void) {
  PtrBuffer *b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    free(b->data[i]);
  }
  free(b->data);
  cparsec2_objects = (PtrBuffer){0};
}

void *mem_malloc(size_t s) {
  void *p = cparsec2_push(malloc(s));
  if (s && !p) {
    fprintf(stderr, "fatal:malloc:%s\n", strerror(errno));
    exit(1);
  }
  return p;
}

void *mem_realloc(void *p, size_t s) {
  if (!p) {
    return mem_malloc(s);
  }
  void *q = realloc(p, s);
  if (q) {
    PtrBuffer *b = &cparsec2_objects;
    for (int i = 0; i < b->len; ++i) {
      if (b->data[i] == p) {
        b->data[i] = q;
        return q;
      }
    }
    cparsec2_push(q);
  }
  return q;
}

void mem_free(void *p) {
  PtrBuffer *b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    if (b->data[i] == p) {
      b->data[i] = NULL;
      break;
    }
  }
  free(p);
}

CharParser genCharParser(CharParserFn f, void *arg) {
  CharParser p = mem_malloc(sizeof(struct stCharParser));
  p->run = f;
  p->arg = arg;
  return p;
}

StringParser genStringParser(StringParserFn f, void *arg) {
  StringParser p = mem_malloc(sizeof(struct stStringParser));
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

noreturn void raise(Ctx *ctx, const char *msg) {
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
