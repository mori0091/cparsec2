/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("many(p)", "[cparsec2][parser][many]") {
  cparsec2_init();
  GIVEN("an input \"1234567890abc\"") {
    Source src = Source_new("1234567890abc");
    WHEN("apply many(digit)") {
      THEN("results \"1234567890\"") {
        REQUIRE(std::string("1234567890") == parse(many(digit), src));
      }
    }
  }
  GIVEN("an input \"aaabbb\"") {
    Source src = Source_new("aaabbb");
    WHEN("apply many(digit)") {
      THEN("results \"\"") {
        REQUIRE(std::string("") == parse(many(digit), src));
      }
    }
    WHEN("apply many(char1('a'))") {
      AND_WHEN("apply many(char1('c'))") {
        THEN("results \"aaa\"") {
          REQUIRE(std::string("aaa") == parse(many(char1('a')), src));
          AND_THEN("results \"\"") {
            REQUIRE(std::string("") == parse(many(char1('c')), src));
          }
        }
      }
    }
    WHEN("apply many(char1('a'))") {
      AND_WHEN("apply many(char1('b'))") {
        AND_WHEN("apply a parser (ex. alpha)") {
          THEN("results \"aaa\"") {
            REQUIRE(std::string("aaa") == parse(many(char1('a')), src));
            AND_THEN("results \"bbb\"") {
              REQUIRE(std::string("bbb") == parse(many(char1('b')), src));
              AND_THEN("cause exception(\"too short\")") {
                REQUIRE_THROWS_WITH(parse(alpha, src), "too short");
              }
            }
          }
        }
      }
    }
  }
  GIVEN("an input \"abcabc\"") {
    Source src = Source_new("abcabc");
    WHEN("apply many(\"abc\")") {
      List(String) xs = parse(many("abc"), src);
      THEN("results [\"abc\", \"abc\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("abc") == itr[0]);
        REQUIRE(std::string("abc") == itr[1]);
      }
    }
  }
  GIVEN("an input \"abcabcab\"") {
    Source src = Source_new("abcabcab");
    WHEN("apply many(\"abc\")") {
      List(String) xs = parse(many("abc"), src);
      THEN("results [\"abc\", \"abc\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("abc") == itr[0]);
        REQUIRE(std::string("abc") == itr[1]);
        AND_WHEN("apply string1(\"ab\")") {
          THEN("results \"ab\"") {
            REQUIRE(std::string("ab") == parse(string1("ab"), src));
          }
        }
      }
    }
  }
  GIVEN("an input \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply many(number)") {
      List(Int) xs = parse(many(number), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
      }
    }
    WHEN("apply many(token(many1(digit)))") {
      List(String) xs = parse(many(token(many1(digit))), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
      }
    }
  }
  GIVEN("an input \",123,456,789\"") {
    Source src = Source_new(",123,456,789");
    WHEN("apply many(skip1st(',', number))") {
      List(Int) xs = parse(many(skip1st(',', number)), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply many(skip1st(',', many1(digit)))") {
      List(String) xs = parse(many(skip1st(',', many1(digit))), src);
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
    WHEN("apply many(skip1st(','), number)") {
      THEN("cause an exception") {
        REQUIRE_THROWS(parse(many(skip1st(',', number)), src));
        // std::cerr << getParseErrorAsString(src) << std::endl;
        // | ...
        // | unexpected 'a'
        // | expecting a number
      }
    }
    WHEN("apply many(tryp(skip1st(','), number))") {
      List(Int) xs = parse(many(tryp(skip1st(',', number))), src);
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
    WHEN("apply many(tryp(skip1st(',', many1(digit))))") {
      List(String) xs =
          parse(many(tryp(skip1st(',', many1(digit)))), src);
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
    WHEN("apply many(tryp(skip1st(',', number)))") {
      List(Int) xs = parse(many(tryp(skip1st(',', number))), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
        AND_WHEN("apply string1(\",abc\")") {
          THEN("results \",abc\"") {
            REQUIRE(std::string(",abc") == parse(string1(",abc"), src));
          }
        }
      }
    }
    WHEN("apply many(tryp(skip1st(',', many1(digit))))") {
      List(String) xs =
          parse(many(tryp(skip1st(',', many1(digit)))), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
        AND_WHEN("apply string1(\",abc\")") {
          THEN("results \",abc\"") {
            REQUIRE(std::string(",abc") == parse(string1(",abc"), src));
          }
        }
      }
    }
  }
  cparsec2_end();
}
