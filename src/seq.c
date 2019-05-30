/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_seq_char(void* arg, Source src, Ctx* ex) {
  PARSER(Char)* p = (PARSER(Char)*)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    while (*p) {
      buf_push(&str, parse(*p, src, &ctx));
      p++;
    }
    return buf_finish(&str);
  }
  else {
    mem_free((void*)str.data);
    /* catch and re-throw exception */
    cthrow(ex, ctx.msg);
  }
}

PARSER(String) seq_char(PARSER(Char) ps[]) {
  PtrBuffer buf = ptrbuf_new();
  while (*ps) {
    ptrbuf_push(&buf, *ps++);
  }
  return PARSER_GEN(String)(run_seq_char, ptrbuf_finish(&buf));
}
