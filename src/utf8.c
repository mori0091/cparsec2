/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

PARSER(String) utf8(const char* s) {
  Source src = Source_new(s);
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      mem_free((void*)parse(anyUtf8, src, &ctx));
    }
  }
  else {
    SourcePos pos = getSourcePos(src);
    assert(pos.offset == list_length(s));
    mem_free((void*)ctx.msg);
  }
  mem_free(src);
  return string1(s);
}
