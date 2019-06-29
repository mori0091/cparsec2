/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("either(p1, p2)", "[cparsec2][parser][either]") {
  cparsec2_init();

  GIVEN("PARSER(String) p = either(string1(\"ab\"), string1(\"bc\"))") {
    PARSER(String) p = either(string1("ab"), string1("bc"));
    WHEN("applied to an input: \"ab\"") {
      Source src = Source_new("ab");
      THEN("results \"ab\"") {
        REQUIRE(std::string("ab") == parse(p, src));
      }
    }
    WHEN("applied to an input: \"bc\"") {
      Source src = Source_new("bc");
      THEN("results \"bc\"") {
        REQUIRE(std::string("bc") == parse(p, src));
      }
    }
    WHEN("applied to an input: \"ac\"") {
      Source src = Source_new("ac");
      THEN("causes an error \"expects 'b' but was 'c'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'b' but was 'c'");
      }
    }
    WHEN("applied to an input: \"bd\"") {
      Source src = Source_new("bd");
      THEN("causes an error \"expects 'c' but was 'd'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'c' but was 'd'");
      }
    }
    WHEN("applied to an input: \"b\"") {
      Source src = Source_new("b");
      THEN("causes an error \"too short\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "too short");
      }
    }
  }

  GIVEN("PARSER(String) p = either(\"ab\", \"bc\")") {
    PARSER(String) p = either("ab", "bc");
    WHEN("applied to an input: \"ab\"") {
      Source src = Source_new("ab");
      THEN("results \"ab\"") {
        REQUIRE(std::string("ab") == parse(p, src));
      }
    }
    WHEN("applied to an input: \"bc\"") {
      Source src = Source_new("bc");
      THEN("results \"bc\"") {
        REQUIRE(std::string("bc") == parse(p, src));
      }
    }
    WHEN("applied to an input: \"ac\"") {
      Source src = Source_new("ac");
      THEN("causes an error \"expects 'b' but was 'c'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'b' but was 'c'");
      }
    }
    WHEN("applied to an input: \"bd\"") {
      Source src = Source_new("bd");
      THEN("causes an error \"expects 'c' but was 'd'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'c' but was 'd'");
      }
    }
    WHEN("applied to an input: \"b\"") {
      Source src = Source_new("b");
      THEN("causes an error \"too short\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "too short");
      }
    }
  }

  GIVEN("PARSER(Char) p = either('a', 'b')") {
    PARSER(Char) p = either('a', 'b');
    WHEN("applied to an input: \"ab\"") {
      Source src = Source_new("ab");
      THEN("results 'a'") {
        REQUIRE('a' == parse(p, src));
      }
    }
    WHEN("applied to an input: \"bc\"") {
      Source src = Source_new("bc");
      THEN("results 'b'") {
        REQUIRE('b' == parse(p, src));
      }
    }
    WHEN("applied to an input: \"c\"") {
      Source src = Source_new("c");
      THEN("causes an error \"expects 'b' but was 'c'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'b' but was 'c'");
      }
    }
    WHEN("applied to an input: \"\"") {
      Source src = Source_new("");
      THEN("causes an error \"too short\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "too short");
      }
    }
  }

  GIVEN("PARSER(Int) p = either(number, skip1st(\"++\", number))") {
    PARSER(Int) p = either(number, skip1st("++", number));
    WHEN("applied to an input: \"123\"") {
      Source src = Source_new("123");
      THEN("results 123") {
        REQUIRE(123 == parse(p, src));
      }
    }
    WHEN("applied to an input: \"++123\"") {
      Source src = Source_new("++123");
      THEN("results 123") {
        REQUIRE(123 == parse(p, src));
      }
    }
    WHEN("applied to an input: \"+123\"") {
      Source src = Source_new("+123");
      THEN("causes exception(\"expects '+' but was '1'\")") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects '+' but was '1'");
      }
    }
  }

  GIVEN("PARSER(List(Int)) p = either(many1(number), "
        "many1(skip1st(\"++\", number)))") {
    PARSER(List(Int))
    p = either(many1(number), many1(skip1st("++", number)));
    WHEN("applied to an input: \"123++456++789\"") {
      Source src = Source_new("123++456++789");
      List(Int) xs = parse(p, src);
      THEN("results [123]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(list_end(xs) == itr + 1);
      }
    }
    WHEN("applied to an input: \"++123++456++789\"") {
      Source src = Source_new("++123++456++789");
      List(Int) xs = parse(p, src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
        REQUIRE(list_end(xs) == itr + 3);
      }
    }
    WHEN("applied to an input: \"+123++456++789\"") {
      Source src = Source_new("+123++456++789");
      THEN("causes exception(\"expects '+' but was '1'\")") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects '+' but was '1'");
      }
    }
  }

  GIVEN("PARSER(List(String)) p = either(many1(many1(digit)), "
        "many1(skip1st(\"++\", many1(digit))))") {
    PARSER(List(String))
    p = either(many1(many1(digit)), many1(skip1st("++", many1(digit))));
    WHEN("applied to an input: \"123++456++789\"") {
      Source src = Source_new("123++456++789");
      List(String) xs = parse(p, src);
      THEN("results [\"123\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(list_end(xs) == itr + 1);
      }
    }
    WHEN("applied to an input: \"++123++456++789\"") {
      Source src = Source_new("++123++456++789");
      List(String) xs = parse(p, src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(std::string("456") == itr[1]);
        REQUIRE(std::string("789") == itr[2]);
        REQUIRE(list_end(xs) == itr + 3);
      }
    }
    WHEN("applied to an input: \"+123++456++789\"") {
      Source src = Source_new("+123++456++789");
      THEN("causes exception(\"expects '+' but was '1'\")") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects '+' but was '1'");
      }
    }
  }

  cparsec2_end();
}
