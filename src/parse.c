/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

// ---- resource management ----

/* list of live objects */
static PtrBuffer cparsec2_objects = {0};

static void cparsec2_ensure(void) {
  PtrBuffer* b = &cparsec2_objects;
  if (b->len == b->capacity) {
    int capa = b->capacity * 2;
    void* data = realloc(b->data, sizeof(void*) * capa);
    if (!data) {
      fprintf(stderr, "fatal:realloc:%s\n", strerror(errno));
      exit(1);
    }
    b->capacity = capa;
    b->data = data;
  }
}

static void* cparsec2_push(void* p) {
  cparsec2_ensure();
  if (p) {
    PtrBuffer* b = &cparsec2_objects;
    b->data[b->len++] = p;
  }
  return p;
}

void cparsec2_init(void) {
  PtrBuffer* b = &cparsec2_objects;
  b->data = malloc(sizeof(void*) * 8);
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
  spaces = many(satisfy(is_space));
}

void cparsec2_end(void) {
  PtrBuffer* b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    free(b->data[i]);
  }
  free(b->data);
  cparsec2_objects = (PtrBuffer){0};
}

void* mem_malloc(size_t s) {
  void* p = cparsec2_push(malloc(s));
  if (s && !p) {
    fprintf(stderr, "fatal:malloc:%s\n", strerror(errno));
    exit(1);
  }
  return p;
}

void* mem_realloc(void* p, size_t s) {
  if (!p) {
    return mem_malloc(s);
  }
  void* q = realloc(p, s);
  if (q) {
    PtrBuffer* b = &cparsec2_objects;
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

void mem_free(void* p) {
  PtrBuffer* b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    if (b->data[i] == p) {
      b->data[i] = NULL;
      break;
    }
  }
  free(p);
}

const char* mem_printf(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(NULL, 0, fmt, ap);
  if (len < 0) {
    fprintf(stderr, "vsnprintf(NULL, 0, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  char* buf = mem_malloc(len + 1);
  va_start(ap, fmt);
  if (vsnprintf(buf, len + 1, fmt, ap) < 0) {
    fprintf(stderr, "vsnprintf(buf, len, fmt, ...):%s\n",
            strerror(errno));
    exit(1);
  }
  return buf;
}

// ---- variadic buffer ----
// -- (see 'collection.c')

// ---- error and exception handling ----

_Noreturn void cthrow(Ctx* ctx, const char* msg) {
  ctx->msg = msg;
  longjmp(ctx->e, -1);
}

// ---- source of input character sequence ----

struct stSource {
  const char* input; /* whole input */
  const char* p;     /* pointer to next char */
};

Source Source_new(const char* input) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->input = input;
  src->p = input;
  return src;
}

char peek(Source src, Ctx* ctx) {
  char c = *src->p;
  if (!c) {
    cthrow(ctx, error("too short"));
  }
  return c;
}

void consume(Source src) {
  assert(*src->p);
  src->p++;
}

// ---- show ----
// clang-format off
#define show(x)                                 \
  _Generic((x)                                  \
           , char        : show_Char            \
           , const char* : show_String          \
           , Token       : show_Token           \
           )(x)
// clang-format on

void show_Char(char x) { printf("'%c'\n", x); }

void show_String(const char* x) { printf("\"%s\"\n", x); }

void show_Token(Token x) {
  printf("<Token:%d,\"%s\">\n", x->type, x->str);
}

// ---- CharParser ----

CharParser genCharParser(CharParserFn f, void* arg) {
  CharParser p = mem_malloc(sizeof(struct stCharParser));
  p->run = f;
  p->arg = arg;
  return p;
}

char parse_Char(CharParser p, Source src, Ctx* ctx) {
  assert(ctx);
  return p->run(p->arg, src, ctx);
}

bool parseTest_Char(const char* msg, CharParser p, const char* input) {
  printf(msg);
  Source src = Source_new(input);
  Ctx ctx;
  TRY(&ctx) {
    show(parse(p, src, &ctx));
    return true;
  }
  else {
    printf("error:%s\n", ctx.msg);
    mem_free((void*)ctx.msg);
    return false;
  }
}

// ---- StringParser ----

StringParser genStringParser(StringParserFn f, void* arg) {
  StringParser p = mem_malloc(sizeof(struct stStringParser));
  p->run = f;
  p->arg = arg;
  return p;
}

const char* parse_String(StringParser p, Source src, Ctx* ctx) {
  assert(ctx);
  return p->run(p->arg, src, ctx);
}

bool parseTest_String(const char* msg, StringParser p,
                      const char* input) {
  printf(msg);
  Source src = Source_new(input);
  Ctx ctx;
  TRY(&ctx) {
    show(parse(p, src, &ctx));
    return true;
  }
  else {
    printf("error:%s\n", ctx.msg);
    mem_free((void*)ctx.msg);
    return false;
  }
}

// ---- TokenParser ----

TokenParser genTokenParser(TokenParserFn f, void* arg) {
  TokenParser p = mem_malloc(sizeof(struct stTokenParser));
  p->run = f;
  p->arg = arg;
  return p;
}

Token parse_Token(TokenParser p, Source src, Ctx* ctx) {
  assert(ctx);
  return p->run(p->arg, src, ctx);
}

bool parseTest_Token(const char* msg, TokenParser p, const char* input) {
  printf(msg);
  Source src = Source_new(input);
  Ctx ctx;
  TRY(&ctx) {
    show(parse(p, src, &ctx));
    return true;
  }
  else {
    printf("error:%s\n", ctx.msg);
    mem_free((void*)ctx.msg);
    return false;
  }
}

// ---- predicates ----
// -- (see 'predicate.c')

// ---- built-in parsers ----

CharParser anyChar;
CharParser digit;
CharParser lower;
CharParser upper;
CharParser alpha;
CharParser alnum;
CharParser letter;
StringParser spaces;
