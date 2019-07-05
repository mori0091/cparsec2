/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

// clang-format off
#define EMOJI_SAMPLE                                              \
  u8"😀🐶🍏⚽️🚗⌚️❤️🏳️👨👩👨🏻👩🏻👨🏼👩🏼👨🏽👩🏽👨🏾👩🏾👨🏿👩🏿"
// clang-format on

SCENARIO("utf8(str)", "[cparsec2][parser][utf8]") {
  cparsec2_init();
  GIVEN(u8"an input: u8\"aiueoあいうえお\"") {
    Source src = Source_new(u8"aiueoあいうえお");
    WHEN(u8"apply utf8(u8\"aiueoあいうえお\")") {
      THEN(u8"results \"aiueoあいうえお\"") {
        REQUIRE(std::string(u8"aiueoあいうえお") ==
                parse(utf8(u8"aiueoあいうえお"), src));
      }
    }
    WHEN("apply many(anyUtf8)") {
      THEN(u8"results [\"a\", \"i\", \"u\", \"e\", \"o\", "
           u8"\"あ\", \"い\", \"う\", \"え\", \"お\"]") {
        List(String) xs = parse(many(anyUtf8), src);
        const char** itr = list_begin(xs);
        REQUIRE(std::string(u8"a") == itr[0]);
        REQUIRE(std::string(u8"i") == itr[1]);
        REQUIRE(std::string(u8"u") == itr[2]);
        REQUIRE(std::string(u8"e") == itr[3]);
        REQUIRE(std::string(u8"o") == itr[4]);
        REQUIRE(std::string(u8"あ") == itr[5]);
        REQUIRE(std::string(u8"い") == itr[6]);
        REQUIRE(std::string(u8"う") == itr[7]);
        REQUIRE(std::string(u8"え") == itr[8]);
        REQUIRE(std::string(u8"お") == itr[9]);
      }
    }
  }
  GIVEN(u8"an input: u8\"κόσμε\"") {
    Source src = Source_new(u8"κόσμε");
    WHEN(u8"apply utf8(u8\"κόσμε\")") {
      THEN(u8"results \"κόσμε\"") {
        REQUIRE(std::string(u8"κόσμε") == parse(utf8(u8"κόσμε"), src));
      }
    }
  }
  GIVEN(u8"an input: u8\"" EMOJI_SAMPLE "\"") {
    Source src = Source_new(EMOJI_SAMPLE);
    WHEN("apply utf8(u8\"" EMOJI_SAMPLE "\")") {
      THEN("results \"" EMOJI_SAMPLE "\"") {
        REQUIRE(std::string(EMOJI_SAMPLE) ==
                parse(utf8(EMOJI_SAMPLE), src));
      }
    }
  }
  cparsec2_end();
}
