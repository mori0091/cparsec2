/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

bool is_anyChar(char c) {
  (void)(c);
  return true;
}
bool is_digit(char c) {
  return '0' <= c && c <= '9';
}
bool is_hexDigit(char c) {
  return is_digit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}
bool is_octDigit(char c) {
  return '0' <= c && c <= '7';
}
bool is_lower(char c) {
  return 'a' <= c && c <= 'z';
}
bool is_upper(char c) {
  return 'A' <= c && c <= 'Z';
}
bool is_alpha(char c) {
  return is_lower(c) || is_upper(c);
}
bool is_alnum(char c) {
  return is_digit(c) || is_alpha(c);
}
bool is_letter(char c) {
  return c == '_' || is_alpha(c);
}
bool is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
