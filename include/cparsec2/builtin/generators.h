/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../parser.h"
#include "../predicate.h"

#include <stdint.h>

CPARSEC2_C_API_BEGIN

PARSER(Char) range(uint8_t min, uint8_t max);
PARSER(Char) oneOf(const char* cs);
PARSER(Char) noneOf(const char* cs);
PARSER(Char) char1(char c);
PARSER(Char) satisfy(Predicate pred);
PARSER(String) string1(const char* s);
PARSER(String) utf8(const char* s);

CPARSEC2_C_API_END
