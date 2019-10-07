/* -*- coding: utf-8-unix -*- */

#include <assert.h>
#include <string.h>

#include <cparsec2/container/buff.h>
#include <cparsec2/container/list.h>

// ---- Buffer (List builder) ----

// Buff(Ptr)
DEFINE_BUFF(Ptr);

// Buff(Char) ; a special case of Buff(T)
DEFINE_BUFF_COMMON(Char);
List(Char) BUFF_FINISH(Char)(Buff(Char) * b) {
  BUFF_PUSH(Char)(b, '\0');
  List(Char) xs = b->data;
  *b = (Buff(Char)){0};
  return xs;
}

// Define Buff(T) for each remaining T in ELEMENT_TYPESET.
FOREACH(DEFINE_BUFF, TYPESET_0());

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

void buff_printf(Buff(Char) * b, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  buff_vprintf(b, format, ap);
  va_end(ap);
}

void buff_vprintf(Buff(Char) * b, const char* format, va_list ap_) {
  int len;
  va_list ap;
  va_copy(ap, ap_);
  len = vsnprintf(NULL, 0, format, ap);
  va_end(ap);
  if (len < 0) {
    fprintf(stderr, "vsnprintf(NULL, 0, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  if (!len) {
    return;
  }
  buff_ensure_by(b, len + 1);
  va_copy(ap, ap_);
  int l = vsnprintf(&(b->data[b->len]), len + 1, format, ap);
  va_end(ap);
  if (l < 0) {
    fprintf(stderr, "vsnprintf(buf, len, fmt, ...):%s\n",
            strerror(errno));
    exit(1);
  }
  b->len += len;
}
