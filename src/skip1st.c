/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char skip1st_fn(void* arg, Source src, Ctx* ex) {
  CharParser* ps = (CharParser*)arg;
  parse(ps[0], src, ex);
  return parse(ps[1], src, ex);
}

CharParser skip1st(CharParser p1, CharParser p2) {
  CharParser* ps = mem_malloc(sizeof(CharParser[2]));
  ps[0] = p1;
  ps[1] = p2;
  return PARSER_GEN(Char)(skip1st_fn, ps);
}
