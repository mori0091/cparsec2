/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

#include <errno.h>
#include <stdarg.h>
#include <string.h>

// ---- resource management ----

/* list of live objects */
static Buff(Ptr) cparsec2_objects = {0};

static void cparsec2_ensure0(void) {
  Buff(Ptr)* b = &cparsec2_objects;
  b->data = malloc(sizeof(void*) * 8);
  b->capacity = 8;
  b->len = 0;
  if (!b->data) {
    fprintf(stderr, "fatal:malloc:%s\n", strerror(errno));
    exit(1);
  }
}

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

// see 'builtin_parsers.c'
void cparsec2_create_builtin_parsers(void);
// see 'char1.c'
void cparsec2_clear_char1_cache(void);

void cparsec2_init(void) {
  cparsec2_ensure0();
  cparsec2_create_builtin_parsers();
}

void cparsec2_end(void) {
  cparsec2_clear_char1_cache();
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
  Buff(Char) b = {0};
  va_list ap;
  va_start(ap, fmt);
  buff_vprintf(&b, fmt, ap);
  va_end(ap);
  return buff_finish(&b);
}
