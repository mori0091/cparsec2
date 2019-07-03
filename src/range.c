/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

static char run_range(void* arg, Source src, Ctx* ex) {
  const uint8_t* minmax = (const uint8_t*)arg;
  Ctx ctx;
  TRY(&ctx) {
    uint8_t c = (uint8_t)peek(src, &ctx);
    if (minmax[0] <= c && c <= minmax[1]) {
      consume(src);
      return (char)c;
    }
    cthrow(&ctx, error("expects one of [%02X, %02X] but was %02X",
                       (int)minmax[0], (int)minmax[1], (int)c));
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

PARSER(Char) range(uint8_t min, uint8_t max) {
  uint8_t* arg = mem_malloc(sizeof(uint8_t) * 2);
  arg[0] = min;
  arg[1] = max;
  return PARSER_GEN(Char)(run_range, (void*)arg);
}
