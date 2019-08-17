/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// initialize cparsec2
void cparsec2_init(void);
// clean up cparsec2 (free all allocated memory)
void cparsec2_end(void);

void* mem_malloc(size_t s);
void* mem_realloc(void* p, size_t s);
void mem_free(void* p);

/** Construct a formated string */
const char* mem_printf(const char* fmt, ...);

/** Construct an error message */
#define error(...) mem_printf(__VA_ARGS__)

#ifdef __cplusplus
}
#endif
