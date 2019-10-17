/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

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

FOREACH(DEFINE_LIST, Ptr, TYPESET_0());
FOREACH(DEFINE_BUFF, Ptr, TYPESET_0());
FOREACH(DEFINE_SHOW_LIST, TYPESET_0());
FOREACH(DEFINE_PARSER, TYPESET(1));
FOREACH(DEFINE_RUNPARSER, TYPESET(1));
FOREACH(CPARSEC2_DEFINE_COMBINATORS_0, TYPESET(0));
FOREACH(CPARSEC2_DEFINE_COMBINATORS_1, TYPESET(1));
