/* -*- coding:utf-8-unix -*- */
#pragma once

#include "base.h"

#include "list.h"

#include "buff/class.h"
#include "buff/prototype.h"

#include "buff/generics.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * \brief Appends a printf-like formatted string.
   *
   * \param b       pointer to a char buffer
   * \param format  format string
   * \param ...     arguments for formatting
   *
   * \see printf(3)
   */
  void buff_printf(Buff(Char) * b, const char* format, ...);

  void buff_vprintf(Buff(Char) * b, const char* format, va_list ap);

#ifdef __cplusplus
}
#endif
