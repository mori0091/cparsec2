/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdbool.h>

#include "sourcepos.h"

#include "macros.h"

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

enum ErrMsgType {
  SysUnexpect = 0,
  Unexpect = 1,
  Expect = 2,
  Message = 3,
};

typedef struct ErrMsgSt {
  enum ErrMsgType type;
  const char* msg;
} ErrMsg;

TYPEDEF_LIST(ErrMsg, ErrMsg);
DECLARE_LIST(ErrMsg);

TYPEDEF_BUFF(ErrMsg, ErrMsg);
DECLARE_BUFF(ErrMsg);

typedef struct {
  SourcePos pos;
  List(ErrMsg) msgs;
} ParseError;

/** Gets the source-position of the ParseError `e` */
SourcePos ParseError_getPos(ParseError e);
/** Sets the source-position to the ParseError `e` */
ParseError ParseError_setPos(SourcePos pos, ParseError e);
/** Returns list of messages in the ParseError `e` */
List(ErrMsg) ParseError_getMessages(ParseError e);
/** Tests whether the ParseError has no message */
bool ParseError_isUnknown(ParseError e);
/** Constructs a ParseError without message */
ParseError ParseError_new(SourcePos pos);
/** Add `msg` to the ParseError `e` */
ParseError ParseError_addMessage(ErrMsg msg, ParseError e);
/** Remove same type messages from `e` and add `msg` to `e` */
ParseError ParseError_setMessage(ErrMsg msg, ParseError e);
/**
 * Merges two errors or select one.
 *
 * - Merges two errors if both source-position were same,
 * - otherwise returns one who has larger source-position.
 */
ParseError ParseError_merge(ParseError e1, ParseError e2);

/**
 * Construct formatted error message string.
 */
const char* ParseError_toString(ParseError e);

/** Tests whether two ParseError have same value */
bool ParseError_isEqual(ParseError e1, ParseError e2);

#ifdef __cplusplus
}
#endif