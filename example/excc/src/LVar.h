/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char* name; // identifier
  int length;       // length of the identifier
  int offset;       // offset address of the local variable
  int size;         // size of the local variable
} LVar;

void clearLVars(void);
int getLVarOffsetMax(void);
LVar getLVar(const char* name, Ctx* ex);
LVar addLVar(const char* name);
LVar findLVar(const char* name);

#ifdef __cplusplus
}
#endif
