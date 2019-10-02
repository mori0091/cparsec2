/* -*- coding: utf-8-unix -*- */
#define TESTIT_CONFIG_MAIN
#include "testit.h"

// `TESITT_CONFIG_MAIN` must be defined in one of the test source files.
//
// When it defined and then "testit.h" was included, code of main()
// function of the testing framework is generated.
//
// **Note**
// The generated `main()` refers an external test-registry (an array of
// testcases), but it is not defined anywhere. The source code of test-registry
// is generated from object files (i.e. *.o or *.a) by `testit.sh`
// post-processing script.
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
// To build test-executable:
//
// Step 1. compile each sources
// ~~~shell
// cc -c main.c -o main.o
// cc -c test1.c -o test1.o
// ~~~
//
// Step 2. generate source code of test-registry.
// ~~~shell
// sh testit.sh main.o test1.o > testdb.c
// ~~~
//
// Step 3. compile and link
// ~~~shell
// cc testdb.c main.o test1.o -o tests
// ~~~
//
// Step 4. run test
// ~~~shell
// ./tests
// ~~~
