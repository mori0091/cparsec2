#include "cparsec2.h"

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  /* NOTE: THIS MUST BE CALLED ONCE AT FIRST. */
  cparsec2_init();

  PARSER(String) digit3 = seq(digit, digit, digit);
  PARSE_TEST(digit3, "1234");   /* -> "123" */
  PARSE_TEST(digit3, "123");    /* -> "123" */
  PARSE_TEST(digit3, "12");     /* -> "error:too short" */
  PARSE_TEST(digit3, "a123");   /* -> "error:not satisfy" */

  return 0;
}
