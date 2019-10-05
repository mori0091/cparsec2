/* -*- coding:utf-8-unix -*- */

//#include <cparsec2.h>
#include <cparsec2/stream.h>

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <cparsec2/alloc.h>
#include <cparsec2/container.h>

struct stStream {
  const char* beg;
  const char* end;
  const char* p;
  FILE* fp;
};

// Construct new Stream object from an array of bytes.
Stream Stream_new_mem(const char* p) {
  Stream s = mem_malloc(sizeof(struct stStream));
  s->beg = p;
  s->end = s->beg + list_length(p);
  s->p   = p;
  s->fp  = NULL;
  return s;
}

// Construct new Stream object from a FILE pointer.
Stream Stream_new_fp(FILE* fp) {
  Stream s = mem_malloc(sizeof(struct stStream));
  s->beg = NULL;
  s->end = NULL;
  s->p   = NULL;
  s->fp  = fp;
  return s;
}

size_t Stream_read(void* ptr, size_t size, Stream s, Ctx* ex) {
  if (s->fp) {
    size_t n = fread(ptr, 1, size, s->fp);
    if (!feof(s->fp) && ferror(s->fp)) {
      cthrow(ex, mem_printf("%s", strerror(errno)));
    }
    return n;
  }
  else {
    size_t rem = s->end - s->p;
    size_t n = rem < size ? rem : size;
    if (0 < n) {
      memcpy(ptr, s->p, n);
      s->p += n;
    }
    return n;
  }
}

intmax_t Stream_getpos(Stream s, Ctx* ex) {
  if (s->fp) {
    off_t pos = ftello(s->fp);
    if (pos == -1) {
      cthrow(ex, mem_printf("%s", strerror(errno)));
    }
    return (intmax_t)pos;
  }
  else {
    assert(s->beg <= s->p && s->p <= s->end);
    return (intmax_t)(s->p - s->beg);
  }
}

void Stream_setpos(intmax_t pos, Stream s, Ctx* ex) {
  if (s->fp) {
    if (fseeko(s->fp, (off_t)pos, SEEK_SET) == -1) {
      cthrow(ex, mem_printf("%s", strerror(errno)));
    }
  }
  else {
    s->p = s->beg + pos;
    assert(s->beg <= s->p && s->p <= s->end);
  }
}
