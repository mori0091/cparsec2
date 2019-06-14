/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

// ---- resource management ----

/* list of live objects */
static Buff(Ptr) cparsec2_objects = {0};

static void cparsec2_ensure(void) {
  Buff(Ptr)* b = &cparsec2_objects;
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
    Buff(Ptr)* b = &cparsec2_objects;
    b->data[b->len++] = p;
  }
  return p;
}

static int number_fn(void* arg, Source src, Ctx* ex) {
  PARSER(String) p = (PARSER(String))arg;
  return atoi(parse(p, src, ex));
}

static void cparsec2_init__stage0(void) {
  Buff(Ptr)* b = &cparsec2_objects;
  b->data = malloc(sizeof(void*) * 8);
  b->capacity = 8;
  b->len = 0;
  if (!b->data) {
    fprintf(stderr, "fatal:malloc:%s\n", strerror(errno));
    exit(1);
  }
}

static void cparsec2_init__stage1(void) {
  anyChar = satisfy(is_anyChar);
  digit = satisfy(is_digit);
  hexDigit = satisfy(is_hexDigit);
  octDigit = satisfy(is_octDigit);
  lower = satisfy(is_lower);
  upper = satisfy(is_upper);
  alpha = satisfy(is_alpha);
  alnum = satisfy(is_alnum);
  letter = satisfy(is_letter);
  space = satisfy(is_space);
}

static void cparsec2_init__stage2(void) {
  spaces = many(space);
  newline = char1('\n');
  crlf = skip1st(char1('\r'), newline);
  endOfLine = expects("<endOfLine>", either(newline, crlf));
  tab = char1('\t');
  number = PARSER_GEN(Int)(number_fn, token(many1(digit)));
}

void cparsec2_init(void) {
  cparsec2_init__stage0();
  cparsec2_init__stage1();
  cparsec2_init__stage2();
}

void cparsec2_end(void) {
  Buff(Ptr)* b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    free(b->data[i]);
  }
  free(b->data);
  cparsec2_objects = (Buff(Ptr)){0};
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
    Buff(Ptr)* b = &cparsec2_objects;
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
  Buff(Ptr)* b = &cparsec2_objects;
  for (int i = 0; i < b->len; ++i) {
    if (b->data[i] == p) {
      b->data[i] = NULL;
      break;
    }
  }
  free(p);
}

const char* mem_printf(const char* fmt, ...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  len = vsnprintf(NULL, 0, fmt, ap);
  va_end(ap);
  if (len < 0) {
    fprintf(stderr, "vsnprintf(NULL, 0, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  char* buf = mem_malloc(len + 1);
  va_start(ap, fmt);
  len = vsnprintf(buf, len + 1, fmt, ap);
  va_end(ap);
  if (len < 0) {
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

// ---- CharParser ----

DEFINE_PARSER(Char, char) {
  printf("'%c'\n", x);
}

// ---- StringParser ----

DEFINE_PARSER(String, const char*) {
  printf("\"%s\"\n", x);
}

// ---- IntParser ----
DEFINE_PARSER(Int, int) {
  printf("%d\n", x);
}

// ---- predicates ----
// -- (see 'predicate.c')

// ---- built-in parsers ----

CharParser anyChar;
CharParser digit;
CharParser hexDigit;
CharParser octDigit;
CharParser lower;
CharParser upper;
CharParser alpha;
CharParser alnum;
CharParser letter;
CharParser space;
StringParser spaces;
CharParser newline;
CharParser crlf;
CharParser endOfLine;
CharParser tab;
IntParser number;
