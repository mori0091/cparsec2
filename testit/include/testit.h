// -*- coding: utf-8-unix -*-
#pragma once

#include <stdint.h>
#include <string.h>
#include <unistd.h>

// ----

#if !defined(TestSuite)
#define TestSuite
#endif

#define test(...) testit(TestSuite, TESTIT_UNIQUE_ID, __VA_ARGS__)
#define testit(suite, name, ...) TESTIT0(suite, name, __VA_ARGS__)

#define c_assert(expr) TESTIT_assert(#expr, expr)
#define c_abort(msg) TESTIT_abort(__LINE__, msg)

#define eq(a, b) TESTIT_BINARY_OPERATOR(testit_eq, a, b)
#define le(a, b) TESTIT_BINARY_OPERATOR(testit_le, a, b)
#define lt(a, b) TESTIT_BINARY_OPERATOR(testit_lt, a, b)
#define ne(a, b) !eq(a, b)
#define ge(a, b) !lt(a, b)
#define gt(a, b) !le(a, b)

// ----

#ifdef __cplusplus
#define TESTIT_CAPI extern "C"
#define TESTIT_BEGIN_CAPI extern "C" {
#define TESTIT_END_CAPI }
#else
#define TESTIT_CAPI
#define TESTIT_BEGIN_CAPI
#define TESTIT_END_CAPI
#endif

#define TESTIT_CAT(x, y) TESTIT_CAT0(x, y)
#define TESTIT_CAT0(x, y) x##y

#ifdef __COUNTER__
#define TESTIT_UNIQUE_ID __COUNTER__
#else
#define TESTIT_UNIQUE_ID __LINE__
#endif

#define TESTIT0(suite_, name_, ...) TESTIT1(suite_, name_, __VA_ARGS__)
#define TESTIT1(suite_, name_, ...)                                      \
  static void TESTIT_TEST_RUN(void);                                     \
  TESTIT_INITIALIZER(TESTIT_ADD_TEST) {                                  \
    static TestSt t = {.file = __FILE__,                                 \
                       .line = __LINE__,                                 \
                       .suite = #suite_,                                 \
                       .name = #name_,                                   \
                       .run = TESTIT_TEST_RUN,                           \
                       .title = "" __VA_ARGS__};                         \
    testit_add_test(&t);                                                 \
  }                                                                      \
  static void TESTIT_TEST_RUN(void)

#define TESTIT_ADD_TEST TESTIT_CAT(TestIt_add__, __LINE__)
#define TESTIT_TEST_RUN TESTIT_CAT(TestIt_run__, __LINE__)

#define TESTIT_INITIALIZER(f) TESTIT_INITIALIZER_0(f)
#define TESTIT_INITIALIZER_0(f)                                          \
  __attribute__((constructor)) static void f(void)

#define TESTIT_assert(exprstr, expr)                                     \
  do {                                                                   \
    if (!(expr)) {                                                       \
      c_abort("Assertion failed `" exprstr "'");                         \
    }                                                                    \
  } while (0)

#define TESTIT_abort(line, msg) TESTIT_abort_0(line, msg)
#define TESTIT_abort_0(line, msg)                                        \
  testit_abort("" __FILE__ ":" #line ": " msg)

// ----

#define TESTIT_BINARY_OPERATOR(op, a, b)                                 \
  _Generic((a)                                                           \
           , int8_t   : op##_I8                                          \
           , int16_t  : op##_I16                                         \
           , int32_t  : op##_I32                                         \
           , int64_t  : op##_I64                                         \
           , uint8_t  : op##_U8                                          \
           , uint16_t : op##_U16                                         \
           , uint32_t : op##_U32                                         \
           , uint64_t : op##_U64                                         \
           , char     : op##_Char                                        \
           , void*    : op##_Ptr                                         \
           , char*    : op##_String                                      \
           )((a), (b))

// ----

TESTIT_BEGIN_CAPI

#include <stdbool.h>

typedef struct TestSt {
  const char* file;
  const int line;
  const char* suite;
  const char* name;
  const char* title;
  void (*run)(void);
  struct {
    // optional properties
    void* (*generator)(size_t index);
    bool should_fail;
  };
} TestSt, *Test;

void testit_add_test(Test t);
void testit_abort(const char* msg);
void* testit_current_test_data(void);

static inline bool testit_eq_I8(int8_t a, int8_t b) {
  return a == b;
}
static inline bool testit_eq_I16(int16_t a, int16_t b) {
  return a == b;
}
static inline bool testit_eq_I32(int32_t a, int32_t b) {
  return a == b;
}
static inline bool testit_eq_I64(int64_t a, int64_t b) {
  return a == b;
}
static inline bool testit_eq_U8(uint8_t a, uint8_t b) {
  return a == b;
}
static inline bool testit_eq_U16(uint16_t a, uint16_t b) {
  return a == b;
}
static inline bool testit_eq_U32(uint32_t a, uint32_t b) {
  return a == b;
}
static inline bool testit_eq_U64(uint64_t a, uint64_t b) {
  return a == b;
}
static inline bool testit_eq_Char(char a, char b) {
  return a == b;
}
static inline bool testit_eq_Ptr(void* a, void* b) {
  return a == b;
}
static inline bool testit_eq_String(const char* a, const char* b) {
  return !strcmp(a, b);
}

static inline bool testit_lt_I8(int8_t a, int8_t b) {
  return a < b;
}
static inline bool testit_lt_I16(int16_t a, int16_t b) {
  return a < b;
}
static inline bool testit_lt_I32(int32_t a, int32_t b) {
  return a < b;
}
static inline bool testit_lt_I64(int64_t a, int64_t b) {
  return a < b;
}
static inline bool testit_lt_U8(uint8_t a, uint8_t b) {
  return a < b;
}
static inline bool testit_lt_U16(uint16_t a, uint16_t b) {
  return a < b;
}
static inline bool testit_lt_U32(uint32_t a, uint32_t b) {
  return a < b;
}
static inline bool testit_lt_U64(uint64_t a, uint64_t b) {
  return a < b;
}
static inline bool testit_lt_Char(char a, char b) {
  return a < b;
}
static inline bool testit_lt_Ptr(void* a, void* b) {
  return a < b;
}
static inline bool testit_lt_String(const char* a, const char* b) {
  return strcmp(a, b) < 0;
}

static inline bool testit_le_I8(int8_t a, int8_t b) {
  return a <= b;
}
static inline bool testit_le_I16(int16_t a, int16_t b) {
  return a <= b;
}
static inline bool testit_le_I32(int32_t a, int32_t b) {
  return a <= b;
}
static inline bool testit_le_I64(int64_t a, int64_t b) {
  return a <= b;
}
static inline bool testit_le_U8(uint8_t a, uint8_t b) {
  return a <= b;
}
static inline bool testit_le_U16(uint16_t a, uint16_t b) {
  return a <= b;
}
static inline bool testit_le_U32(uint32_t a, uint32_t b) {
  return a <= b;
}
static inline bool testit_le_U64(uint64_t a, uint64_t b) {
  return a <= b;
}
static inline bool testit_le_Char(char a, char b) {
  return a <= b;
}
static inline bool testit_le_Ptr(void* a, void* b) {
  return a <= b;
}
static inline bool testit_le_String(const char* a, const char* b) {
  return strcmp(a, b) <= 0;
}

TESTIT_END_CAPI

// ---- main ----
#ifdef TESTIT_CONFIG_MAIN
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TESTIT_BEGIN_CAPI

typedef struct TestListSt* TestList;
struct TestListSt {
  Test head;
  TestList tail;
};

static TestList TestList_cons(Test x, TestList xs) {
  TestList ys = malloc(sizeof(ys[0]));
  assert(ys && "Not enough memory");
  ys->head = x;
  ys->tail = xs;
  return ys;
}

static int TestList_length(TestList xs) {
  int len = 0;
  while (xs) {
    len++;
    xs = xs->tail;
  }
  return len;
}

static TestList tests = 0;

void testit_add_test(Test t) {
  tests = TestList_cons(t, tests);
}

struct TestItException {
  const char* msg;
  jmp_buf jmp;
};

static struct TestItException testit_except_;

void testit_abort(const char* msg) {
  testit_except_.msg = msg;
  longjmp(testit_except_.jmp, 1);
}

static void* current_test_data_;

void* testit_current_test_data(void) {
  if (!current_test_data_) {
    testit_abort("testit_current_test_data() called "
                 "from a non-parametric test");
  }
  return current_test_data_;
}

#ifdef _WIN32
/* for Windows platform (including Mingw) */
static bool is_ANSI_escape_available(void) {
  return false;
}
#else
/* for Other platform (including MSYS) */
#include <unistd.h>
static bool is_ANSI_escape_available(void) {
  return isatty(fileno(stdout));
}
#endif

#define BOLD ";1"
#define DIM ";2"
#define UNDERLINE ";4"
#define BLACK ";30"
#define RED ";31"
#define GREEN ";32"
#define YELLOW ";33"
#define BLUE ";34"
#define MAGENTA ";35"
#define CYAN ";36"
#define WHITE ";37"

static void print(const char* esc, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (!is_ANSI_escape_available()) {
    vprintf(fmt, ap);
  } else {
    printf("\033[0%sm", esc);
    vprintf(fmt, ap);
    printf("\033[0m");
  }
  va_end(ap);
}

static void showError(Test t, int paramIndex) {
  printf("[");
  print(BOLD RED, "%s", "FAIL");
  printf("] ");
  print(BOLD UNDERLINE, "%s",
        (t->title[0] ? t->title : "<no description>"));
  printf("\n");
  printf("| %s:%d: (%s::%s)", t->file, t->line, t->suite, t->name);
  if (0 <= paramIndex) {
    printf(" #%d", paramIndex);
  }
  printf("\n");
  printf("| ");
  print(BOLD YELLOW, "%s", testit_except_.msg);
  printf("\n");
  printf("\n");
}

static bool runTest(Test t, void* data) {
  testit_except_ = (struct TestItException){0};
  volatile bool passed = false;
  current_test_data_ = data;
  if (!setjmp(testit_except_.jmp)) {
    t->run();
    passed = true;
  }
  current_test_data_ = NULL;
  return (t->should_fail ? !passed : passed);
}

static int runTestRunner(void) {
  int tests_total = 0;
  int tests_failed = 0;
  int total = 0;
  int fail = 0;
  for (TestList ts = tests; ts; ts = ts->tail) {
    Test t = ts->head;
    tests_total++;
    int fail_ = 0;
    if (!t->generator) {
      total++;
      bool passed = runTest(t, NULL);
      if (!passed) {
        showError(t, -1);
        fail_++;
      }
    } else {
      for (int paramIndex = 0;; paramIndex++) {
        void* data = t->generator(paramIndex);
        if (!data) {
          break;
        }
        total++;
        bool passed = runTest(t, data);
        if (!passed) {
          showError(t, paramIndex);
          fail_++;
        }
      }
    }
    fail += fail_;
    tests_failed += !!fail_;
  }
  printf("\n");
  if (!fail) {
    print(BOLD GREEN, "All tests passed ");
    printf("(");
    print(BOLD CYAN, "%d testcases", total);
    printf(" in %d tests", tests_total);
    printf(")\n");
  } else {
    printf("tests   : %4d", tests_total);
    printf(" | ");
    print(GREEN, "%4d passed", tests_total - tests_failed);
    printf(" | ");
    print(BOLD RED, "%4d failed", tests_failed);
    printf("\n");
    printf("testcases: %4d", total);
    printf(" | ");
    print(GREEN, "%4d passed", total - fail);
    printf(" | ");
    print(BOLD RED, "%4d failed", fail);
    printf("\n");
  }
  printf("\n");
  return !!fail;
}

static void testit_list_tests(void) {
  for (TestList xs = tests; xs; xs = xs->tail) {
    Test t = xs->head;
    char cardinality = (t->generator ? '*' : ' ');
    printf("[%c] ", cardinality);
    if (t->title[0]) {
      print(BOLD UNDERLINE, "%s", t->title);
    } else {
      print(DIM UNDERLINE, "%s", "<no description>");
    }
    printf("\n");
    printf("    %s:%d (%s::%s)\n", t->file, t->line, t->suite, t->name);
  }
  printf("\n");
  printf("%d test cases\n", TestList_length(tests));
  printf("Note: [*] is a parameterized test (0 < cardinality)\n");
  printf("\n");
}

static void testit_help(const char* appName) {
  printf("%s - TestIt test runner\n", appName);
  printf("\n");
  printf("Usage: %s [options]\n", appName);
  printf("\n");
  printf("Options:\n");
  const char* opt_fmt = "  %-16s  %s\n";
  printf(opt_fmt, "-h, --help", "show this help");
  printf(opt_fmt, "-l, --list", "list all tests");
  printf("\n");
}

static void testit_init(int argc, char** argv) {
  struct {
    bool help;
    bool list;
  } op = {0};
  for (int i = 1; i < argc; i++) {
    const char* arg = argv[i];
    if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
      op.help = true;
      continue;
    }
    if (!strcmp(arg, "-l") || !strcmp(arg, "--list")) {
      op.list = true;
      continue;
    }
    //...
    if (arg[0] == '-') {
      printf("unknown option: %s\n", arg);
      exit(1);
    }
    printf("invalid argument: %s\n", arg);
    exit(1);
  }
  if (op.help) {
    testit_help(argv[0]);
    exit(0);
  }
  if (op.list) {
    testit_list_tests();
    exit(0);
  }
}

int main(int argc, char** argv) {
  testit_init(argc, argv);
  return runTestRunner();
}

TESTIT_END_CAPI

#endif
