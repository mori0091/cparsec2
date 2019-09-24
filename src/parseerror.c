/* -*- coding:utf-8-unix -*- */

#include <assert.h>
#include <inttypes.h>
#include <string.h>

#include "cparsec2/alloc.h"
#include "cparsec2/parseerror.h"

DEFINE_LIST(ErrMsg);
DEFINE_BUFF(ErrMsg);

/** Gets the source-offset of the ParseError `e` */
intmax_t ParseError_getOffset(ParseError e) {
  return e.offset;
}
/** Sets the source-offset to the ParseError `e` */
ParseError ParseError_setOffset(intmax_t offset, ParseError e) {
  e.offset = offset;
  return e;
}
/** Gets the source-position of the ParseError `e` */
SourcePos ParseError_getPos(ParseError e) {
  return e.pos;
}
/** Sets the source-position to the ParseError `e` */
ParseError ParseError_setPos(SourcePos pos, ParseError e) {
  e.pos = pos;
  return e;
}
/** Returns list of messages in the ParseError `e` */
List(ErrMsg) ParseError_getMessages(ParseError e) {
  return e.msgs;
}
/** Tests whether the ParseError has no message */
bool ParseError_isUnknown(ParseError e) {
  return LIST_LENGTH(ErrMsg)(e.msgs) == 0;
}
/** Constructs a ParseError without message */
ParseError ParseError_new(void) {
  ParseError e = {0};
  e.pos = SourcePos_new("");
  return e;
}
/** Add `msg` to the ParseError `e` */
ParseError ParseError_addMessage(ErrMsg msg, ParseError e) {
  Buff(ErrMsg) b = {0};
  BUFF_APPEND(ErrMsg)(&b, e.msgs);
  BUFF_PUSH(ErrMsg)(&b, msg);
  e.msgs = BUFF_FINISH(ErrMsg)(&b);
  return e;
}
/** Remove same type messages from `e` and add `msg` to `e` */
ParseError ParseError_setMessage(ErrMsg msg, ParseError e) {
  Buff(ErrMsg) b = {0};
  BUFF_PUSH(ErrMsg)(&b, msg);
  ErrMsg* itr = LIST_BEGIN(ErrMsg)(e.msgs);
  ErrMsg* end = LIST_END(ErrMsg)(e.msgs);
  for (; itr != end; ++itr) {
    if (msg.type != itr->type) {
      BUFF_PUSH(ErrMsg)(&b, *itr);
    }
  }
  e.msgs = BUFF_FINISH(ErrMsg)(&b);
  return e;
}
/**
 * Merges two errors or select one.
 *
 * - Merges two errors if both source-position were same,
 * - otherwise returns one who has larger source-position.
 */
ParseError ParseError_merge(ParseError e1, ParseError e2) {
  if (e1.offset > e2.offset) {
    return e1;
  }
  if (e1.offset < e2.offset) {
    return e2;
  }
  Buff(ErrMsg) b = {0};
  BUFF_APPEND(ErrMsg)(&b, e1.msgs);
  BUFF_APPEND(ErrMsg)(&b, e2.msgs);
  e1.msgs = BUFF_FINISH(ErrMsg)(&b);
  return e1;
}

static const char* format_msgs(const char* prefix, List(String) msgs) {
  Buff(Char) b = {0};
  if (0 < list_length(msgs)) {
    buff_append(&b, mem_printf("%s ", prefix));
    const char** itr = list_begin(msgs);
    const char** end = list_end(msgs);
    switch (list_length(msgs)) {
    case 0:
      break;
    case 1:
      buff_append(&b, *itr++);
      break;
    case 2:
      buff_append(&b, *itr++);
      buff_append(&b, " or ");
      buff_append(&b, *itr++);
      break;
    default:
      while (itr < end - 1) {
        buff_append(&b, *itr++);
        buff_append(&b, ", ");
      }
      buff_append(&b, "or ");
      buff_append(&b, *itr++);
    }
    buff_append(&b, "\n");
  }
  return buff_finish(&b);
}

/**
 * Construct formatted error message string.
 */
const char* ParseError_toString(ParseError e) {
  Buff(Char) b = {0};
  // Parse error:{offset}
  buff_append(
      &b, mem_printf("Parse error:%" PRIdMAX "\n", (intmax_t)e.offset));
  // [filename:]{line}:{column}
  buff_append(&b, SourcePos_toString(e.pos));
  buff_append(&b, "\n");
  //   unexpected: {unexpect}
  //   expecting: {expect1}, {expect2}, ... , or {expectN}
  if (ParseError_isUnknown(e)) {
    buff_append(&b, mem_printf("  unknown error\n"));
  } else {
    const char* mSysUnexpect = NULL;
    Buff(String) mUnexpect = {0};
    Buff(String) mExpect = {0};
    Buff(String) mMessage = {0};
    ErrMsg* itr = LIST_BEGIN(ErrMsg)(e.msgs);
    ErrMsg* end = LIST_END(ErrMsg)(e.msgs);
    for (; itr != end; ++itr) {
      switch (itr->type) {
      case SysUnexpect:
        if (!mSysUnexpect) {
          mSysUnexpect = itr->msg;
        }
        break;
      case Unexpect:
        buff_push(&mUnexpect, itr->msg);
        break;
      case Expect:
        buff_push(&mExpect, itr->msg);
        break;
      case Message:
        buff_push(&mMessage, itr->msg);
        break;
      default:
        assert(0 && "invalid message type");
      }
    }
    if (mSysUnexpect) {
      List(String) lst = buff_finish(&mUnexpect);
      if (!list_length(lst)) {
        buff_push(&mUnexpect, mSysUnexpect);
      } else {
        buff_append(&mUnexpect, lst);
      }
    }
    buff_append(&b, format_msgs("  unexpected", buff_finish(&mUnexpect)));
    buff_append(&b, format_msgs("  expecting", buff_finish(&mExpect)));
    buff_append(&b, format_msgs("  ", buff_finish(&mMessage)));
  }
  return buff_finish(&b);
}

/** Tests whether two ParseError have same value */
bool ParseError_isEqual(ParseError e1, ParseError e2) {
  if (e1.offset != e2.offset) {
    return false;
  }
  if (LIST_LENGTH(ErrMsg)(e1.msgs) != LIST_LENGTH(ErrMsg)(e2.msgs)) {
    return false;
  }
  ErrMsg* itr1 = LIST_BEGIN(ErrMsg)(e1.msgs);
  ErrMsg* end1 = LIST_END(ErrMsg)(e1.msgs);
  ErrMsg* itr2 = LIST_BEGIN(ErrMsg)(e2.msgs);
  for (; itr1 != end1; ++itr1, ++itr2) {
    if (itr1->type != itr2->type || strcmp(itr1->msg, itr2->msg)) {
      return false;
    }
  }
  return true;
}
