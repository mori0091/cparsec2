/* -*- coding: utf-8-unix -*- */

#include <assert.h>
#include <string.h>

#include <cparsec2/container/buff.h>
#include <cparsec2/container/list.h>

// ---- Buffer (List builder) ----

// Buff(None)
DEFINE_BUFF(None);

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
