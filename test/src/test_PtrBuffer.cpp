/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("PtrBuffer", "[cparsec2][core][collection]") {
  cparsec2_init();
  GIVEN("PtrBUffer buf = ptrbuf_new()") {
    PtrBuffer buf = ptrbuf_new();
    CHECK(buf.len <= buf.capacity);
    CHECK(buf.len == 0);
    WHEN("10 pointers were 'push'ed") {
      AND_WHEN("'finish'ed") {
        for (int i = 1; i <= 10; ++i) {
          ptrbuf_push(&buf, (void*)(intptr_t)i);
        }
        void** p = ptrbuf_finish(&buf);
        CHECK(p);
        THEN("results NULL terminated list of pointers (10 pointers + "
             "NULL)") {
          int i = 1;
          while (*p) {
            REQUIRE((intptr_t)i++ == (intptr_t)*p++);
          }
          CHECK(i == 10 + 1);
        }
      }
    }
    WHEN("10 pointers were 'push'ed") {
      for (int i = 1; i <= 10; ++i) {
        ptrbuf_push(&buf, (void*)(intptr_t)i);
      }
      AND_WHEN("a NULL terminated list of 5 pointers was 'append'ed") {
        void* a[] = {(void*)(intptr_t)11, (void*)(intptr_t)12,
                     (void*)(intptr_t)13, (void*)(intptr_t)14,
                     (void*)(intptr_t)15, NULL};
        ptrbuf_append(&buf, a);
        AND_WHEN("'finish'ed") {
          void** p = ptrbuf_finish(&buf);
          CHECK(p);
          THEN("results NULL terminated list of pointers (15 pointers + "
               "NULL)") {
            int i = 1;
            while (*p) {
              REQUIRE((intptr_t)i++ == (intptr_t)*p++);
            }
            CHECK(i == 15 + 1);
          }
        }
      }
    }
  }
  cparsec2_end();
}
