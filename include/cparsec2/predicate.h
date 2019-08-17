/* -*- coding:utf-8-unix -*- */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- predicates ----

typedef bool (*Predicate)(char c);

bool is_anyChar(char c);
bool is_digit(char c);
bool is_hexDigit(char c);
bool is_octDigit(char c);
bool is_lower(char c);
bool is_upper(char c);
bool is_alpha(char c);
bool is_alnum(char c);
bool is_letter(char c);
bool is_space(char c);

#ifdef __cplusplus
}
#endif
