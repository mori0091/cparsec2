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

// Construct new Source object from a string or from a FILE pointer
// clang-format off
#define Source_new(x)                                                    \
  _Generic((x)                                                           \
           , char*       : newStringSource                               \
           , const char* : newStringSource                               \
           , FILE*       : newFileSource                                 \
           , Source      : Source_identity                               \
           )(x)
// clang-format on

Source Source_identity(Source src);
// Construct new Source object from a string.
Source newStringSource(const char* text);
// Construct new Source object from a FILE pointer.
Source newFileSource(FILE* fp);
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
