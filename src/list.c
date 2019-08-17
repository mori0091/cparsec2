/* -*- coding: utf-8-unix */

#include <assert.h>
#include <string.h>

#include <cparsec2/list.h>

// ---- List ----

// List(String)
DEFINE_LIST(String);

// List(Int)
DEFINE_LIST(Int);

// List(Ptr)
DEFINE_LIST(Ptr);

// List(Node)
DEFINE_LIST(Node);

// List(Char) ; a special case of List(T)
ELEMENT_TYPE(List(Char)) * LIST_BEGIN(Char)(List(Char) xs) {
  assert(xs);
  return xs;
}
ELEMENT_TYPE(List(Char)) * LIST_END(Char)(List(Char) xs) {
  assert(xs);
  return xs + LIST_LENGTH(Char)(xs);
}
int LIST_LENGTH(Char)(List(Char) xs) {
  assert(xs);
  size_t n = strnlen(xs, SIZE_MAX);
  assert(n < SIZE_MAX);
  return n;
}

// ---- Buffer (List builder) ----

// Buff(String)
DEFINE_BUFF(String);

// Buff(Int)
DEFINE_BUFF(Int);

// Buff(Ptr)
DEFINE_BUFF(Ptr);

// Buff(Node)
DEFINE_BUFF(Node);

// Buff(Char) ; a special case of Buff(T)
DEFINE_BUFF_COMMON(Char);
List(Char) BUFF_FINISH(Char)(Buff(Char) * b) {
  BUFF_PUSH(Char)(b, '\0');
  List(Char) xs = b->data;
  *b = (Buff(Char)){0};
  return xs;
}
