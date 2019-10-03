/* -*- coding: utf-8-unix -*- */

#define TestSuite testit_eq
#include "testit.h"

// -- bool --

test("c_assert(eq(false, false)) should pass") {
  c_assert(eq(false, false));
}

test("c_assert(eq(false, true)) should fail", .should_fail = true) {
  c_assert(eq(false, true));
}

test("c_assert(eq(true, false)) should fail", .should_fail = true) {
  c_assert(eq(true, false));
}

test("c_assert(eq(true, true)) should pass") {
  c_assert(eq(true, true));
}

// -- char --

test("c_assert(eq('a', 'a')) should pass") {
  c_assert(eq('a', 'a'));
}

test("c_assert(eq('a', 'b')) should fail", .should_fail = true) {
  c_assert(eq('a', 'b'));
}

test("c_assert(eq('b', 'a')) should fail", .should_fail = true) {
  c_assert(eq('b', 'a'));
}

test("c_assert(eq('b', 'b')) should pass") {
  c_assert(eq('b', 'b'));
}

// -- integer --

test("c_assert(eq(0, 0)) should pass") {
  c_assert(eq(0, 0));
}

test("c_assert(eq(0, 1)) should fail", .should_fail = true) {
  c_assert(eq(0, 1));
}

test("c_assert(eq(1, 0)) should fail", .should_fail = true) {
  c_assert(eq(1, 0));
}

test("c_assert(eq(1, 1)) should pass") {
  c_assert(eq(1, 1));
}

test("c_assert(eq(-1, -1)) should pass") {
  c_assert(eq(-1, -1));
}

test("c_assert(eq(-1, 1)) should fail", .should_fail = true) {
  c_assert(eq(-1, 1));
}

test("c_assert(eq(1, -1)) should fail", .should_fail = true) {
  c_assert(eq(1, -1));
}

// -- string --

test("c_assert(eq(\"abc\", \"abc\")) should pass") {
  c_assert(eq("abc", "abc"));
}

test("c_assert(eq(\"abc\", \"ab\")) should fail", .should_fail = true) {
  c_assert(eq("abc", "ab"));
}

test("c_assert(eq(\"ab\", \"abc\")) should fail", .should_fail = true) {
  c_assert(eq("ab", "abc"));
}

test("c_assert(eq(\"abc\", \"ABC\")) should fail", .should_fail = true) {
  c_assert(eq("abc", "ABC"));
}

test("c_assert(eq(\"ABC\", \"abc\")) should fail", .should_fail = true) {
  c_assert(eq("ABC", "abc"));
}

test("c_assert(eq(\"ABC\", \"ABC\")) should pass") {
  c_assert(eq("ABC", "ABC"));
}

// -- int8_t --

test("c_assert(eq((int8_t)0, (int8_t)0)) should pass") {
  c_assert(eq((int8_t)0, (int8_t)0));
}

test("c_assert(eq(INT8_MIN, (int8_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT8_MIN, (int8_t)0));
}

test("c_assert(eq((int8_t)0, INT8_MIN)) should fail",
     .should_fail = true) {
  c_assert(eq((int8_t)0, INT8_MIN));
}

test("c_assert(eq(INT8_MIN, INT8_MIN)) should pass") {
  c_assert(eq(INT8_MIN, INT8_MIN));
}

test("c_assert(eq(INT8_MAX, (int8_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT8_MAX, (int8_t)0));
}

test("c_assert(eq((int8_t)0, INT8_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((int8_t)0, INT8_MAX));
}

test("c_assert(eq(INT8_MAX, INT8_MAX)) should pass") {
  c_assert(eq(INT8_MAX, INT8_MAX));
}

// -- uint8_t --

test("c_assert(eq((uint8_t)0, (uint8_t)0)) should pass") {
  c_assert(eq((uint8_t)0, (uint8_t)0));
}

test("c_assert(eq(UINT8_MAX, (uint8_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(UINT8_MAX, (uint8_t)0));
}

test("c_assert(eq((uint8_t)0, UINT8_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((uint8_t)0, UINT8_MAX));
}

test("c_assert(eq(UINT8_MAX, UINT8_MAX)) should pass") {
  c_assert(eq(UINT8_MAX, UINT8_MAX));
}

// -- int16_t --

test("c_assert(eq((int16_t)0, (int16_t)0)) should pass") {
  c_assert(eq((int16_t)0, (int16_t)0));
}

test("c_assert(eq(INT16_MIN, (int16_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT16_MIN, (int16_t)0));
}

test("c_assert(eq((int16_t)0, INT16_MIN)) should fail",
     .should_fail = true) {
  c_assert(eq((int16_t)0, INT16_MIN));
}

test("c_assert(eq(INT16_MIN, INT16_MIN)) should pass") {
  c_assert(eq(INT16_MIN, INT16_MIN));
}

test("c_assert(eq(INT16_MAX, (int16_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT16_MAX, (int16_t)0));
}

test("c_assert(eq((int16_t)0, INT16_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((int16_t)0, INT16_MAX));
}

test("c_assert(eq(INT16_MAX, INT16_MAX)) should pass") {
  c_assert(eq(INT16_MAX, INT16_MAX));
}

// -- uint16_t --

test("c_assert(eq((uint16_t)0, (uint16_t)0)) should pass") {
  c_assert(eq((uint16_t)0, (uint16_t)0));
}

test("c_assert(eq(UINT16_MAX, (uint16_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(UINT16_MAX, (uint16_t)0));
}

test("c_assert(eq((uint16_t)0, UINT16_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((uint16_t)0, UINT16_MAX));
}

test("c_assert(eq(UINT16_MAX, UINT16_MAX)) should pass") {
  c_assert(eq(UINT16_MAX, UINT16_MAX));
}

// -- int32_t --

test("c_assert(eq((int32_t)0, (int32_t)0)) should pass") {
  c_assert(eq((int32_t)0, (int32_t)0));
}

test("c_assert(eq(INT32_MIN, (int32_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT32_MIN, (int32_t)0));
}

test("c_assert(eq((int32_t)0, INT32_MIN)) should fail",
     .should_fail = true) {
  c_assert(eq((int32_t)0, INT32_MIN));
}

test("c_assert(eq(INT32_MIN, INT32_MIN)) should pass") {
  c_assert(eq(INT32_MIN, INT32_MIN));
}

test("c_assert(eq(INT32_MAX, (int32_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT32_MAX, (int32_t)0));
}

test("c_assert(eq((int32_t)0, INT32_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((int32_t)0, INT32_MAX));
}

test("c_assert(eq(INT32_MAX, INT32_MAX)) should pass") {
  c_assert(eq(INT32_MAX, INT32_MAX));
}

// -- uint32_t --

test("c_assert(eq((uint32_t)0, (uint32_t)0)) should pass") {
  c_assert(eq((uint32_t)0, (uint32_t)0));
}

test("c_assert(eq(UINT32_MAX, (uint32_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(UINT32_MAX, (uint32_t)0));
}

test("c_assert(eq((uint32_t)0, UINT32_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((uint32_t)0, UINT32_MAX));
}

test("c_assert(eq(UINT32_MAX, UINT32_MAX)) should pass") {
  c_assert(eq(UINT32_MAX, UINT32_MAX));
}

// -- int64_t --

test("c_assert(eq((int64_t)0, (int64_t)0)) should pass") {
  c_assert(eq((int64_t)0, (int64_t)0));
}

test("c_assert(eq(INT64_MIN, (int64_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT64_MIN, (int64_t)0));
}

test("c_assert(eq((int64_t)0, INT64_MIN)) should fail",
     .should_fail = true) {
  c_assert(eq((int64_t)0, INT64_MIN));
}

test("c_assert(eq(INT64_MIN, INT64_MIN)) should pass") {
  c_assert(eq(INT64_MIN, INT64_MIN));
}

test("c_assert(eq(INT64_MAX, (int64_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(INT64_MAX, (int64_t)0));
}

test("c_assert(eq((int64_t)0, INT64_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((int64_t)0, INT64_MAX));
}

test("c_assert(eq(INT64_MAX, INT64_MAX)) should pass") {
  c_assert(eq(INT64_MAX, INT64_MAX));
}

// -- uint64_t --

test("c_assert(eq((uint64_t)0, (uint64_t)0)) should pass") {
  c_assert(eq((uint64_t)0, (uint64_t)0));
}

test("c_assert(eq(UINT64_MAX, (uint64_t)0)) should fail",
     .should_fail = true) {
  c_assert(eq(UINT64_MAX, (uint64_t)0));
}

test("c_assert(eq((uint64_t)0, UINT64_MAX)) should fail",
     .should_fail = true) {
  c_assert(eq((uint64_t)0, UINT64_MAX));
}

test("c_assert(eq(UINT64_MAX, UINT64_MAX)) should pass") {
  c_assert(eq(UINT64_MAX, UINT64_MAX));
}

