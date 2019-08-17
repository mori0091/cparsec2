/* -*- coding:utf-8-unix -*- */
#pragma once

#include "parser.h"

// ---- parser invocation ----

// T parse(Parser<T> p, Souce src, Ctx *ctx)
#define parse(p, src, ctx) GENERIC_P(p, PARSE, TYPESET(1))(p, src, ctx)

// ---- parser invocation (for debug purpose) ----

// bool parseTest(Parser<T> p, const char *input);
#define parseTest(p, input) PARSE_TEST_I("", p, input)

#define PARSE_TEST(p, input)                                             \
  PARSE_TEST_I(mem_printf("%s \"%s\" => ", #p, input), p, input)

// #define PARSE_TEST_I(msg, p, input) (p->test(msg, p, input))
#define PARSE_TEST_I(msg, p, input)                                      \
  GENERIC_P(p, PARSETEST, TYPESET(1))(msg, p, input)
