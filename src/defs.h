#ifndef INCLUDE_KMEMU_DEFS_H_
#define INCLUDE_KMEMU_DEFS_H_

#include <stdlib.h>

#define EXIT_ARGS 1
#define EXIT_FILE 2
#define EXIT_UNKNOWN 3
#define EXIT_EXEC 4
#define EXIT_MEMORY 128

// Try allocating memory
//
// If the allocation fails, print an error message and exit
//
// Uses calloc so printing the memory doesn't touch uninitialized memory
#define SAFE_ALLOCATE(var, count, size)                                        \
  var = calloc(count, size);                                                   \
  if (var == NULL) {                                                           \
    fprintf(stderr, "Allocation error. Aborting\n");                           \
    exit(EXIT_MEMORY);                                                         \
  }

// Try reallocating memory
//
// If the realloc fails, print an error message and exit
#define SAFE_REALLOCATE(var, count, size)                                      \
  var = realloc(var, count * size);                                            \
  if (var == NULL) {                                                           \
    fprintf(stderr, "Allocation error. Aborting\n");                           \
    exit(EXIT_MEMORY);                                                         \
  }

// Try freeing the pointer and set it to NULL
//
// If the pointer is already NULL or points to NULL, do nothing
#define SAFE_FREE(x) ((x && *x) ? free(*x) : (void)0, *(x) = NULL)

// Return the maximum of two values
#define max(a, b) ((a) > (b) ? (a) : (b))

#endif
