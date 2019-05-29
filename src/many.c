/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_many(void* arg, Source src, Ctx* ex) {
  UNUSED(ex);
  PARSER(Char) parser = (PARSER(Char))arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      buf_push(&str, parse(parser, src, &ctx));
    }
  }
  else {
    /* catch and discard exception */
    mem_free((void*)ctx.msg);
  }
  return buf_finish(&str);
}

PARSER(String) many(PARSER(Char) p) {
  return PARSER_GEN(String)(run_many, p);
}
