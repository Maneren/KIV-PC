#ifndef INCLUDE_INSTRUCTIONS_ALL_H_
#define INCLUDE_INSTRUCTIONS_ALL_H_

#include "call.h"
#include "conditions.h"
#include "data.h"
#include "operators.h"

typedef int (*instruction_handler_t)(VM *vm);

extern instruction_handler_t instruction_table[256];

#endif // INCLUDE_INSTRUCTIONS_ALL_H_a
