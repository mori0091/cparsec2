/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("many1(p)", "[cparsec2][parser][many1]") {
  cparsec2_init();
  GIVEN("an input \"1234567890abc\"") {
    Source src = Source_new("1234567890abc");
    WHEN("apply many1(digit)") {
      THEN("results \"1234567890\"") {
        REQUIRE(std::string("1234567890") == parse(many1(digit), src));
      }
    }
  }
  GIVEN("an input \"aaabbb\"") {
    Source src = Source_new("aaabbb");
    WHEN("apply many1(digit)") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(many1(digit), src));
      }
    }
    WHEN("apply many1(char1('a'))") {
      AND_WHEN("apply many1(char1('c'))") {
        THEN("results \"aaa\"") {
          REQUIRE(std::string("aaa") == parse(many1(char1('a')), src));
          AND_THEN("cause an error") {
            REQUIRE_THROWS(parse(many1(char1('c')), src));
          }
        }
      }
    }
    WHEN("apply many1(char1('a'))") {
      AND_WHEN("apply many1(char1('b'))") {
        AND_WHEN("apply a parser (ex. alpha)") {
          THEN("results \"aaa\"") {
            REQUIRE(std::string("aaa") == parse(many1(char1('a')), src));
            AND_THEN("results \"bbb\"") {
              REQUIRE(std::string("bbb") ==
                      parse(many1(char1('b')), src));
              AND_THEN("cause an error") {
                REQUIRE_THROWS(parse(alpha, src));
              }
            }
          }
        }
      }
    }
  }
  GIVEN("an input \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply many1(number)") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(many1(number), src));
      }
    }
    WHEN("apply many1(token(many1(digit)))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(many1(token(many1(digit))), src));
      }
    }
  }
  GIVEN("an input \",123,456,789\"") {
    Source src = Source_new(",123,456,789");
    WHEN("apply many1(skip1st(char1(','), number))") {
      List(Int) xs = parse(many1(skip1st(char1(','), number)), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply many1(skip1st(char1(','), many1(digit)))") {
      List(String) xs =
          parse(many1(skip1st(char1(','), many1(digit))), src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(std::string("456") == itr[1]);
        REQUIRE(std::string("789") == itr[2]);
      }
    }
  }
  GIVEN("an input \",123,abc\"") {
    Source src = Source_new(",123,abc");
    WHEN("apply many1(tryp(skip1st(char1(','), number)))") {
      List(Int) xs = parse(many1(tryp(skip1st(char1(','), number))), src);
      THEN("results [123]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        AND_WHEN("apply string1(\",abc\")") {
          THEN("results \",abc\"") {
            REQUIRE(std::string(",abc") == parse(string1(",abc"), src));
          }
        }
      }
    }
    WHEN("apply many1(tryp(skip1st(char1(','), many1(digit))))") {
      List(String) xs =
          parse(many1(tryp(skip1st(char1(','), many1(digit)))), src);
      THEN("results [\"123\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        AND_WHEN("apply string1(\",abc\")") {
          THEN("results \",abc\"") {
            REQUIRE(std::string(",abc") == parse(string1(",abc"), src));
          }
        }
      }
    }
  }
  GIVEN("an input \",abc\"") {
    Source src = Source_new(",abc");
    WHEN("apply many1(skip1st(char1(','), number))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(many1(skip1st(char1(','), number)), src));
        AND_WHEN("apply string1(\",abc\")") {
          THEN("cause an error") {
            REQUIRE_THROWS(parse(string1(",abc"), src));
          }
        }
      }
    }
    WHEN("apply many1(skip1st(char1(','), many1(digit)))") {
      THEN("cause an error") {
        REQUIRE_THROWS(
            parse(many1(skip1st(char1(','), many1(digit))), src));
        AND_WHEN("apply string1(\",abc\")") {
          THEN("cause an error") {
            REQUIRE_THROWS(parse(string1(",abc"), src));
          }
        }
      }
    }
  }
  cparsec2_end();
}
