/* -*- coding:utf-8-unix -*- */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Buff(None)
TYPEDEF_BUFF(None, None);

// Buff(Char)
TYPEDEF_BUFF(Char, char);

// Buff(String)
TYPEDEF_BUFF(String, const char*);

// Buff(Int)
TYPEDEF_BUFF(Int, int);

// Buff(Ptr)
TYPEDEF_BUFF(Ptr, void*);

// Buff(Node)
TYPEDEF_BUFF(Node, Node);

FOREACH(DECLARE_BUFF, ELEMENT_TYPESET);

#ifdef __cplusplus
}
#endif
