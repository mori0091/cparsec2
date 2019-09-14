/* -*- coding:utf-8-unix -*- */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef const char* FilePath;

typedef struct {
  FilePath name; ///< source file name
  int line;      ///< line number (>= 1)
  int column;    ///< column number (>= 1)
} SourcePos;

typedef char Token;
typedef const char* Tokens;

SourcePos SourcePos_new(FilePath name);
const char* SourcePos_toString(SourcePos p);
SourcePos SourcePos_advance1(int tabWidth, Token c, SourcePos pos);
SourcePos SourcePos_advanceN(int tabWidth, Tokens cs, SourcePos pos);

#ifdef __cplusplus
}
#endif
