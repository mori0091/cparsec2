/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <string>

#ifdef parse
#undef parse
#endif

template <typename Parser>
inline auto parse(Parser p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return p->run(p->arg, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline std::string parse(PARSER(String) p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return p->run(p->arg, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

#ifdef seq
#undef seq
template <typename... Parser>
inline PARSER(String) seq(PARSER(Char) p, Parser... args) {
  PARSER(Char) ps[] = {p, args..., NULL};
  return seq_char(ps);
}
#endif
#ifdef SEQ_I
#undef SEQ_I
#endif

#ifdef cons
#undef cons
inline PARSER(String) cons(PARSER(Char) p, PARSER(String) ps) {
  return cons_char(p, ps);
}
#endif

#ifdef either
#undef either
inline PARSER(Char) either(char p1, char p2) {
  return EITHER(c)(p1, p2);
}
inline PARSER(String) either(const char* p1, const char* p2) {
  return EITHER(s)(p1, p2);
}
inline PARSER(Char) either(PARSER(Char) p1, PARSER(Char) p2) {
  return EITHER(Char)(p1, p2);
}
inline PARSER(String) either(PARSER(String) p1, PARSER(String) p2) {
  return EITHER(String)(p1, p2);
}
inline PARSER(Int) either(PARSER(Int) p1, PARSER(Int) p2) {
  return EITHER(Int)(p1, p2);
}
inline PARSER(Token) either(PARSER(Token) p1, PARSER(Token) p2) {
  return EITHER(Token)(p1, p2);
}
#endif

#ifdef tryp
#undef tryp
inline PARSER(Char) tryp(char p) {
  return TRYP(c)(p);
}
inline PARSER(String) tryp(const char* p) {
  return TRYP(s)(p);
}
inline PARSER(Char) tryp(PARSER(Char) p) {
  return TRYP(Char)(p);
}
inline PARSER(String) tryp(PARSER(String) p) {
  return TRYP(String)(p);
}
inline PARSER(Int) tryp(PARSER(Int) p) {
  return TRYP(Int)(p);
}
inline PARSER(Token) tryp(PARSER(Token) p) {
  return TRYP(Token)(p);
}
#endif

#ifdef token
#undef token
inline PARSER(Token) token(int type, char c) {
  return TOKEN(c)(type, c);
}
inline PARSER(Token) token(int type, const char* s) {
  return TOKEN(s)(type, s);
}
inline PARSER(Token) token(int type, PARSER(Char) p) {
  return TOKEN(Char)(type, p);
}
inline PARSER(Token) token(int type, PARSER(String) p) {
  return TOKEN(String)(type, p);
}
#endif
