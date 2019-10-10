/* -*- coding:utf-8-unix -*- */
#pragma once

#include "metac2.h"
#include <assert.h>

#define END_OF_STATEMENTS static_assert(1, "end of statement")

#ifdef __cplusplus

#define CPARSEC2_C_API extern "C"
#define CPARSEC2_C_API_BEGIN extern "C" {
#define CPARSEC2_C_API_END }

#else

#define CPARSEC2_C_API
#define CPARSEC2_C_API_BEGIN
#define CPARSEC2_C_API_END

#endif

#define UNUSED(v) ((void)v)

// ---- F(T); ... for each T in varargs
#define FOREACH(F, ...) SEP_BY(SEMICOLON, F, __VA_ARGS__)

// ---- _Generic(expr, C(T) : F(T), ...) for each T in varargs
#define GENERIC_METHOD(expr, C, F, ...) GENERIC(expr, C, F, __VA_ARGS__)
// ---- _Generic(expr, PARSER(T) : F(T), ...) for each T in varargs
#define GENERIC_P(expr, F, ...) GENERIC(expr, PARSER, F, __VA_ARGS__)

// ---- True if the type of expr was T, false otherwise
#ifdef __cplusplus
#define type_eq(T, expr) (std::is_same<T, decltype(expr)>::value)
#else
#define type_eq(T, expr) (_Generic((expr), T : true, default : false))
#endif

#define ELEMENT_TYPE(T) CAT(T, _elem_type)

#define RETURN_TYPE(T) CAT(T, _return_type)
