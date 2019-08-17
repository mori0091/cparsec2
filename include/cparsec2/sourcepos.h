/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdbool.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  off_t offset;
} SourcePos;

// tests if two SourcePos are same or not
bool isSourcePosEqual(SourcePos p1, SourcePos p2);

#ifdef __cplusplus
}
#endif
