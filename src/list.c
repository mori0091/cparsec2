/* -*- coding: utf-8-unix */

#include <assert.h>
#include <cparsec2.h>

// ---- List ----

#define DEFINE_LIST(T, E)                                                \
  E* LIST_BEGIN(T)(List(T) xs) {                                         \
    return xs.data;                                                      \
  }                                                                      \
  E* LIST_END(T)(List(T) xs) {                                           \
    return xs.data + xs.len;                                             \
  }                                                                      \
  int LIST_LENGTH(T)(List(T) xs) {                                       \
    return xs.len;                                                       \
  }                                                                      \
  _Static_assert(1, "")

// List(String)
DEFINE_LIST(String, const char*);

// List(Int)
DEFINE_LIST(Int, int);

// List(Char) ; a special case of List(T)
char* LIST_BEGIN(Char)(List(Char) xs) {
  assert(xs);
  return xs;
}
char* LIST_END(Char)(List(Char) xs) {
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

#define DEFINE_BUFF(T, E)                                                \
  DEFINE_BUFF_COMMON(T, E);                                              \
  DEFINE_BUFF_FINISH(T)

#define DEFINE_BUFF_COMMON(T, E)                                         \
  void BUFF_ENSURE(T)(Buff(T) * b) {                                     \
    if (!b->data) {                                                      \
      b->data = mem_malloc(sizeof(E) * 8);                               \
      b->capacity = 8;                                                   \
      b->len = 0;                                                        \
    }                                                                    \
    if (b->capacity == b->len) {                                         \
      b->capacity *= 2;                                                  \
      b->data = mem_realloc(b->data, sizeof(E) * b->capacity);           \
    }                                                                    \
  }                                                                      \
  void BUFF_PUSH(T)(Buff(T) * b, E x) {                                  \
    BUFF_ENSURE(T)(b);                                                   \
    b->data[b->len++] = x;                                               \
  }                                                                      \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs) {                         \
    E* itr = LIST_BEGIN(T)(xs);                                          \
    E* end = LIST_END(T)(xs);                                            \
    for (; itr != end; itr++) {                                          \
      BUFF_PUSH(T)(b, *itr);                                             \
    }                                                                    \
  }                                                                      \
  _Static_assert(1, "")

#define DEFINE_BUFF_FINISH(T)                                            \
  List(T) BUFF_FINISH(T)(Buff(T) * b) {                                  \
    List(T) xs = (List(T)){.data = b->data, .len = b->len};              \
    *b = (Buff(T)){0};                                                   \
    return xs;                                                           \
  }                                                                      \
  _Static_assert(1, "")

// Buff(String)
DEFINE_BUFF(String, const char*);

// Buff(Int)
DEFINE_BUFF(Int, int);

// Buff(Char) ; a special case of Buff(T)
DEFINE_BUFF_COMMON(Char, char);
List(Char) BUFF_FINISH(Char)(Buff(Char) * b) {
  BUFF_PUSH(Char)(b, '\0');
  List(Char) xs = b->data;
  *b = (Buff(Char)){0};
  return xs;
}
