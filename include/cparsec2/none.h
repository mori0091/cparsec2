/* -*- coding:utf-8-unix -*- */
#pragma once

#include "macros.h"

CPARSEC2_C_API_BEGIN

typedef struct {
  char _;
} None;

static const None NONE = {0};

CPARSEC2_C_API_END

#define isNONE(x) type_eq(None, x)
