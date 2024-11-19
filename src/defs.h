#ifndef INCLUDE_KMEMU_DEFS_H_
#define INCLUDE_KMEMU_DEFS_H_

#include <stdlib.h>

#define EXIT_ARGS 1
#define EXIT_FILE 2
#define EXIT_UNKNOWN 3
#define EXIT_EXEC 4
#define EXIT_MEMORY 128

// Use calloc instead of malloc so printing the memory doesn't touch
// uninitialized memory
#define SAFE_ALLOCATE(x, n, s)                                                 \
  x = calloc(n, s);                                                            \
  if (x == NULL) {                                                             \
    fprintf(stderr, "Allocation error. Aborting\n");                           \
    exit(EXIT_MEMORY);                                                         \
  }

// Free the pointer only if it isn't NULL and then set it to NULL
#define SAFE_FREE(x) ((x && *x) ? free(*x) : (void)0, *(x) = NULL)

#endif
