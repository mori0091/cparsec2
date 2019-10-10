/* -*- coding:utf-8-unix -*- */
#pragma once

#include "parser.h"
#include <stdint.h>

#define ParseResult(T) CAT(T, ParseResult)
#define RUNPARSER(T) CAT(PARSER(T), _runParser)
#define RUNPARSER_EX(T) CAT(PARSER(T), _runParserEx)

#define TYPEDEF_ParseResult(T)                                           \
  typedef struct {                                                       \
    intmax_t consumed;                                                   \
    bool succeeded;                                                      \
    union {                                                              \
      RETURN_TYPE(PARSER(T)) ok;                                         \
      ParseError err;                                                    \
    };                                                                   \
  } ParseResult(T);                                                      \
  END_OF_STATEMENTS

#define DECLARE_RUNPARSER(T)                                             \
  ParseResult(T) RUNPARSER(T)(PARSER(T) p, Source src);                  \
  ParseResult(T) RUNPARSER_EX(T)(PARSER(T) p, Source src, bool del_src); \
  END_OF_STATEMENTS

#define DEFINE_RUNPARSER(T)                                              \
  ParseResult(T) RUNPARSER(T)(PARSER(T) p, Source src) {                 \
    intmax_t offset = getSourceOffset(src);                              \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      RETURN_TYPE(PARSER(T)) ret = PARSE(T)(p, src, &ctx);               \
      return (ParseResult(T)){.consumed = getSourceOffset(src) - offset, \
                              .succeeded = true,                         \
                              .ok = ret};                                \
    }                                                                    \
    else {                                                               \
      return (ParseResult(T)){.consumed = getSourceOffset(src) - offset, \
                              .succeeded = false,                        \
                              .err = getParseError(src)};                \
    }                                                                    \
  }                                                                      \
  ParseResult(T) RUNPARSER_EX(T)(PARSER(T) p, Source src, bool del) {    \
    ParseResult(T) result = RUNPARSER(T)(p, src);                        \
    if (del) {                                                           \
      mem_free(src);                                                     \
    }                                                                    \
    return result;                                                       \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN

FOREACH(TYPEDEF_ParseResult, TYPESET(1));
FOREACH(DECLARE_RUNPARSER, TYPESET(1));

CPARSEC2_C_API_END
