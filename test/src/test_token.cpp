/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("token(TK_NUMBER, many1(digit))", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"1234\" and 'number' as token(TK_NUMBER, "
        "many1(digit))") {
    Source src = Source_new("1234");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number") {
      Token tok = parse(number, src);
      THEN("results a token whose type is TK_NUMBER") {
        REQUIRE(TK_NUMBER == tok->type);
        AND_THEN("value is \"1234\"") {
          REQUIRE("1234" == std::string(tok->str));
        }
      }
    }
  }
  GIVEN(
      "an input: \"1\" and 'number' as token(TK_NUMBER, many1(digit))") {
    Source src = Source_new("1");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number") {
      Token tok = parse(number, src);
      THEN("results a token whose type is TK_NUMBER") {
        REQUIRE(TK_NUMBER == tok->type);
        AND_THEN("value is \"1\"") {
          REQUIRE("1" == std::string(tok->str));
        }
      }
    }
  }
  GIVEN("an input: \"\" and 'number' as token(TK_NUMBER, many1(digit))") {
    Source src = Source_new("");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(number, src), "too short");
      }
    }
  }
  GIVEN("an input: \"a123\" and 'number' as token(TK_NUMBER, "
        "many1(digit))") {
    Source src = Source_new("a123");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(number, src), "not satisfy");
      }
    }
  }
  cparsec2_end();
}
