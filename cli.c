#include "cli.h"
#include <stdlib.h>

const char *HELP_TEXT = "Usage: graph.exe ⟨func⟩ ⟨out-file⟩ [⟨limits⟩]\n";

void error(char *message) {
  /* print usage to stderr */
  fprintf(stderr, "ERROR: %s\n%s", message, HELP_TEXT);
  exit(1);
}

Limits parse_limits(char *limits_string) {
  Limits limits;

  limits.x_top = strtof(limits_string, &limits_string);
  limits.x_bottom = strtof(limits_string, &limits_string);
  limits.y_top = strtof(limits_string, &limits_string);
  limits.y_bottom = strtof(limits_string, &limits_string);

  return limits;
}

Args parse_args(int argc, char *argv[]) {
  if (argc <= 2) {
    error("Not enough arguments");
  }

  Args args;

  char *function = argv[1];

  String function_string = String_from_cstr(function);

  /* trim quotes and whitespace */
  args.function = String_trim(&function_string, '"');

  args.output = fopen(argv[2], "w");

  if (!args.output) {
    error("Can't open output file");
  }

  Limits limits = {-10.f, 10.f, -10.f, 10.f};

  if (argc > 2) {
    limits = parse_limits(argv[3]);
  }

  args.limits = limits;

  return args;
}
