/* -*- coding: utf-8-unix -*- */
#pragma once

// (1) Define types
typedef struct {
  const char* name;
  int age;
} Person;

// (2) Define 'CPARSEC2_USER_TYPESET' macro, 
//     which should be a list of type names. (comma-separated)
#define CPARSEC2_USER_TYPESET Person

// (3) Then include 'cparsec2.h'
#include <cparsec2.h>
