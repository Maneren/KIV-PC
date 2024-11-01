#include "defs.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#define size_t unsigned int

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s ⟨program.kmx⟩ [⟨output[.txt]⟩]\n", argv[0]);
    return EXIT_ARGS;
  }

  char const *filename = argv[1];

  char const *output_file = NULL;
  if (argc > 2) {
    output_file = argv[2];
  }

  VM vm = {0};

  int status = init_vm_from_file(filename, output_file, &vm);

  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Failed to initialize VM\n");
    vm_free(&vm);
    return status;
  }

  // load 0, 1 or 2 as debug level
  const char *debug_envvar = getenv("DEBUG");
  char debug_level = 0;
  if (debug_envvar) {
    debug_level = debug_envvar[0];

    if (debug_level < '0' || debug_level > '2') {
      fprintf(stderr, "Invalid debug level\n");
      vm_free(&vm);
      return EXIT_ARGS;
    }

    vm.debug = debug_level - '0';
  }

  status = vm_run(&vm);

  if (status != EXIT_SUCCESS) {
    // flush stdout so it doesn't get mixed with the error message
    fflush(stdout);

    fprintf(stderr, "Program execution failed\n");
    if (vm.error_msg)
      fprintf(stderr, "Reason: %s\n", vm.error_msg);
  }

  vm_free(&vm);

  return status;
}
