/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("token(many1(digit))", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"1234\"") {
    Source src = Source_new("1234");
    WHEN("apply number = token(many1(digit))") {
      StringParser number = token(many1(digit));
      THEN("results \"1234\"") {
        REQUIRE(std::string("1234") == parse(number, src));
      }
    }
  }
  GIVEN("an input: \"1\"") {
    Source src = Source_new("1");
    WHEN("apply number = token(many1(digit))") {
      StringParser number = token(many1(digit));
      THEN("value is \"1\"") {
        REQUIRE(std::string("1") == parse(number, src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply number = token(many1(digit))") {
      StringParser number = token(many1(digit));
      THEN("cause an error") {
        REQUIRE_THROWS(parse(number, src));
      }
    }
  }
  GIVEN("an input: \"a123\"") {
    Source src = Source_new("a123");
    WHEN("apply number = token(many1(digit))") {
      StringParser number = token(many1(digit));
      THEN("cause an error") {
        REQUIRE_THROWS(parse(number, src));
      }
    }
  }
  cparsec2_end();
}

SCENARIO("token((char)'+')", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"1 + 2\"") {
    Source src = Source_new("1 + 2");
    WHEN("apply plus = token((char)'+')") {
      CharParser plus = token((char)'+');
      THEN("cause an error") {
        REQUIRE_THROWS(parse(plus, src));
      }
    }
  }
  GIVEN("an input: \"+ 2\"") {
    Source src = Source_new("+ 2");
    WHEN("apply plus = token((char)'+')") {
      CharParser plus = token((char)'+');
      THEN("value is \"+\"") {
        REQUIRE('+' == parse(plus, src));
      }
    }
  }
  GIVEN("an input: \"2\"") {
    Source src = Source_new("2");
    WHEN("apply plus = token((char)'+')") {
      CharParser plus = token((char)'+');
      THEN("cause an error") {
        REQUIRE_THROWS(parse(plus, src));
      }
    }
  }
  cparsec2_end();
}

SCENARIO("token(\"foo\")", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"foo foofoo bar\"") {
    Source src = Source_new("foo foofoo bar");
    WHEN("apply foo = token(\"foo\")") {
      StringParser foo = token("foo");
      THEN("results \"foo\"") {
        REQUIRE(std::string("foo") == parse(foo, src));
        AND_WHEN("apply foo") {
          THEN("results \"foo\"") {
            REQUIRE(std::string("foo") == parse(foo, src));
            AND_WHEN("apply foo") {
              THEN("results \"foo\"") {
                REQUIRE(std::string("foo") == parse(foo, src));
                AND_WHEN("apply foo") {
                  THEN("cause an error") {
                    REQUIRE_THROWS(parse(foo, src));
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  cparsec2_end();
}

SCENARIO("token(PARSER(List(T)))", "[cparsec2][parser][token]") {
  cparsec2_init();
  GIVEN("an input: \"123, 456 ,  789  ,\"") {
    Source src = Source_new("123, 456 ,  789  ,");
    WHEN("apply many(p) where p = skip2nd(number, token(','))") {
      PARSER(Int) p = skip2nd(number, token(','));
      List(Int) xs = parse(many(p), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply many(p) where "
         "p = skip2nd(token(many1(digit)), token(','))") {
      PARSER(String) p = skip2nd(token(many1(digit)), token(','));
      List(String) xs = parse(many(p), src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(std::string("456") == itr[1]);
        REQUIRE(std::string("789") == itr[2]);
      }
    }
  }
  cparsec2_end();
}
