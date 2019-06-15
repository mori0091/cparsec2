/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <cparsec2/list.hpp>
#include <string>

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

#ifdef skip
#undef skip
inline PARSER(Int) skip(PARSER(Char) p) {
  return SKIP(Char)(p);
}
inline PARSER(Int) skip(PARSER(String) p) {
  return SKIP(String)(p);
}
inline PARSER(Int) skip(PARSER(Int) p) {
  return SKIP(Int)(p);
}
#endif

#ifdef skip1st
#undef skip1st
template <typename P>
inline PARSER(Char) skip1st(P p1, PARSER(Char) p2) {
  return SKIP1ST(Char)(skip(p1), p2);
}
template <typename P>
inline PARSER(String) skip1st(P p1, PARSER(String) p2) {
  return SKIP1ST(String)(skip(p1), p2);
}
template <typename P>
inline PARSER(Int) skip1st(P p1, PARSER(Int) p2) {
  return SKIP1ST(Int)(skip(p1), p2);
}
#endif

#ifdef many
#undef many
inline PARSER(List(Char)) many(PARSER(Char) p) {
  return MANY(Char)(p);
}
inline PARSER(List(String)) many(PARSER(String) p) {
  return MANY(String)(p);
}
inline PARSER(List(Int)) many(PARSER(Int) p) {
  return MANY(Int)(p);
}
#endif

#ifdef many1
#undef many1
inline PARSER(List(Char)) many1(PARSER(Char) p) {
  return MANY1(Char)(p);
}
inline PARSER(List(String)) many1(PARSER(String) p) {
  return MANY1(String)(p);
}
inline PARSER(List(Int)) many1(PARSER(Int) p) {
  return MANY1(Int)(p);
}
#endif

#ifdef seq
#undef seq
template <typename... Parser>
inline PARSER(List(Char)) seq(PARSER(Char) p, Parser... args) {
  PARSER(Char) ps[] = {p, args..., NULL};
  return SEQ(Char)((void**)ps);
}
template <typename... Parser>
inline PARSER(List(String)) seq(PARSER(String) p, Parser... args) {
  PARSER(String) ps[] = {p, args..., NULL};
  return SEQ(String)((void**)ps);
}
template <typename... Parser>
inline PARSER(List(Int)) seq(PARSER(Int) p, Parser... args) {
  PARSER(Int) ps[] = {p, args..., NULL};
  return SEQ(Int)((void**)ps);
}
#endif

#ifdef cons
#undef cons
inline PARSER(List(Char)) cons(PARSER(Char) p, PARSER(List(Char)) ps) {
  return CONS(Char)(p, ps);
}
inline PARSER(List(String))
    cons(PARSER(String) p, PARSER(List(String)) ps) {
  return CONS(String)(p, ps);
}
inline PARSER(List(Int)) cons(PARSER(Int) p, PARSER(List(Int)) ps) {
  return CONS(Int)(p, ps);
}
#endif

#ifdef either
#undef either
inline PARSER(Char) either(char c1, char c2) {
  return EITHER(Char)(char1(c1), char1(c2));
}
inline PARSER(Char) either(char c, PARSER(Char) p) {
  return EITHER(Char)(char1(c), p);
}
inline PARSER(Char) either(PARSER(Char) p, char c) {
  return EITHER(Char)(p, char1(c));
}
inline PARSER(Char) either(PARSER(Char) p1, PARSER(Char) p2) {
  return EITHER(Char)(p1, p2);
}
inline PARSER(String) either(const char* s1, const char* s2) {
  return EITHER(String)(string1(s1), string1(s2));
}
inline PARSER(String) either(const char* s, PARSER(String) p) {
  return EITHER(String)(string1(s), p);
}
inline PARSER(String) either(PARSER(String) p, const char* s) {
  return EITHER(String)(p, string1(s));
}
inline PARSER(String) either(PARSER(String) p1, PARSER(String) p2) {
  return EITHER(String)(p1, p2);
}
inline PARSER(Int) either(PARSER(Int) p1, PARSER(Int) p2) {
  return EITHER(Int)(p1, p2);
}
#endif

#ifdef tryp
#undef tryp
inline PARSER(Char) tryp(char p) {
  return TRYP(Char)(char1(p));
}
inline PARSER(Char) tryp(PARSER(Char) p) {
  return TRYP(Char)(p);
}
inline PARSER(String) tryp(const char* s) {
  return TRYP(String)(string1(s));
}
inline PARSER(String) tryp(PARSER(String) p) {
  return TRYP(String)(p);
}
inline PARSER(Int) tryp(PARSER(Int) p) {
  return TRYP(Int)(p);
}
#endif

#ifdef token
#undef token
inline PARSER(Char) token(char c) {
  return TOKEN(Char)(char1(c));
}
inline PARSER(Char) token(PARSER(Char) p) {
  return TOKEN(Char)(p);
}
inline PARSER(String) token(const char* s) {
  return TOKEN(String)(string1(s));
}
inline PARSER(String) token(PARSER(String) p) {
  return TOKEN(String)(p);
}
inline PARSER(Int) token(PARSER(Int) p) {
  return TOKEN(Int)(p);
}
#endif
