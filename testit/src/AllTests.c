/* -*- coding: utf-8-unix -*- */
#define TESTIT_CONFIG_MAIN
#include "testit.h"

// `TESITT_CONFIG_MAIN` must be defined in one of the test source files.
//
// When it defined and then "testit.h" was included, code of main()
// function of the testing framework is generated.
// (i.e. No need to implement main() function)
//
// For example, supposing that the following source files are there:
//
// ~~~c
// /** @file: main.c */
// #define TESTIT_CONFIG_MAIN
// #include "testit.h"
// ~~~
//
// ~~~c
// /** @file: test1.c */
// #define TestSuite Test_01
// #include "testit.h"
//
// test("my 1st test") {
//   c_assert(1);
// }
// ~~~
//
// To build test-executable 'AllTests':
//
// ~~~shell
// cc main.c test1.c -o AllTests
// ~~~
//
// To run test:
// ~~~shell
// ./AllTests
// ~~~
//
// To show help messages:
// ~~~shell
// ./AllTests --help
// ~~~
