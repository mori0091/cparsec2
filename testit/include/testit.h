// -*- coding: utf-8-unix -*-
#pragma once

#include <unistd.h>

#ifdef __cplusplus
#define TESTIT_EXTERN extern "C"
#else
#define TESTIT_EXTERN
#endif

#if !defined(TestSuite)
#define TestSuite
#endif

#define TESTIT_CAT(x, y) TESTIT_CAT0(x, y)
#define TESTIT_CAT0(x, y) x##y

#ifdef __COUNTER__
#define TESTIT_ANONYMOUS_ID() TESTIT_CAT(anonymous, __COUNTER__)
#else
#define TESTIT_ANONYMOUS_ID() TESTIT_CAT(anonymous, __LINE__)
#endif

#define test(...) testit(TestSuite, TESTIT_ANONYMOUS_ID(), __VA_ARGS__)
#define testit(suite, name, ...) TESTIT0(suite, name, __VA_ARGS__)

#define c_assert(expr) TESTIT_assert(expr)
#define c_abort(msg) TESTIT_abort(__LINE__, msg)

#define TESTIT0(suite_, name_, ...) TESTIT1(suite_, name_, __VA_ARGS__)
#define TESTIT1(suite_, name_, ...)                                      \
  static void TESTIT_FUNC(suite_, name_)(void);                          \
  TESTIT_EXTERN TestSt TESTIT_TEST(suite_, name_) = {                    \
      .file = __FILE__,                                                  \
      .line = __LINE__,                                                  \
      .suite = #suite_,                                                  \
      .name = #name_,                                                    \
      .run = TESTIT_FUNC(suite_, name_),                                 \
      .title = "" __VA_ARGS__};                                          \
  static void TESTIT_FUNC(suite_, name_)(void)

#define TESTIT_IDENT(prefix, suite, id) TESTIT_IDENT0(prefix, suite, id)
#define TESTIT_IDENT0(prefix, suite, id) prefix##suite##_##id
#define TESTIT_TEST(suite, id) TESTIT_IDENT(TestIt_test__, suite, id)
#define TESTIT_FUNC(suite, id) TESTIT_IDENT(TestIt_run__, suite, id)

#define TESTIT_assert(expr)                                              \
  do {                                                                   \
    if (!(expr)) {                                                       \
      c_abort("Assertion failed `" #expr "'");                           \
    }                                                                    \
  } while (0)

#define TESTIT_abort(line, msg) TESTIT_abort_0(line, msg)
#define TESTIT_abort_0(line, msg)                                        \
  TestIt_abort("" __FILE__ ":" #line ": " msg)

#ifdef __cplusplus
extern "C" {
#endif

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

void TestIt_abort(const char* msg);
void* testit_current_test_data(void);

#ifdef __cplusplus
}
#endif

// ---- main ----
#ifdef TESTIT_CONFIG_MAIN
#ifdef __cplusplus
extern "C" {
#endif
#include <setjmp.h>
#include <stdio.h>

extern Test* testit_tests_;
extern int testit_tests_total_;

struct TestItException {
  const char* msg;
  jmp_buf jmp;
};

static struct TestItException testit_except_;

void TestIt_abort(const char* msg) {
  testit_except_.msg = msg;
  longjmp(testit_except_.jmp, 1);
}

static void* current_test_data_;

void* testit_current_test_data(void) {
  if (!current_test_data_) {
    TestIt_abort(
        "testit_current_test_data() called from a non-parametric test");
  }
  return current_test_data_;
}

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
/* for Windows platform (including Mingw) */
static bool is_ANSI_escape_available(void) {
  return false;
}
#else
/* for Other platform (including MSYS) */
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

static bool runTest(Test t) {
  testit_except_ = (struct TestItException){0};
  volatile bool passed = false;
  if (!setjmp(testit_except_.jmp)) {
    t->run();
    passed = true;
  }
  return (t->should_fail ? !passed : passed);
}

static int runTestRunner(void) {
  const int total_testcases = testit_tests_total_;
  int fail_testcases = 0;
  int total = 0;
  int fail = 0;
  for (int i = 0; i < total_testcases; ++i) {
    Test t = testit_tests_[i];
    if (!t->generator) {
      total++;
      bool passed = runTest(t);
      if (!passed) {
        showError(t, -1);
        fail++;
        fail_testcases++;
      }
    } else {
      int fail_ = 0;
      for (int paramIndex = 0;; paramIndex++) {
        void* data = t->generator(paramIndex);
        if (!data) {
          break;
        }
        total++;
        current_test_data_ = data;
        bool passed = runTest(t);
        current_test_data_ = NULL;
        if (!passed) {
          showError(t, paramIndex);
          fail_++;
        }
      }
      fail += fail_;
      fail_testcases += !!fail_;
    }
  }
  printf("\n");
  if (!fail) {
    print(BOLD GREEN, "All tests passed ");
    printf("(");
    print(BOLD CYAN, "%d tests", total);
    printf(" in %d test cases", total_testcases);
    printf(")\n");
  } else {
    printf("test cases: %4d", total_testcases);
    printf(" | ");
    print(GREEN, "%4d passed", total_testcases - fail_testcases);
    printf(" | ");
    print(BOLD RED, "%4d failed", fail_testcases);
    printf("\n");
    printf("     tests: %4d", total);
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
  const int N = testit_tests_total_;
  for (int i = 0; i < N; ++i) {
    Test t = testit_tests_[i];
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
  printf("%d test cases\n", N);
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

#ifdef __cplusplus
}
#endif

#endif
