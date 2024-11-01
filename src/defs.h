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

#define SAFE_FREE(x) ((x && *x) ? free(*x) : 0, *(x) = NULL)
