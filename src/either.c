/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_either(void* arg, Source src, Ctx* ex) {
  PARSER(String)* p = (PARSER(String)*)arg;
  const char* state = src->p;
  Ctx ctx;
  TRY(&ctx) {
    return parse(p[0], src, &ctx);
  }
  else if (state != src->p) {
    cthrow(ex, ctx.msg);
  }
  else {
    return parse(p[1], src, ex);
  }
}

PARSER(String) either(PARSER(String) p1, PARSER(String) p2) {
  PARSER(String)* arg = mem_malloc(sizeof(PARSER(String)) * 2);
  arg[0] = p1;
  arg[1] = p2;
  return PARSER_GEN(String)(run_either, arg);
}
