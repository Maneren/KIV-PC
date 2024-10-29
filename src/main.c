#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#define size_t unsigned int

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <program.kmx> [<output[.txt]>]\n", argv[0]);
    return EXIT_FAILURE;
  }

  char const *filename = argv[1];

  char const *output_file = NULL;
  if (argc > 2) {
    output_file = argv[2];
  }

  VM vm = {0};

  if (init_vm_from_file(filename, output_file, &vm) != EXIT_SUCCESS) {
    fprintf(stderr, "Failed to initialize VM\n");
    return EXIT_FAILURE;
  }

  if (vm_run(&vm)) {
    fprintf(stderr, "Failed to run VM\n");
  }

  fflush(stdout);
  fflush(stderr);

  vm_free(&vm);

  return EXIT_SUCCESS;
}
