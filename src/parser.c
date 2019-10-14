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

FOREACH(DEFINE_SHOW_LIST, TYPESET_0());

FOREACH(DEFINE_PARSER, TYPESET(1));
