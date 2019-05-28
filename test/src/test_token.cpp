/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("token(TK_NUMBER, many1(digit))", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"1234\"") {
    Source src = Source_new("1234");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number = token(TK_NUMBER, many1(digit))") {
      Token tok = parse(number, src);
      THEN("results a token whose type is TK_NUMBER") {
        REQUIRE(TK_NUMBER == tok->type);
        AND_THEN("value is \"1234\"") {
          REQUIRE("1234" == std::string(tok->str));
        }
      }
    }
  }
  GIVEN("an input: \"1\"") {
    Source src = Source_new("1");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number = token(TK_NUMBER, many1(digit))") {
      Token tok = parse(number, src);
      THEN("results a token whose type is TK_NUMBER") {
        REQUIRE(TK_NUMBER == tok->type);
        AND_THEN("value is \"1\"") {
          REQUIRE("1" == std::string(tok->str));
        }
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number = token(TK_NUMBER, many1(digit))") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(number, src), "too short");
      }
    }
  }
  GIVEN("an input: \"a123\"") {
    Source src = Source_new("a123");
    TokenParser number = token(TK_NUMBER, many1(digit));
    WHEN("apply number = token(TK_NUMBER, many1(digit))") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(number, src), "not satisfy");
      }
    }
  }
  cparsec2_end();
}

SCENARIO("token('+', (char)'+')", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"1 + 2\"") {
    Source src = Source_new("1 + 2");
    TokenParser plus = token('+', (char)'+');
    WHEN("apply plus = token('+', (char)'+')") {
      THEN("cause exception(\"expects '+' but was '1'\")") {
        REQUIRE_THROWS_WITH(parse(plus, src), "expects '+' but was '1'");
      }
    }
  }
  GIVEN("an input: \" + 2\"") {
    Source src = Source_new(" + 2");
    TokenParser plus = token('+', (char)'+');
    WHEN("apply plus = token('+', (char)'+')") {
      Token tok = parse(plus, src);
      THEN("results a token whose type is '+'") {
        REQUIRE('+' == tok->type);
        AND_THEN("value is \"+\"") {
          REQUIRE("+" == std::string(tok->str));
        }
      }
    }
  }
  GIVEN("an input: \" 2\"") {
    Source src = Source_new(" 2");
    TokenParser plus = token('+', (char)'+');
    WHEN("apply plus = token('+', (char)'+')") {
      THEN("cause exception(\"expects '+' but was '2'\")") {
        REQUIRE_THROWS_WITH(parse(plus, src), "expects '+' but was '2'");
      }
    }
  }
  cparsec2_end();
}
