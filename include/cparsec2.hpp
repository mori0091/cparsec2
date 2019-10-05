/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <cparsec2/list.hpp>
#include <string>

#ifdef Source_new
#undef Source_new
inline auto Source_new(const char* text) {
  return newStringSource(text);
}
inline auto Source_new(FILE* fp) {
  return newFileSource(fp);
}
#endif

#ifdef PARSER_CAST
#undef PARSER_CAST
#define PARSER_CAST(expr) parser_cast(expr)
#endif

template <typename P>
inline P parser_cast(P p) {
  return p;
}
inline PARSER(Char) parser_cast(char c) {
  return char1(c);
}
inline PARSER(String) parser_cast(const char* s) {
  return string1(s);
}

#ifdef parse
#undef parse
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
#endif

#ifdef PARSE_TEST_I
#undef PARSE_TEST_I
#define PARSE_TEST_I(msg, p, input) cxx_parsetest(msg, p, input)
#define DEFINE_CXX_PARSETEST(T)                                          \
  inline auto cxx_parsetest(const char* msg, PARSER(T) p,                \
                            const char* input) {                         \
    return PARSETEST(T)(msg, p, input);                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_PARSETEST, TYPESET(1));
#endif

#ifdef runParserEx
#undef runParserEx
#define runParserEx(p, src, del)                                         \
  cxx_runParserEx(parser_cast(p), (src), (del))
#define DEFINE_CXX_RUNPARSEREX(T)                                        \
  inline auto cxx_runParserEx(PARSER(T) p, Source src, bool del) {       \
    return RUNPARSER_EX(T)(p, src, del);                                 \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_RUNPARSEREX, TYPESET(1));
#endif

#ifdef runParser
#undef runParser
#define runParser(p, input)                                              \
  runParserEx((p), Source_new(input),                                    \
              (!std::is_same<Source, decltype(input)>::value))
#endif

#ifdef is_a
#undef is_a
#endif

#ifdef expects
#undef expects
#define expects(desc, p) cxx_expects((desc), parser_cast(p))
#define DEFINE_CXX_EXPECTS(T)                                            \
  inline auto cxx_expects(const char* desc, PARSER(T) p) {               \
    return EXPECTS(T)(desc, p);                                          \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_EXPECTS, TYPESET(1));
#endif

#ifdef many
#undef many
#define many(p) cxx_many(parser_cast(p))
#define DEFINE_CXX_MANY(T)                                               \
  inline auto cxx_many(PARSER(T) p) {                                    \
    return MANY(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_MANY, TYPESET(0));
#endif

#ifdef many1
#undef many1
#define many1(p) cxx_many1(parser_cast(p))
#define DEFINE_CXX_MANY1(T)                                              \
  inline auto cxx_many1(PARSER(T) p) {                                   \
    return MANY1(T)(p);                                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_MANY1, TYPESET(0));
#endif

#ifdef seq
#undef seq
#define seq(...) cxx_seq(__VA_ARGS__)
#define DEFINE_CXX_SEQ(T)                                                \
  template <typename... P>                                               \
  inline auto cxx_seq(PARSER(T) p, P... args) {                          \
    PARSER(T) ps[] = {p, args..., NULL};                                 \
    return SEQ(T)((void**)ps);                                           \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SEQ, TYPESET(0));
#endif

#ifdef cons
#undef cons
#define cons(p, ps) cxx_cons(parser_cast(p), parser_cast(ps))
#define DEFINE_CXX_CONS(T)                                               \
  inline auto cxx_cons(PARSER(T) p, PARSER(List(T)) ps) {                \
    return CONS(T)(p, ps);                                               \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_CONS, TYPESET(0));
#endif

#ifdef skip
#undef skip
#define skip(p) cxx_skip(parser_cast(p))
#define DEFINE_CXX_SKIP(T)                                               \
  inline auto cxx_skip(PARSER(T) p) {                                    \
    return SKIP(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP, TYPESET(1));
#endif

#ifdef skip1st
#undef skip1st
#define skip1st(p1, p2) cxx_skip1st(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_SKIP1ST(T)                                            \
  template <typename P>                                                  \
  inline auto cxx_skip1st(P p1, PARSER(T) p2) {                          \
    return SKIP1ST(T)(skip(p1), p2);                                     \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP1ST, TYPESET(1));
#endif

#ifdef skip2nd
#undef skip2nd
#define skip2nd(p1, p2) cxx_skip2nd(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_SKIP2ND(T)                                            \
  template <typename P>                                                  \
  inline auto cxx_skip2nd(PARSER(T) p1, P p2) {                          \
    return SKIP2ND(T)(p1, skip(p2));                                     \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP2ND, TYPESET(1));
#endif

#ifdef either
#undef either
#define either(p1, p2) cxx_either(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_EITHER(T)                                             \
  inline auto cxx_either(PARSER(T) p1, PARSER(T) p2) {                   \
    return EITHER(T)(p1, p2);                                            \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_EITHER, TYPESET(1));
#endif

#ifdef tryp
#undef tryp
#define tryp(p) cxx_tryp(parser_cast(p))
#define DEFINE_CXX_TRYP(T)                                               \
  inline auto cxx_tryp(PARSER(T) p) {                                    \
    return TRYP(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_TRYP, TYPESET(1));
#endif

#ifdef token
#undef token
#define token(p) cxx_token(parser_cast(p))
#define DEFINE_CXX_TOKEN(T)                                              \
  inline auto cxx_token(PARSER(T) p) {                                   \
    return TOKEN(T)(p);                                                  \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_TOKEN, TYPESET(1));
#endif
