/* -*- coding:utf-8-unix -*- */
#pragma once

#include "parser.h"
#include "runparser.h"

// ---- parser invocation ----

// template<class T>
// T parse(Parser<T> p, Souce src, Ctx *ctx)
#define parse(p, src, ctx) GENERIC_P(p, PARSE, TYPESET(1))(p, src, ctx)

// ---- parser invocation (for debug purpose) ----

// template<class T>
// bool parseTest(Parser<T> p, const char *input);
#define parseTest(p, input) PARSE_TEST_I("", p, input)

// template<class T>
// bool PARSE_TEST(Parser<T> p, const char *input);
#define PARSE_TEST(p, input)                                             \
  PARSE_TEST_I(mem_printf("%s \"%s\" => ", #p, input), p, input)

#define PARSE_TEST_I(msg, p, input)                                      \
  GENERIC_P(p, PARSETEST, TYPESET(1))(msg, p, input)

// template<class T, class S>
// ParseResult<T> runParser(Parser<T> p, S input);
#define runParser(p, input)                                              \
  runParserEx(p, Source_new(input), !isSource(input))

// template<class T>
// ParseResult<T> runParserEx(Parser<T> p, Source src, bool del);
#define runParserEx(p, src, del)                                         \
  (GENERIC_P(PARSER_CAST(p), RUNPARSER_EX, TYPESET(1)))(PARSER_CAST(p),  \
                                                        (src), (del))

#define isSource(input)                                                  \
  (_Generic((input), Source : true, default : false))
