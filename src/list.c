/* -*- coding: utf-8-unix */

#include <assert.h>
#include <cparsec2.h>

// ---- List ----

#define DEFINE_LIST(T)                                                   \
  ELEMENT_TYPE(List(T)) * LIST_BEGIN(T)(List(T) xs) {                    \
    return xs.data;                                                      \
  }                                                                      \
  ELEMENT_TYPE(List(T)) * LIST_END(T)(List(T) xs) {                      \
    return xs.data + xs.len;                                             \
  }                                                                      \
  int LIST_LENGTH(T)(List(T) xs) {                                       \
    return xs.len;                                                       \
  }                                                                      \
  END_OF_STATEMENTS

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

#define DEFINE_BUFF(T)                                                   \
  DEFINE_BUFF_COMMON(T);                                                 \
  DEFINE_BUFF_FINISH(T)

#define DEFINE_BUFF_COMMON(T)                                            \
  void BUFF_ENSURE(T)(Buff(T) * b) {                                     \
    if (!b->data) {                                                      \
      b->data = mem_malloc(sizeof(ELEMENT_TYPE(Buff(T))) * 8);           \
      b->capacity = 8;                                                   \
      b->len = 0;                                                        \
    }                                                                    \
    if (b->capacity == b->len) {                                         \
      b->capacity *= 2;                                                  \
      b->data = mem_realloc(b->data, sizeof(ELEMENT_TYPE(Buff(T))) *     \
                                         b->capacity);                   \
    }                                                                    \
  }                                                                      \
  void BUFF_PUSH(T)(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x) {              \
    BUFF_ENSURE(T)(b);                                                   \
    b->data[b->len++] = x;                                               \
  }                                                                      \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs) {                         \
    ELEMENT_TYPE(List(T))* itr = LIST_BEGIN(T)(xs);                      \
    ELEMENT_TYPE(List(T))* end = LIST_END(T)(xs);                        \
    for (; itr != end; itr++) {                                          \
      BUFF_PUSH(T)(b, *itr);                                             \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS

#define DEFINE_BUFF_FINISH(T)                                            \
  List(T) BUFF_FINISH(T)(Buff(T) * b) {                                  \
    List(T) xs = (List(T)){.data = b->data, .len = b->len};              \
    *b = (Buff(T)){0};                                                   \
    return xs;                                                           \
  }                                                                      \
  END_OF_STATEMENTS

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
