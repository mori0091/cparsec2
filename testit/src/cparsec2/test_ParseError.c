/* -*- coding: utf-8-unix -*- */
#define TestSuite ParseError
#include "testit.h"
#include <cparsec2.h>

test("a ParseError w/ no messages is 'unknown error'") {
  ParseError e = ParseError_new();
  c_assert(ParseError_isUnknown(e));
  c_assert(eq(0, e.offset));
  c_assert(eq(1, e.pos.line));
  c_assert(eq(1, e.pos.column));
  c_assert(eq(0, LIST_LENGTH(ErrMsg)(ParseError_getMessages(e))));
}

test("ParseError_addMessage(msg, e) creates a new ParseError "
     "that has all messages in 'e' and the given 'msg'") {
  ParseError e0, e1, e2, e3;
  e0 = ParseError_new();
  e1 = ParseError_addMessage((ErrMsg){Unexpect, "a message #1"}, e0);
  e2 = ParseError_addMessage((ErrMsg){Expect, "a message #2"}, e1);
  e3 = ParseError_addMessage((ErrMsg){Expect, "a message #3"}, e2);
  c_assert(eq(0, LIST_LENGTH(ErrMsg)(ParseError_getMessages(e0))));
  c_assert(eq(1, LIST_LENGTH(ErrMsg)(ParseError_getMessages(e1))));
  c_assert(eq(2, LIST_LENGTH(ErrMsg)(ParseError_getMessages(e2))));
  c_assert(eq(3, LIST_LENGTH(ErrMsg)(ParseError_getMessages(e3))));
}

test("ParseError_setMessage(msg, e) creates a new ParseError "
     "that has the given 'msg'.") {
  ParseError e = ParseError_new();
  e = ParseError_setMessage((ErrMsg){Expect, "foo"}, e);
  List(ErrMsg) msgs = ParseError_getMessages(e);
  c_assert(eq(1, LIST_LENGTH(ErrMsg)(msgs)));
  c_assert(eq(Expect, LIST_BEGIN(ErrMsg)(msgs)[0].type));
  c_assert(eq("foo", LIST_BEGIN(ErrMsg)(msgs)[0].msg));
}

test("ParseError_setMessage(msg, e) creates a new ParseError, "
     "that contains 'msg' and any messages in 'e', "
     "of type differ from 'msg'.") {
  ParseError e1, e2;
  e1 = ParseError_new();
  e1 = ParseError_addMessage((ErrMsg){Unexpect, "foo"}, e1);
  e1 = ParseError_addMessage((ErrMsg){Unexpect, "bar"}, e1);
  e1 = ParseError_addMessage((ErrMsg){Expect, "baz"}, e1);

  e2 = ParseError_setMessage((ErrMsg){Unexpect, "FOO"}, e1);

  List(ErrMsg) msgs = ParseError_getMessages(e2);
  c_assert(eq(2, LIST_LENGTH(ErrMsg)(msgs)));
  c_assert(eq(Unexpect, LIST_BEGIN(ErrMsg)(msgs)[0].type));
  c_assert(eq(Expect, LIST_BEGIN(ErrMsg)(msgs)[1].type));
  c_assert(eq("FOO", LIST_BEGIN(ErrMsg)(msgs)[0].msg));
  c_assert(eq("baz", LIST_BEGIN(ErrMsg)(msgs)[1].msg));
}
