/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_seq_char(void *arg, Source src) {
  Buffer str = buf_new();
  CharParser *p = (CharParser *)arg;
  while (*p) {
    CharResult c = parse(*p, src);
    if (c.error) {
      /* if (str.data) free(str.data); */
      /* catch and re-throw exception */
      return (StringResult){.error = c.error};
    }
    buf_push(&str, c.result);
    p++;
  }
  return (StringResult){.result = buf_finish(&str)};
}

StringParser seq_char(CharParser ps[]) {
  PtrBuffer buf = ptrbuf_new();
  while (*ps) {
    ptrbuf_push(&buf, *ps++);
  }
  return genParser(run_seq_char, ptrbuf_finish(&buf));
}
