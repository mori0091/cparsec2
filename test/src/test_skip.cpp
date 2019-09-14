/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("skip", "[cparsec2][parser][skip]") {
  cparsec2_init();
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply skip(char1('a'))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(char1('a')), src));
        AND_WHEN("apply skip(string1(\"bc\"))") {
          THEN("success") {
            REQUIRE_NOTHROW(parse(skip(string1("bc")), src));
          }
        }
      }
    }
    WHEN("apply skip(char1('b'))") {
      THEN("cause exception(\"expects 'b' but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(skip(char1('b')), src),
                            "expects 'b' but was 'a'");
      }
    }
    WHEN("apply skip(string1(\"ab\"))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(string1("ab")), src));
        AND_WHEN("apply skip(char1('c'))") {
          THEN("success") {
            REQUIRE_NOTHROW(parse(skip(char1('c')), src));
          }
        }
      }
    }
    WHEN("apply skip(string1(\"bc\"))") {
      THEN("cause exception") {
        REQUIRE_THROWS(parse(skip(string1("bc")), src));
      }
    }
  }

  GIVEN("an input: \"123 456 789\"") {
    Source src = Source_new("123 456 789");
    WHEN("apply skip(number)") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(number), src));
        AND_WHEN("apply string1(\"456 789\")") {
          THEN("results \"456 789\"") {
            REQUIRE(std::string("456 789") ==
                    parse(string1("456 789"), src));
          }
        }
      }
    }
    WHEN("apply skip(many1(number))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(many1(number)), src));
        AND_WHEN("apply anyChar") {
          THEN("causes exception(\"too short\")") {
            REQUIRE_THROWS_WITH(parse(anyChar, src), "too short");
          }
        }
      }
    }
    WHEN("apply skip(many1(token(many1(digit))))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(many1(token(many1(digit)))), src));
        AND_WHEN("apply anyChar") {
          THEN("causes exception(\"too short\")") {
            REQUIRE_THROWS_WITH(parse(anyChar, src), "too short");
          }
        }
      }
    }
  }
  cparsec2_end();
}
