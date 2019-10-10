/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

#define DEFINE_SHOW_LIST(T)                                              \
  void SHOW(List(T))(List(T) xs) {                                       \
    ELEMENT_TYPE(List(T))* itr = LIST_BEGIN(T)(xs);                      \
    ELEMENT_TYPE(List(T))* end = LIST_END(T)(xs);                        \
    if (itr == end) {                                                    \
      printf("[]");                                                      \
    } else {                                                             \
      printf("[");                                                       \
      SHOW(T)(*itr);                                                     \
      itr++;                                                             \
      while (itr != end) {                                               \
        printf(", ");                                                    \
        SHOW(T)(*itr);                                                   \
        itr++;                                                           \
      }                                                                  \
      printf("]");                                                       \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS

void SHOW(Char)(char x) {
  printf("'%c'", x);
}
void SHOW(String)(const char* x) {
  printf("\"%s\"", x);
}
void SHOW(Int)(int x) {
  printf("%d", x);
}
void SHOW(None)(None x) {
  UNUSED(x);
  printf("()");
}
void SHOW(Node)(Node x) {
  printf("<Node:%p>", (void*)x);
}

FOREACH(DEFINE_SHOW_LIST, TYPESET_0());

FOREACH(DEFINE_PARSER, TYPESET(1));
