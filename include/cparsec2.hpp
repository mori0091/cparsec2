/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <cparsec2/list.hpp>
#include <string>

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
#endif

template <typename Parser>
inline auto parse(Parser p, Source src) {
  Ctx ctx;
  TRY(&ctx) {
    return p->run(p->arg, src, &ctx);
  }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline std::string parse(PARSER(String) p, Source src) {
  Ctx ctx;
  TRY(&ctx) {
    return p->run(p->arg, src, &ctx);
  }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

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
