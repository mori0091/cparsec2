/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

#include <assert.h>

SourcePos SourcePos_new(FilePath name) {
  if (! name) {
    name = "";
  }
  return (SourcePos){.name = name, .line = 1, .column = 1};
}

const char* SourcePos_toString(SourcePos p) {
  if (p.name && p.name[0]) {
    return mem_printf("%s:%d:%d", p.name, p.line, p.column);
  }
  return mem_printf("%d:%d", p.line, p.column);
}

SourcePos SourcePos_advance1(int tabWidth, Token c, SourcePos pos) {
  switch (c) {
  case '\n':
    pos.line++;
    pos.column = 1;
    break;
  case '\t':
    pos.column = ((pos.column - 1) / tabWidth + 1) * tabWidth + 1;
    break;
  default:
    pos.column++;
  }
  return pos;
}

SourcePos SourcePos_advanceN(int tabWidth, Tokens cs, SourcePos pos) {
  assert(cs && "null pointer");
  for (; *cs; cs++) {
    pos = SourcePos_advance1(tabWidth, *cs, pos);
  }
  return pos;
}
