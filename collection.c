/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

Buffer buf_new(void) {
  return (Buffer){0};
}

void buf_ensure(Buffer *b) {
  if (!b->data) {
    b->data = malloc(8);
    b->capacity = 8;
    b->len = 0;
  }
  if (b->capacity == b->len) {
    b->capacity *= 2;
    b->data = realloc(b->data, b->capacity);
  }
}

void buf_push(Buffer *b, char v) {
  buf_ensure(b);
  b->data[b->len++] = v;
}

PtrBuffer ptrbuf_new(void) {
  return (PtrBuffer){0};
}

void ptrbuf_ensure(PtrBuffer *b) {
  int elemSize = sizeof(void *);
  if (!b->data) {
    b->data = malloc(8 * elemSize);
    b->capacity = 8;
    b->len = 0;
  }
  if (b->capacity == b->len) {
    b->capacity *= 2;
    b->data = realloc(b->data, b->capacity * elemSize);
  }
}

void ptrbuf_push(PtrBuffer *b, void *v)  {
  ptrbuf_ensure(b);
  b->data[b->len++] = v;
}
