#include "cli.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  Args args = parse_args(argc, argv);

  printf("args: %s", args.function.data);

  return EXIT_SUCCESS;
}
