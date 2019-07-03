/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stStream* Stream;

// Construct new Stream object from a string or from a FILE pointer
// clang-format off
#define Stream_new(x)                           \
  _Generic((x)                                  \
           , char*       : Stream_new_mem       \
           , const char* : Stream_new_mem       \
           , FILE*       : Stream_new_fp        \
           )(x)
// clang-format on

// Construct new Stream object from an array of bytes.
Stream Stream_new_mem(const char* s);
// Construct new Stream object from a FILE pointer.
Stream Stream_new_fp(FILE* fp);

size_t Stream_read(void* ptr, size_t size, Stream s, Ctx* ex);
off_t Stream_getpos(Stream s, Ctx* ex);
void Stream_setpos(off_t pos, Stream s, Ctx* ex);

#ifdef __cplusplus
}
#endif
