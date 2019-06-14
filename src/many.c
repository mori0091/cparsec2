/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_many(void* arg, Source src, Ctx* ex) {
  UNUSED(ex);
  PARSER(Char) parser = (PARSER(Char))arg;
  Buff(Char) str = {0};
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      buff_push(&str, parse(parser, src, &ctx));
    }
  }
  else {
    /* catch and discard exception */
    mem_free((void*)ctx.msg);
  }
  return buff_finish(&str);
}

PARSER(String) many(PARSER(Char) p) {
  return PARSER_GEN(String)(run_many, p);
}
