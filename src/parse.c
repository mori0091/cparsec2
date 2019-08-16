/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>
#include <cparsec2/stream.h>

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

static char endOfFile_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    cthrow(ex, mem_printf("expects <eof> but was '%c'", c));
  }
  else {
    mem_free((void*)ctx.msg);
  }
  return 0;
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
  endOfFile = PARSER_GEN(Char)(endOfFile_fn, NULL);
  tab = char1('\t');
  number = PARSER_GEN(Int)(number_fn, token(many1(digit)));
}

static void cparsec2_init__stage3(void) {
  PARSER(Char) t = range(0x80, 0xBF);
  PARSER(String) u1 = seq(range(0x00, 0x7F));
  PARSER(String) u2 = seq(range(0xC2, 0xDF), t);
  PARSER(String) u3a = seq(char1((char)0xE0), range(0xA0, 0xBF), t);
  PARSER(String) u3b = seq(range(0xE1, 0xEC), t, t);
  PARSER(String) u3c = seq(char1((char)0xED), range(0x80, 0x9F));
  PARSER(String) u3d = seq(range(0xEE, 0xEF), t, t);
  PARSER(String) u4a = seq(char1((char)0xF0), range(0x90, 0xBF), t, t);
  PARSER(String) u4b = seq(range(0xF1, 0xF3), t, t, t);
  PARSER(String) u4c = seq(char1((char)0xF4), range(0x80, 0xBF), t, t);
  anyUtf8 = TREE_FOLDL(EITHER(String), u1, u2, u3a, u3b, u3c, u3d, u4a,
                       u4b, u4c);
}

void cparsec2_init(void) {
  cparsec2_init__stage0();
  cparsec2_init__stage1();
  cparsec2_init__stage2();
  cparsec2_init__stage3();
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

// ---- error and exception handling ----

_Noreturn void cthrow(Ctx* ctx, const char* msg) {
  ctx->msg = msg;
  longjmp(ctx->e, -1);
}

// ---- source of input character sequence ----

struct stSource {
  Stream s;
};

Source newStringSource(const char* text) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(text);
  return src;
}

Source newFileSource(FILE* fp) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(fp);
  return src;
}

char peek(Source src, Ctx* ctx) {
  char c;
  off_t pos = Stream_getpos(src->s, ctx);
  if (!Stream_read((void*)&c, 1, src->s, ctx)) {
    cthrow(ctx, mem_printf("too short"));
  }
  Stream_setpos(pos, src->s, ctx);
  return c;
}

void consume(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    char c;
    if (!Stream_read((void*)&c, 1, src->s, &ctx)) {
      cthrow(&ctx, mem_printf("too short"));
    }
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

SourcePos getSourcePos(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    return (SourcePos){.offset = Stream_getpos(src->s, &ctx)};
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

void setSourcePos(Source src, SourcePos pos) {
  Ctx ctx;
  TRY(&ctx) {
    Stream_setpos(pos.offset, src->s, &ctx);
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

bool isSourcePosEqual(SourcePos p1, SourcePos p2) {
  return p1.offset == p2.offset;
}

// ---- CharParser ----

DEFINE_PARSER(Char, x) {
  printf("'%c'\n", x);
}

// ---- StringParser ----

DEFINE_PARSER(String, x) {
  printf("\"%s\"\n", x);
}

// ---- IntParser ----
DEFINE_PARSER(Int, x) {
  printf("%d\n", x);
}

// ---- StringListParser ----
DEFINE_PARSER(List(String), x) {
  const char** itr = list_begin(x);
  const char** end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[\"%s\"", *itr++);
    while (itr != end) {
      printf(", \"%s\"", *itr++);
    }
    printf("]\n");
  }
}

// ---- IntListParser ----
DEFINE_PARSER(List(Int), x) {
  int* itr = list_begin(x);
  int* end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[%d", *itr++);
    while (itr != end) {
      printf(", %d", *itr++);
    }
    printf("]\n");
  }
}

// ---- NodeParser ----
DEFINE_PARSER(Node, x) {
  printf("<Node:%p>\n", (void*)x);
}

// ---- NodeListParser ----
DEFINE_PARSER(List(Node), x) {
  Node* itr = list_begin(x);
  Node* end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[<Node:%p>", (void*)*itr++);
    while (itr != end) {
      printf(", <Node:%p>", (void*)*itr++);
    }
    printf("]\n");
  }
}

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
CharParser endOfFile;
CharParser tab;
IntParser number;
StringParser anyUtf8;
