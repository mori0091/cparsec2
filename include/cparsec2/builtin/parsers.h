/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../parser.h"

CPARSEC2_C_API_BEGIN

extern PARSER(Char) anyChar;
extern PARSER(Char) digit;
extern PARSER(Char) hexDigit;
extern PARSER(Char) octDigit;
extern PARSER(Char) lower;
extern PARSER(Char) upper;
extern PARSER(Char) alpha;
extern PARSER(Char) alnum;
extern PARSER(Char) letter;
extern PARSER(Char) space;
extern PARSER(None) spaces;
extern PARSER(Char) newline;
extern PARSER(Char) crlf;
extern PARSER(Char) endOfLine;
extern PARSER(None) endOfFile;
extern PARSER(Char) tab;
extern PARSER(Int) number;
extern PARSER(String) anyUtf8;

CPARSEC2_C_API_END
