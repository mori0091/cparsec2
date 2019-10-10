/* -*- coding:utf-8-unix -*- */
#pragma once

#include "metac2.h"

#define ELEMENT_TYPESET TYPESET(0), Ptr
#define TYPESET(n) Char, CAT(TYPESET_, n)()
#define TYPESET_0() String, Int, None, Node
#define TYPESET_1() TYPESET_0(), APPLY(List, TYPESET_0())
