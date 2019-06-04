/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("anyChar", "[cparsec2][parser][anyChar]") {
  cparsec2_init();
  GIVEN("an input \"abc\" ") {
    Source src = Source_new("abc");
    WHEN("apply anyChar") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(anyChar, src));
      }
    }
    WHEN("apply anyChar") {
      AND_WHEN("apply anyChar") {
        THEN("results 'a'") {
          REQUIRE('a' == parse(anyChar, src));
          AND_THEN("results 'b'") {
            REQUIRE('b' == parse(anyChar, src));
          }
        }
      }
    }
    WHEN("apply anyChar") {
      AND_WHEN("apply anyChar") {
        THEN("results 'a'") {
          REQUIRE('a' == parse(anyChar, src));
          AND_THEN("results 'b'") {
            REQUIRE('b' == parse(anyChar, src));
            AND_THEN("results 'c'") {
              REQUIRE('c' == parse(anyChar, src));
            }
          }
        }
      }
    }
    WHEN("apply anyChar") {
      AND_WHEN("apply anyChar") {
        AND_WHEN("apply anyChar") {
          THEN("results 'a'") {
            REQUIRE('a' == parse(anyChar, src));
            AND_THEN("results 'b'") {
              REQUIRE('b' == parse(anyChar, src));
              AND_THEN("results 'c'") {
                REQUIRE('c' == parse(anyChar, src));
              }
            }
          }
        }
      }
    }
    WHEN("apply anyChar") {
      AND_WHEN("apply anyChar") {
        AND_WHEN("apply anyChar") {
          AND_WHEN("apply anyChar") {
            THEN("results 'a'") {
              REQUIRE('a' == parse(anyChar, src));
              AND_THEN("results 'b'") {
                REQUIRE('b' == parse(anyChar, src));
                AND_THEN("results 'c'") {
                  REQUIRE('c' == parse(anyChar, src));
                  AND_THEN("cause exception(\"too short\")") {
                    REQUIRE_THROWS_WITH(parse(anyChar, src), "too short");
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
