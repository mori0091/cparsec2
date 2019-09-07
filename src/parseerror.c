/* -*- coding:utf-8-unix -*- */

#include <assert.h>
#include <inttypes.h>
#include <string.h>

#include "cparsec2/alloc.h"
#include "cparsec2/parseerror.h"

DEFINE_LIST(ErrMsg);
DEFINE_BUFF(ErrMsg);

/** Gets the source-position of the ParseError `e` */
SourcePos ParseError_getPos(ParseError e) {
  return e.pos;
}
/** Sets the source-position to the ParseError `e` */
ParseError ParseError_setPos(SourcePos pos, ParseError e) {
  return (ParseError){.pos = pos, .msgs = e.msgs};
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
ParseError ParseError_new(SourcePos pos) {
  return (ParseError){.pos = pos, .msgs = (List(ErrMsg)){0}};
}
/** Add `msg` to the ParseError `e` */
ParseError ParseError_addMessage(ErrMsg msg, ParseError e) {
  Buff(ErrMsg) b = {0};
  BUFF_APPEND(ErrMsg)(&b, e.msgs);
  BUFF_PUSH(ErrMsg)(&b, msg);
  return (ParseError){.pos = e.pos, .msgs = BUFF_FINISH(ErrMsg)(&b)};
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
  return (ParseError){.pos = e.pos, .msgs = BUFF_FINISH(ErrMsg)(&b)};
}
/**
 * Merges two errors or select one.
 *
 * - Merges two errors if both source-position were same,
 * - otherwise returns one who has larger source-position.
 */
ParseError ParseError_merge(ParseError e1, ParseError e2) {
  if (e1.pos.offset > e2.pos.offset) {
    return e1;
  }
  if (e1.pos.offset < e2.pos.offset) {
    return e2;
  }
  Buff(ErrMsg) b = {0};
  BUFF_APPEND(ErrMsg)(&b, e1.msgs);
  BUFF_APPEND(ErrMsg)(&b, e2.msgs);
  return (ParseError){.pos = e1.pos, .msgs = BUFF_FINISH(ErrMsg)(&b)};
}

static const char* format_msgs(const char* prefix, List(String) msgs) {
  Buff(Char) b = {0};
  if (0 < list_length(msgs)) {
    buff_append(&b, mem_printf("%s ", prefix));
    const char** itr = list_begin(msgs);
    const char** end = list_end(msgs);
    if (itr != end) {
      buff_append(&b, *itr++);
    }
    for (; itr < end - 1; ++itr) {
      buff_append(&b, ", ");
      buff_append(&b, *itr);
    }
    if (itr != end) {
      buff_append(&b, ", or ");
      buff_append(&b, *itr);
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
  buff_append(&b, mem_printf("Parse error:%" PRIdMAX "\n",
                             (intmax_t)e.pos.offset));
  if (ParseError_isUnknown(e)) {
    buff_append(&b, mem_printf("  unknown error\n"));
  } else {
    Buff(String) mUnexpect = {0};
    Buff(String) mExpect = {0};
    Buff(String) mMessage = {0};
    ErrMsg* itr = LIST_BEGIN(ErrMsg)(e.msgs);
    ErrMsg* end = LIST_END(ErrMsg)(e.msgs);
    for (; itr != end; ++itr) {
      switch (itr->type) {
      case SysUnexpect:
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
    buff_append(&b, format_msgs("  unexpected", buff_finish(&mUnexpect)));
    buff_append(&b, format_msgs("  expecting", buff_finish(&mExpect)));
    buff_append(&b, format_msgs("  ", buff_finish(&mMessage)));
  }
  return buff_finish(&b);
}

/** Tests whether two ParseError have same value */
bool ParseError_isEqual(ParseError e1, ParseError e2) {
  if (!isSourcePosEqual(e1.pos, e2.pos)) {
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
