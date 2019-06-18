// -*- coding: utf-8-unix -*-

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("Buff(T) and List(T)", "[cparsec2][parser][Buff][List]") {
  cparsec2_init();
  GIVEN("a Buff(Char) buf") {
    Buff(Char) buf = {0};
    WHEN("buff_push(&buf, x) for each x in 'a'..'j'") {
      for (char x = 'a'; x <= 'j'; ++x) {
        buff_push(&buf, x);
      }
      THEN("10 == buf.len <= buf.capacity") {
        REQUIRE(10 == buf.len);
        REQUIRE(buf.len <= buf.capacity);
      }
      AND_WHEN("List(Char) xs = buff_finish(&buf)") {
        List(Char) xs = buff_finish(&buf);
        THEN("buf is cleared") {
          REQUIRE(0 == buf.len);
          REQUIRE(0 == buf.capacity);
          REQUIRE(nullptr == buf.data);
          AND_THEN("list_length(xs) == 10") {
            REQUIRE(list_length(xs) == 10);
            REQUIRE(list_begin(xs) + list_length(xs) == list_end(xs));
            AND_THEN("xs == \"abcdefghij\"") {
              REQUIRE(xs == std::string("abcdefghij"));
            }
          }
        }
      }
    }
  }
  GIVEN("a Buff(Int) buf") {
    Buff(Int) buf = {0};
    WHEN("buff_push(&buf, x) for each x in 1..10") {
      for (int x = 1; x <= 10; ++x) {
        buff_push(&buf, x);
      }
      THEN("10 == buf.len <= buf.capacity") {
        REQUIRE(10 == buf.len);
        REQUIRE(buf.len <= buf.capacity);
      }
      AND_WHEN("List(Int) xs = buff_finish(&buf)") {
        List(Int) xs = buff_finish(&buf);
        THEN("buf is cleared") {
          REQUIRE(0 == buf.len);
          REQUIRE(0 == buf.capacity);
          REQUIRE(nullptr == buf.data);
          AND_THEN("list_length(xs) == 10") {
            REQUIRE(list_length(xs) == 10);
            REQUIRE(list_begin(xs) + list_length(xs) == list_end(xs));
            AND_THEN("sum of all x in xs == 55") {
              int* itr = list_begin(xs);
              int* end = list_end(xs);
              int sum = 0;
              while (itr != end) {
                sum += *itr++;
              }
              REQUIRE(sum == 55);
            }
          }
        }
      }
    }
  }
  GIVEN("a Buff(String) buf") {
    Buff(String) buf = {0};
    WHEN("buff_push(&buf, x) for each x in \"x0\", .. \"x9\"") {
      for (int i = 0; i <= 9; i++) {
        buff_push(&buf, mem_printf("x%d", i));
      }
      THEN("10 == buf.len <= buf.capacity") {
        REQUIRE(10 == buf.len);
        REQUIRE(buf.len <= buf.capacity);
      }
      AND_WHEN("List(String) xs = buff_finish(&buf)") {
        List(String) xs = buff_finish(&buf);
        THEN("buf is cleared") {
          REQUIRE(0 == buf.len);
          REQUIRE(0 == buf.capacity);
          REQUIRE(nullptr == buf.data);
          AND_THEN("list_length(xs) == 10") {
            REQUIRE(list_length(xs) == 10);
            REQUIRE(list_begin(xs) + list_length(xs) == list_end(xs));
            AND_WHEN("const char** itr = list_begin(xs)") {
              THEN("itr[] == {\"x0\", ..., \"x9\"}") {
                const char** itr = list_begin(xs);
                REQUIRE(itr[0] == std::string("x0"));
                REQUIRE(itr[1] == std::string("x1"));
                REQUIRE(itr[2] == std::string("x2"));
                REQUIRE(itr[3] == std::string("x3"));
                REQUIRE(itr[4] == std::string("x4"));
                REQUIRE(itr[5] == std::string("x5"));
                REQUIRE(itr[6] == std::string("x6"));
                REQUIRE(itr[7] == std::string("x7"));
                REQUIRE(itr[8] == std::string("x8"));
                REQUIRE(itr[9] == std::string("x9"));
              }
            }
          }
        }
      }
    }
  }
  GIVEN("a Buff(Ptr) buf") {
    Buff(Ptr) buf = {0};
    WHEN("buff_push(&buf, x) for each x in 1..10") {
      for (intptr_t x = 1; x <= 10; ++x) {
        buff_push(&buf, (void*)x);
      }
      THEN("10 == buf.len <= buf.capacity") {
        REQUIRE(10 == buf.len);
        REQUIRE(buf.len <= buf.capacity);
      }
      AND_WHEN("List(Ptr) xs = buff_finish(&buf)") {
        List(Ptr) xs = buff_finish(&buf);
        THEN("buf is cleared") {
          REQUIRE(0 == buf.len);
          REQUIRE(0 == buf.capacity);
          REQUIRE(nullptr == buf.data);
          AND_THEN("list_length(xs) == 10") {
            REQUIRE(list_length(xs) == 10);
            REQUIRE(list_begin(xs) + list_length(xs) == list_end(xs));
            AND_WHEN("void** itr = list_begin(xs)") {
              THEN("itr[] == {(void*)1, ..., (void*)10}") {
                void** itr = list_begin(xs);
                REQUIRE(itr[0] == (void*)1);
                REQUIRE(itr[1] == (void*)2);
                REQUIRE(itr[2] == (void*)3);
                REQUIRE(itr[3] == (void*)4);
                REQUIRE(itr[4] == (void*)5);
                REQUIRE(itr[5] == (void*)6);
                REQUIRE(itr[6] == (void*)7);
                REQUIRE(itr[7] == (void*)8);
                REQUIRE(itr[8] == (void*)9);
                REQUIRE(itr[9] == (void*)10);
              }
            }
          }
        }
      }
    }
  }
  cparsec2_end();
}
