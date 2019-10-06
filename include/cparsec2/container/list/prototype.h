/* -*- coding:utf-8-unix -*- */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// List(Char)
#define CharList String
typedef const char* List(Char);
typedef const char ELEMENT_TYPE(List(Char));

// List(None)
TYPEDEF_LIST(None, None);

// List(String)
TYPEDEF_LIST(String, const char*);

// List(Int)
TYPEDEF_LIST(Int, int);

// List(Ptr)
TYPEDEF_LIST(Ptr, void*);

// List(Node)
TYPEDEF_LIST(Node, Node);

FOREACH(DECLARE_LIST, ELEMENT_TYPESET);

#ifdef __cplusplus
}
#endif
