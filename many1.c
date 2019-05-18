/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

char* cons(char c, const char* cs) {
  char *str = malloc(1 + strlen(cs) + 1);
  str[0] = c;
  str[1] = '\0';
  return strcat(str, cs);
}

static StringResult run_many1(StringParser self, Source src) {
  CharResult c = parse(self->parser, src);
  if (c.error) {
    /* catch and re-throw exception */
    return (StringResult){.error = c.error};
  }
  StringParser p = many(self->parser);
  StringResult s = parse(p, src);
  free(p);
  if (s.error) {
    /* catch and re-throw exception */
    return s;
  }
  char *str = cons(c.result, s.result);
  free((void*)s.result);
  return (StringResult){.result = str};
}

StringParser many1(CharParser p) {
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_many1;
  self->parser = p;
  return self;
}
