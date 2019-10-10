/* -*- coding:utf-8-unix -*- */
#pragma once

#include "parser.h"
#include "parser_cast.h"
#include "runparser.h"

// ---- parser invocation ----

// template<class T>
// T parse(Parser<T> p, Souce src, Ctx *ctx)
#ifdef __cplusplus

#define parse(p, src) cxx_parse(p, src)
#define DEFINE_CXX_PARSE(T)                                              \
  inline auto cxx_parse(PARSER(T) p, Source src) {                       \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return PARSE(T)(p, src, &ctx);                                     \
    }                                                                    \
    else {                                                               \
      throw std::string(ctx.msg);                                        \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_PARSE, TYPESET(1));

#else

#define parse(p, src, ctx) GENERIC_P(p, PARSE, TYPESET(1))(p, src, ctx)

#endif

// ---- parser invocation (for debug purpose) ----

// template<class T>
// bool parseTest(Parser<T> p, const char *input);
#define parseTest(p, input) PARSE_TEST_I("", p, input)

// template<class T>
// bool PARSE_TEST(Parser<T> p, const char *input);
#define PARSE_TEST(p, input)                                             \
  PARSE_TEST_I(mem_printf("%s \"%s\" => ", #p, input), p, input)

#ifdef __cplusplus

#define PARSE_TEST_I(msg, p, input) cxx_parsetest(msg, p, input)
#define DEFINE_CXX_PARSETEST(T)                                          \
  inline auto cxx_parsetest(const char* msg, PARSER(T) p,                \
                            const char* input) {                         \
    return PARSETEST(T)(msg, p, input);                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_PARSETEST, TYPESET(1));

#else

#define PARSE_TEST_I(msg, p, input)                                      \
  GENERIC_P(p, PARSETEST, TYPESET(1))(msg, p, input)

#endif

// template<class T, class S>
// ParseResult<T> runParser(Parser<T> p, S input);
#define runParser(p, input)                                              \
  runParserEx(p, Source_new(input), !type_eq(Source, (input)))

// template<class T>
// ParseResult<T> runParserEx(Parser<T> p, Source src, bool del);
#ifdef __cplusplus

#define runParserEx(p, src, del)                                         \
  cxx_runParserEx(parser_cast(p), (src), (del))
#define DEFINE_CXX_RUNPARSEREX(T)                                        \
  inline auto cxx_runParserEx(PARSER(T) p, Source src, bool del) {       \
    return RUNPARSER_EX(T)(p, src, del);                                 \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_RUNPARSEREX, TYPESET(1));

#else

#define runParserEx(p, src, del)                                         \
  (GENERIC_P(PARSER_CAST(p), RUNPARSER_EX, TYPESET(1)))(PARSER_CAST(p),  \
                                                        (src), (del))

#endif
