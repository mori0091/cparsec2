/* -*- coding: utf-8-unix -*- */

#include "my_cparsec2.h"

// implementation of 'person' parser
Person person_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  parse(token((char)'('), src, ex);
  const char* name = parse(token(many1(letter)), src, ex);
  parse(token((char)','), src, ex);
  int age = parse(number, src, ex);
  parse(token((char)')'), src, ex);
  return (Person){.name = name, .age = age};
}

int main() {
  cparsec2_init();

  PARSER(Person) person = PARSER_GEN(Person)(person_fn, NULL);
  PARSE_TEST(many(person), "(Alice, 19) (Bob, 24)");
  // -> [<Person:.name="Alice", .age=19>, <Person:.name="Bob", .age=24>]

  cparsec2_end();
}
