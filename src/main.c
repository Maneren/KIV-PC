#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#define size_t unsigned int

int main(int argc, char *argv[]) {
  char *filename = "sample/bubble.kmx";

  VM vm = {0};

  if (init_vm_from_file(filename, &vm) != EXIT_SUCCESS) {
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
