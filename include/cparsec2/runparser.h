/* -*- coding:utf-8-unix -*- */
#pragma once

#include "parser.h"

#define ParseResult(T) CAT(T, ParseResult)
#define RUNPARSER(T) CAT(PARSER(T), _runParser)
#define RUNPARSER_EX(T) CAT(PARSER(T), _runParserEx)

#define TYPEDEF_ParseResult(T)                                           \
  typedef struct {                                                       \
    off_t consumed;                                                      \
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
    off_t offset = getSourceOffset(src);                                 \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      RETURN_TYPE(PARSER(T)) ret = PARSE(T)(p, src, &ctx);               \
      ParseResult(T)                                                     \
          result = {.consumed = getSourceOffset(src) - offset,           \
                    .succeeded = true,                                   \
                    .ok = ret};                                          \
      return result;                                                     \
    }                                                                    \
    ParseResult(T) result = {.consumed = getSourceOffset(src) - offset,  \
                             .succeeded = false,                         \
                             .err = getParseError(src)};                 \
    return result;                                                       \
  }                                                                      \
  ParseResult(T)                                                         \
      RUNPARSER_EX(T)(PARSER(T) p, Source src, bool del_src) {           \
    ParseResult(T) result = RUNPARSER(T)(p, src);                        \
    if (del_src) {                                                       \
      mem_free(src);                                                     \
    }                                                                    \
    return result;                                                       \
  }                                                                      \
  END_OF_STATEMENTS

#ifdef __cplusplus
  extern "C" {
#endif

FOREACH(TYPEDEF_ParseResult, TYPESET(1));
FOREACH(DECLARE_RUNPARSER, TYPESET(1));

#ifdef __cplusplus
  }
#endif
