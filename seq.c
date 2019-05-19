/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_seq_char(StringParser self, Source src) {
  Buffer str = buf_new();
  CharParser *p = self->parsers;
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
  buf_push(&str, '\0');
  return (StringResult){.result = str.data};
}

StringParser seq_char(CharParser ps[]) {
  PtrBuffer buf = ptrbuf_new();
  while (*ps) {
    ptrbuf_push(&buf, *ps++);
  }
  ptrbuf_push(&buf, NULL);
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_seq_char;
  self->parsers = (CharParser *)buf.data;
  return self;
}
