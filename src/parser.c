/* -*- coding:utf-8-unix -*- */

#include <cparsec2/parser.h>

// ---- CharParser ----
DEFINE_PARSER(Char, x) {
  printf("'%c'\n", x);
}

// ---- StringParser ----
DEFINE_PARSER(String, x) {
  printf("\"%s\"\n", x);
}

// ---- IntParser ----
DEFINE_PARSER(Int, x) {
  printf("%d\n", x);
}

// ---- StringListParser ----
DEFINE_PARSER(List(String), x) {
  const char** itr = list_begin(x);
  const char** end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[\"%s\"", *itr++);
    while (itr != end) {
      printf(", \"%s\"", *itr++);
    }
    printf("]\n");
  }
}

// ---- IntListParser ----
DEFINE_PARSER(List(Int), x) {
  int* itr = list_begin(x);
  int* end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[%d", *itr++);
    while (itr != end) {
      printf(", %d", *itr++);
    }
    printf("]\n");
  }
}

// ---- NodeParser ----
DEFINE_PARSER(Node, x) {
  printf("<Node:%p>\n", (void*)x);
}

// ---- NodeListParser ----
DEFINE_PARSER(List(Node), x) {
  Node* itr = list_begin(x);
  Node* end = list_end(x);
  if (itr == end) {
    printf("[]\n");
  } else {
    printf("[<Node:%p>", (void*)*itr++);
    while (itr != end) {
      printf(", <Node:%p>", (void*)*itr++);
    }
    printf("]\n");
  }
}
