/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "exception.h"
#include "parseerror.h"
#include "sourcepos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stSource* Source;

static inline Source Source_identity(Source src) {
  return src;
}
// Construct new Source object from a string.
Source Source_fromString(const char* text);
// Construct new Source object from a FILE pointer.
Source Source_fromFile(FILE* fp);

// peek head char
char peek(Source src, Ctx* ctx);
// drop head char
void consume(Source src);
// get current source offset (backup state of the source)
intmax_t getSourceOffset(Source src);
// set current souce offset (revert state of the source)
void setSourceOffset(Source src, intmax_t offset);
// get current source position (backup state of the source)
SourcePos getSourcePos(Source src);
// set current souce position (revert state of the source)
void setSourcePos(Source src, SourcePos pos);

const char* getParseErrorAsString(Source src);
ParseError getParseError(Source src);
void setParseError(Source src, ParseError err);

void parseError(Source src, ErrMsg msg);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

inline auto Source_new(const char* text) {
  return Source_fromString(text);
}
inline auto Source_new(FILE* fp) {
  return Source_fromFile(fp);
}
inline auto Source_new(Source src) {
  return Source_identity(src);
}

#else

// Construct new Source object from a string or from a FILE pointer
// clang-format off
#define Source_new(x)                                                   \
  _Generic((x)                                                          \
           , char*       : Source_fromString                            \
           , const char* : Source_fromString                            \
           , FILE*       : Source_fromFile                              \
           , Source      : Source_identity                              \
           )(x)
// clang-format on

#endif
