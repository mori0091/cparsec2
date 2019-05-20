/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(void *arg, Source src) {
  Buffer str = buf_new();
  CharParser parser = (CharParser)arg;
  for (;;) {
    CharResult c = parse(parser, src);
    if (c.error) {
      /* catch and discard exception */
      free((void *)c.error);
      break;
    }
    buf_push(&str, c.result);
  }
  return (StringResult){.result = buf_finish(&str)};
}

StringParser many(CharParser p) {
  return genParser(run_many, p);
}
