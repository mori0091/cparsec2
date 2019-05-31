/* -*- coding: utf-8-unix -*- */

#pragma once

#include <cparsec2.h>

// PARSER(Int) is already declared/defined in CPARSEC2 library.
// So define 'Int' macro to avoid multiple definition.
#define Int Integer

/* declare class PARSER(Int), whose instance return int when applied */
DECLARE_PARSER(Int, int);
