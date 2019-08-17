/* -*- coding:utf-8-unix -*- */

#include <cparsec2/sourcepos.h>

bool isSourcePosEqual(SourcePos p1, SourcePos p2) {
  return p1.offset == p2.offset;
}
