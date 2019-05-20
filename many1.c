/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many1(void *arg, Source src) {
  CharParser parser = (CharParser)arg;
  CharResult c = parse(parser, src);
  if (c.error) {
    /* catch and re-throw exception */
    return (StringResult){.error = c.error};
  }
  StringResult s = parse(many(parser), src);
  if (s.error) {
    /* catch and re-throw exception */
    return s;
  }
  Buffer str = buf_new();
  buf_push(&str, c.result);
  buf_append(&str, s.result);
  return (StringResult){.result = buf_finish(&str)};
}

StringParser many1(CharParser p) {
  return genParser(run_many1, p);
}
