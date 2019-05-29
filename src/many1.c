/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_many1(void* arg, Source src, Ctx* ex) {
  PARSER(Char) parser = (PARSER(Char))arg;
  Ctx ctx;
  Buffer str = buf_new();
  TRY(&ctx) {
    buf_push(&str, parse(parser, src, &ctx));
    buf_append(&str, parse(many(parser), src, &ctx));
    return buf_finish(&str);
  }
  else {
    mem_free((void*)str.data);
    /* catch and re-throw exception */
    cthrow(ex, ctx.msg);
  }
}

PARSER(String) many1(PARSER(Char) p) {
  return PARSER_GEN(String)(run_many1, p);
}
