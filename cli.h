#ifndef INCLUDE_SEMESTRALKA_CLI_H_
#define INCLUDE_SEMESTRALKA_CLI_H_

#include "string.h"
#include <stdio.h>

typedef struct {
  float x_top;
  float x_bottom;
  float y_top;
  float y_bottom;
} Limits;

typedef struct {
  String function;
  FILE *output;
  Limits limits;
} Args;

Args parse_args(int argc, char *argv[]);

#endif // INCLUDE_SEMESTRALKA_CLI_H_
