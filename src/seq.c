/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_seq_char(void* arg, Source src, Ctx* ex) {
  PARSER(Char)* p = (PARSER(Char)*)arg;
  Buff(Char) str = {0};
  Ctx ctx;
  TRY(&ctx) {
    while (*p) {
      buff_push(&str, parse(*p, src, &ctx));
      p++;
    }
    return buff_finish(&str);
  }
  else {
    mem_free((void*)str.data);
    /* catch and re-throw exception */
    cthrow(ex, ctx.msg);
  }
}

PARSER(String) seq_char(PARSER(Char) ps[]) {
  Buff(Ptr) buf = {0};
  while (*ps) {
    buff_push(&buf, *ps++);
  }
  buff_push(&buf, NULL);
  void* arg = list_begin(buff_finish(&buf));
  return PARSER_GEN(String)(run_seq_char, arg);
}
