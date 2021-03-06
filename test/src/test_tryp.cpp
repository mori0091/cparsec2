/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("tryp(p)", "[cparsec2][parser][tryp]") {
  cparsec2_init();

  GIVEN("an input \"123\"") {
    Source src = Source_new("123");
    WHEN("applied 'tryp(many(digit))'") {
      THEN("resuls \"123\"") {
        REQUIRE(std::string("123") == parse(tryp(many(digit)), src));
      }
    }
    WHEN("applied 'tryp(char1('a'))'") {
      THEN("causes an exception \"expects 'a' but was '1'\"") {
        off_t previous_state = getSourceOffset(src);
        REQUIRE_THROWS_WITH(parse(tryp(char1('a')), src),
                            "expects 'a' but was '1'");
        AND_THEN("input-state is not changed") {
          REQUIRE(previous_state == getSourceOffset(src));
        }
      }
    }
  }

  GIVEN("a PARSER(String) p = either(tryp(\"ab\"), string1(\"bc\"))") {
    PARSER(String) p = either(tryp("ab"), string1("bc"));
    WHEN("an input was \"ab\"") {
      Source src = Source_new("ab");
      THEN("applied 'p' results \"ab\"") {
        REQUIRE(std::string("ab") == parse(p, src));
      }
    }
    WHEN("an input was \"bc\"") {
      Source src = Source_new("bc");
      THEN("applied 'p' results \"bc\"") {
        REQUIRE(std::string("bc") == parse(p, src));
      }
    }
    WHEN("an input was \"ac\"") {
      Source src = Source_new("ac");
      THEN("applied 'p' causes an error") {
        REQUIRE_THROWS(parse(p, src));
      }
    }
    WHEN("an input was \"bd\"") {
      Source src = Source_new("bd");
      THEN("applied 'p' causes an error") {
        REQUIRE_THROWS(parse(p, src));
      }
    }
    WHEN("an input was \"b\"") {
      Source src = Source_new("b");
      THEN("applied 'p' causes an error") {
        REQUIRE_THROWS(parse(p, src));
      }
    }
  }

  cparsec2_end();
}

SCENARIO("tryp(PARSER(List(T)))", "[cparsec2][parser][tryp]") {
  cparsec2_init();
  GIVEN("an input: \",123,456,789\"") {
    Source src = Source_new(",123,456,789");
    WHEN("apply tryp(p) where p = many(skip1st(',', number))") {
      PARSER(List(Int)) p = many(skip1st(',', number));
      List(Int) xs = parse(tryp(p), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply tryp(p) where p = many(skip1st(',', many1(digit)))") {
      PARSER(List(String)) p = many(skip1st(',', many1(digit)));
      List(String) xs = parse(tryp(p), src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(std::string("456") == itr[1]);
        REQUIRE(std::string("789") == itr[2]);
      }
    }
  }
  GIVEN("an input \",abc\"") {
    Source src = Source_new(",abc");
    WHEN("apply tryp(skip1st(char1(','), number))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(tryp(skip1st(char1(','), number)), src));
        AND_WHEN("apply string1(\",abc\")") {
          THEN("results \",abc\"") {
            REQUIRE(std::string(",abc") == parse(string1(",abc"), src));
          }
        }
      }
    }
    WHEN("apply tryp(skip1st(char1(','), many1(digit)))") {
      THEN("cause an error") {
        REQUIRE_THROWS(
            parse(tryp(skip1st(char1(','), many1(digit))), src));
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
